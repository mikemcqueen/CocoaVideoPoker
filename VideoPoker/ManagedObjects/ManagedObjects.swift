//
//  ManagedObjects.swift
//  VideoPoker
//
//  Core Data classes for the entities in ManagedObjects.xcdatamodeld.
//

import CoreData

/// A run of hands played at one pay schedule. Balance is in cents.
final class Session: NSManagedObject {
    @NSManaged var actualReturn: Double
    @NSManaged var balance: Int32
    @NSManaged var denomination: Int32
    @NSManaged var endDate: Date?
    @NSManaged var gameId: Int32
    @NSManaged var gameReturn: Double
    @NSManaged var handCount: Int32
    @NSManaged var perfectPlayCount: Int32
    @NSManaged var startDate: Date?
    @NSManaged var fundings: Set<Funding>
    @NSManaged var hands: Set<Hand>

    static let entityName = "session"

    @nonobjc class func fetchRequest() -> NSFetchRequest<Session> {
        NSFetchRequest<Session>(entityName: entityName)
    }

    var gameID: GameID? {
        GameID(rawValue: Int(gameId))
    }
}

/// Cash added to a session, in cents
final class Funding: NSManagedObject {
    @NSManaged var amount: Int32
    @NSManaged var date: Date?
    @NSManaged var session: Session?

    static let entityName = "funding"
}

/// One deal and draw
final class Hand: NSManagedObject {
    @NSManaged var bet: Int32
    @NSManaged var payout: NSNumber?
    @NSManaged var startDate: Date?
    @NSManaged var session: Session?
    @NSManaged var dealtHand: HandData?
    @NSManaged var bestHand: HandData?
    @NSManaged var yourHand: HandData?
    @NSManaged var finalHand: HandData?

    static let entityName = "hand"

    @nonobjc class func fetchRequest() -> NSFetchRequest<Hand> {
        NSFetchRequest<Hand>(entityName: entityName)
    }

    func addDealtHand(cards: Int, value: HandValue, ev: Double) {
        precondition(dealtHand == nil, "addDealtHand: dealtHand non-nil")
        dealtHand = makeHandData(cards: cards, value: value, ev: ev)
    }

    func addBestHand(cards: Int, value: HandValue, ev: Double) {
        precondition(bestHand == nil, "addBestHand: bestHand non-nil")
        bestHand = makeHandData(cards: cards, value: value, ev: ev)
    }

    func addOrUpdateYourHand(cards: Int, value: HandValue, ev: Double) {
        if let yourHand = yourHand {
            yourHand.update(cards: cards, value: value, ev: ev)
        } else {
            yourHand = makeHandData(cards: cards, value: value, ev: ev)
        }
    }

    func addFinalHand(cards: Int, value: HandValue, ev: Double) {
        precondition(finalHand == nil, "addFinalHand: finalHand non-nil")
        finalHand = makeHandData(cards: cards, value: value, ev: ev)
    }

    private func makeHandData(cards: Int, value: HandValue, ev: Double) -> HandData {
        let data = HandData(context: managedObjectContext!)
        data.hand = self
        data.update(cards: cards, value: value, ev: ev)
        return data
    }
}

/// Cards (packed with Card.uniqueValue), hand value and expected value of a
/// hand at one point of play
final class HandData: NSManagedObject {
    @NSManaged var cards: Int32
    @NSManaged var ev: Double
    @NSManaged var value: Int32
    @NSManaged var hand: Hand?

    static let entityName = "handData"

    func update(cards: Int, value: HandValue, ev: Double) {
        self.cards = Int32(truncatingIfNeeded: cards)
        // group hand values don't fit; stored truncated, as they always were
        self.value = Int32(truncatingIfNeeded: value.rawValue)
        self.ev = ev
    }
}
