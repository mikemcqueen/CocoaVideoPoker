//
//  GameModel.swift
//  VideoPoker
//
//  Created by Mike McQueen on 7/24/10.
//

import CoreData
import UIKit

enum GameModelError: Error {
    case noPaySchedule(GameID?, Double)
}

/// The game in play: pay schedule, solver, deck, current hand, bet, and the
/// Core Data session history. Instantiated by MainWindow.xib and shared by the
/// Train, Solve and History tabs.
final class GameModel: NSObject {
    private static let defaultGameID = GameID.jacksOrBetter
    private static let defaultReturn = 99.54
    private static let defaultBalance: Int32 = 10000        // cents
    private static let defaultDenomination: Int32 = 25      // cents

    private(set) var schedule: PaySchedule!
    private(set) var solver: SolverData!
    private(set) var deck: Deck!
    /// The hand of whichever tab is showing
    var hand: CurrentHand!

    private(set) var bet = 5
    private let maxBet = 5
    private(set) var handValueChanged = false

    private(set) var session: Session?
    private var handRecord: Hand?

    private var model: NSManagedObjectModel?
    private var stores: NSPersistentStoreCoordinator?
    private var storesIncompatible = false
    private var managedContext: NSManagedObjectContext?

    override init() {
        super.init()
        do {
            try loadOrStartNewSession()
        } catch {
            NSLog("Couldn't load session (\(error)); deleting history")
            deleteStores()
            try! loadOrStartNewSession()
        }
    }

    static var storeURL: URL {
        documentsDirectory.appendingPathComponent("VideoPoker.sqlite")
    }

    static var documentsDirectory: URL {
        FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).last!
    }

    // MARK: - Sessions

    private func loadOrStartNewSession() throws {
        let gameID = GameModel.defaultGameID
        let gameReturn = GameModel.defaultReturn

        guard context != nil else {
            try setPaySchedule(PayScheduleCatalog.shared.schedule(gameID, estimatedReturn: gameReturn), loadSolver: true)
            return
        }

        session = loadLastSession() ?? startNewSession(gameID, estimatedReturn: gameReturn,
                                                       balance: GameModel.defaultBalance,
                                                       denomination: GameModel.defaultDenomination)
        if let session = session, !GameModel.hasSolverData(for: session.gameID) {
            NSLog("No solver data for saved game \(session.gameId) - starting default game")
            self.session = startNewSession(gameID, estimatedReturn: gameReturn,
                                           balance: session.balance, denomination: session.denomination)
        }
        let session = self.session!
        try setPaySchedule(session.gameID.flatMap { PayScheduleCatalog.shared.schedule($0, estimatedReturn: session.gameReturn) },
                           loadSolver: true)
    }

    private func loadLastSession() -> Session? {
        let request = Session.fetchRequest()
        request.sortDescriptors = [NSSortDescriptor(key: "startDate", ascending: false)]
        request.fetchLimit = 1
        let session = (try? context?.fetch(request))?.first
        if session != nil {
            NSLog("last session loaded")
        }
        return session
    }

    private func startNewSession(_ gameID: GameID, estimatedReturn: Double, balance: Int32, denomination: Int32) -> Session {
        let session = Session(context: context!)
        session.gameId = Int32(gameID.rawValue)
        session.gameReturn = estimatedReturn
        session.balance = balance
        session.denomination = denomination
        session.startDate = Date()
        return session
    }

    /// Switches to another pay schedule, starting a new session.
    /// Returns false if it's the current schedule or there's no solver data for it.
    func switchTo(_ newSchedule: PaySchedule) -> Bool {
        if newSchedule === schedule {
            NSLog("Same pay schedule - ignoring")
            return false
        }
        if !GameModel.hasSolverData(for: newSchedule.gameID) {
            NSLog("No solver data for game \(newSchedule.gameID) - ignoring")
            return false
        }

        if let oldSession = session {
            save()
            session = startNewSession(newSchedule.gameID, estimatedReturn: newSchedule.estimatedReturn,
                                      balance: oldSession.balance, denomination: oldSession.denomination)
        }

        // TODO: if base pay schedule is the same don't need to reload?
        let loadSolver = schedule.gameID != newSchedule.gameID
        do {
            try setPaySchedule(newSchedule, loadSolver: loadSolver)
        } catch {
            NSLog("Couldn't switch to pay schedule: \(error)")
            return false
        }
        return true
    }

    private func setPaySchedule(_ newSchedule: PaySchedule?, loadSolver: Bool) throws {
        guard let newSchedule = newSchedule else {
            throw GameModelError.noPaySchedule(session?.gameID, session?.gameReturn ?? 0)
        }
        if loadSolver {
            let game = newSchedule.game
            solver = try SolverData(schedule: newSchedule, deckSize: game.deckSize,
                                    url: GameModel.solverURL(for: game.id))
            deck = Deck(size: game.deckSize)
        }
        schedule = newSchedule
    }

    static func solverURL(for gameID: GameID) -> URL {
        let filename = PayScheduleCatalog.shared.game(gameID)!.solverFilename
        return Bundle.main.resourceURL!.appendingPathComponent(filename)
    }

    static func hasSolverData(for gameID: GameID?) -> Bool {
        guard let gameID = gameID, PayScheduleCatalog.shared.game(gameID) != nil else {
            return false
        }
        return FileManager.default.fileExists(atPath: solverURL(for: gameID).path)
    }

    // MARK: - Play

    func setCard(_ card: Card, at index: Int) {
        hand.setCard(card, at: index, solver: solver)
    }

    func setCards(_ cards: [Card]) {
        hand.setCards(cards, solver: solver)
        hand.clearHeldCards()
    }

    /// Balance is stored in cents and may not go negative (model validation)
    var canCoverBet: Bool {
        guard let session = session else {
            return true
        }
        return session.balance >= Int32(bet) * session.denomination
    }

    /// Cash balance in dollars
    var cash: Double {
        Double(session?.balance ?? 0) / 100.0
    }

    func addFunds(_ amount: Int32) {
        guard let session = session else {
            return
        }
        let funding = Funding(context: session.managedObjectContext!)
        funding.amount = amount
        funding.date = Date()
        funding.session = session
        session.balance += amount
        save()
    }

    /// Shuffles and deals a new hand. Returns false if the balance can't cover the bet.
    func deal() -> Bool {
        if !canCoverBet {
            NSLog("deal: balance \(session?.balance ?? 0) can't cover bet")
            return false
        }

        deck.shuffle()
        setCards((0..<5).map { _ in deck.deal() })

        let dealtCards = hand.cards
        let dealtHandValue = schedule.payingValue(of: dealtCards)
        let bestCards = hand.bestCards

        if let session = session {
            let record = Hand(context: session.managedObjectContext!)
            record.startDate = Date()
            record.bet = Int32(bet)
            record.addDealtHand(cards: Card.uniqueValue(of: dealtCards), value: dealtHandValue, ev: ev(forCardBits: 0))
            record.addBestHand(cards: Card.uniqueValue(of: bestCards), value: schedule.payingValue(of: bestCards), ev: bestEv)
            record.session = session
            session.balance -= Int32(bet) * session.denomination
            handRecord = record
        }
        setHandValue(dealtHandValue)

        save()
        return true
    }

    /// Replaces the cards that aren't held. Returns the payout in coins.
    func draw() -> Int {
        for index in 0..<5 where !hand.isHeld(hand.card(at: index)) {
            hand.setCard(deck.deal(), at: index)
        }

        let finalCards = hand.cards
        let finalHandValue = schedule.payingValue(of: finalCards)
        handRecord?.addFinalHand(cards: Card.uniqueValue(of: finalCards), value: finalHandValue, ev: 0.0)
        setHandValue(finalHandValue)

        var payout = 0
        if finalHandValue != .none {
            payout = schedule.payout(for: finalHandValue, coinCount: bet)
        }

        if let session = session {
            if payout > 0 {
                session.balance += Int32(payout) * session.denomination
            }
            session.handCount += 1
            if handRecord?.yourHand?.ev ?? 0 == handRecord?.bestHand?.ev ?? 0 {
                session.perfectPlayCount += 1
            }
        }

        save()
        return payout
    }

    /// The paying value of the current hand, highlighted in the pay table
    private(set) var handValue = HandValue.none

    private func setHandValue(_ value: HandValue) {
        handValueChanged = value != handValue
        handValue = value
    }

    /// Records and returns the expected value of the held cards
    @discardableResult
    func updateEv() -> Double {
        let heldCards = hand.heldCards
        let ev = self.ev(forCardBits: hand.cardBits(of: heldCards))
        handRecord?.addOrUpdateYourHand(cards: Card.uniqueValue(of: heldCards), value: .none, ev: ev)
        return ev
    }

    var bestEv: Double {
        solver.bestResults.ev
    }

    func ev(forCardBits bits: Int) -> Double {
        solver.results[bits].ev
    }

    func betOne() {
        bet = bet < maxBet ? bet + 1 : 1
    }

    // MARK: - Core Data stack

    func save() {
        guard let context = managedContext, context.hasChanges else {
            return
        }
        do {
            try context.save()
        } catch {
            NSLog("save(): Unresolved error \(error)")
            abort()
        }
    }

    var context: NSManagedObjectContext? {
        if managedContext == nil, let coordinator = persistentStores {
            let context = NSManagedObjectContext(concurrencyType: .mainQueueConcurrencyType)
            context.persistentStoreCoordinator = coordinator
            managedContext = context
        }
        return managedContext
    }

    private var managedObjectModel: NSManagedObjectModel {
        if model == nil {
            let url = Bundle.main.url(forResource: "ManagedObjects", withExtension: "momd")!
            model = NSManagedObjectModel(contentsOf: url)
        }
        return model!
    }

    private var persistentStores: NSPersistentStoreCoordinator? {
        if stores == nil && !storesIncompatible {
            initStores()
        }
        return stores
    }

    private func initStores() {
        let coordinator = NSPersistentStoreCoordinator(managedObjectModel: managedObjectModel)
        do {
            try coordinator.addPersistentStore(ofType: NSSQLiteStoreType, configurationName: nil,
                                               at: GameModel.storeURL, options: nil)
            stores = coordinator
        } catch let error as NSError {
            NSLog("addPersistentStore: \(error), \(error.userInfo)")
            guard error.code == NSPersistentStoreIncompatibleVersionHashError else {
                abort()
            }
            // Play without history until the user decides; the window isn't up yet
            storesIncompatible = true
            DispatchQueue.main.async {
                self.askDeleteIncompatibleStores()
            }
        }
    }

    private func askDeleteIncompatibleStores() {
        let window = UIApplication.shared.connectedScenes
            .compactMap { ($0 as? UIWindowScene)?.keyWindow }.first
        guard let root = window?.rootViewController else {
            return
        }
        GameModel.askDeleteHistory(from: root) { delete in
            guard delete else {
                abort()
            }
            self.deleteStores()
            try? self.loadOrStartNewSession()
        }
    }

    /// Asks the user to confirm deleting all history
    static func askDeleteHistory(from viewController: UIViewController, completion: @escaping (Bool) -> Void) {
        let alert = UIAlertController(title: "Confirm", message: "Delete all history?", preferredStyle: .alert)
        alert.addAction(UIAlertAction(title: "No", style: .default) { _ in completion(false) })
        alert.addAction(UIAlertAction(title: "Yes", style: .default) { _ in completion(true) })
        viewController.present(alert, animated: true)
    }

    func deleteStores() {
        let fileManager = FileManager.default
        for store in stores?.persistentStores ?? [] {
            var success = true
            do {
                try stores?.remove(store)
            } catch {
                NSLog("deleteStores: removePersistentStore: \(error)")
                success = false
            }
            if let url = store.url {
                do {
                    try fileManager.removeItem(at: url)
                } catch {
                    NSLog("deleteStores: removeItem: \(error)")
                    success = false
                }
            }
            if success {
                NSLog("deleteStores: store deleted")
            }
        }

        do {
            try fileManager.removeItem(at: GameModel.storeURL)
            NSLog("deleteStores: storeURL deleted")
        } catch {
            NSLog("deleteStores: removeItem(storeURL): \(error)")
        }

        stores = nil
        storesIncompatible = false
        managedContext = nil
        session = nil
        handRecord = nil
    }
}

/// Something that can provide the game model (for nib outlets)
@objc protocol GameModelContainer: AnyObject {
    var game: GameModel! { get }
}
