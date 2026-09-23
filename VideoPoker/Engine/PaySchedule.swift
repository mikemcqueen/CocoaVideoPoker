//
//  PaySchedule.swift
//  VideoPoker
//
//  Copyright 2010-2011 Mike McQueen. All rights reserved.
//

enum GameID: Int, CaseIterable, Comparable {
    case allAmerican
    case bonusDeuces
    case bonusPoker
    case bonusPokerDeluxe
    case deucesWild
    case doubleBonus
    case doubleDblBonus
    case doubleDeuces
    case jacksOrBetter
    case jokersWild
    case looseDeuces
    case superAces
    case tripleDblBonus
    case whiteHotAces

    static func < (lhs: GameID, rhs: GameID) -> Bool {
        lhs.rawValue < rhs.rawValue
    }
}

/// One line of a pay table definition. In a game's base table, a `group`
/// line starts a group of hands that pay as one line, and the next `group`
/// line ends it (e.g. JACKS OR BETTER = pair of jacks ... pair of aces).
struct Payline {
    struct Flags: OptionSet {
        let rawValue: Int

        static let group  = Flags(rawValue: 1)
        static let noWild = Flags(rawValue: 2)
    }

    let handValue: HandValue
    let payout: Int
    let jackpot: Int
    let flags: Flags

    init(_ handValue: HandValue, _ payout: Int, _ jackpot: Int = 0, flags: Flags = []) {
        self.handValue = handValue
        self.payout = payout
        self.jackpot = jackpot
        self.flags = flags
    }
}

/// A pay table: payouts per paying hand for 1 to 5 coins bet.
final class PaySchedule {
    let gameID: GameID
    /// Estimated return (percent) with perfect play
    let estimatedReturn: Double
    let wildCardNumber: Int

    /// Base table used for matching hand values to paylines
    private let baseLines: [Payline]
    private var lines: [(handValue: HandValue, payouts: [Int])] = []
    private var lowestHandValue = HandValue.royalFlush

    init(gameID: GameID, base: [Payline], overrides: [Payline], coinCount: Int = 5,
         estimatedReturn: Double, wildCardNumber: Int)
    {
        self.gameID = gameID
        self.baseLines = base
        self.estimatedReturn = estimatedReturn
        self.wildCardNumber = wildCardNumber

        var inGroup = false
        for line in base {
            let isGroup = line.flags.contains(.group)
            // a line in a group, or not a group header
            if inGroup || !isGroup {
                lowestHandValue = min(lowestHandValue, line.handValue)
            }
            if !inGroup {
                lines.append((line.handValue, PaySchedule.payouts(for: line, coinCount: coinCount)))
                inGroup = isGroup
            } else if isGroup {
                inGroup = false
            }
        }

        for line in overrides {
            lines[paylineIndex(of: line.handValue)].payouts = PaySchedule.payouts(for: line, coinCount: coinCount)
        }
    }

    private static func payouts(for line: Payline, coinCount: Int) -> [Int] {
        var total = 0
        return (1...coinCount).map { coin in
            if coin == coinCount && line.jackpot != 0 {
                total = line.jackpot
            } else {
                total += line.payout
            }
            return total
        }
    }

    var game: Game {
        PayScheduleCatalog.shared.game(gameID)!
    }

    var paylineCount: Int {
        lines.count
    }

    func paylineIndex(of handValue: HandValue) -> Int {
        guard let index = lines.firstIndex(where: { $0.handValue == handValue }) else {
            preconditionFailure("paylineIndex(): bad handValue \(handValue.rawValue)")
        }
        return index
    }

    func handValue(ofPayline index: Int) -> HandValue {
        lines[index].handValue
    }

    func payout(ofPayline index: Int, coinCount: Int) -> Int {
        lines[index].payouts[coinCount - 1]
    }

    func payout(for handValue: HandValue, coinCount: Int) -> Int {
        payout(ofPayline: paylineIndex(of: handValue), coinCount: coinCount)
    }

    /// Value of the hand's cards in this game's pay table (`.none` if it doesn't pay)
    func payingValue(of cards: [Card]) -> HandValue {
        match(HandAnalyzer.value(of: cards, wildCardNumber: wildCardNumber))
    }

    /// Maps a hand value to the payline it pays as, or `.none`.
    func match(_ handValue: HandValue) -> HandValue {
        if handValue < lowestHandValue {
            return .none
        }

        var bestMatch = HandValue.none
        var inGroup = false
        var waitingForGroup = false

        for line in baseLines.reversed() {
            let isGroup = line.flags.contains(.group)
            if isGroup {
                inGroup.toggle()
            }
            if waitingForGroup {
                if isGroup {
                    bestMatch = line.handValue
                    waitingForGroup = false
                }
                continue
            }
            if matches(line, handValue) {
                if !inGroup {
                    bestMatch = line.handValue
                } else {
                    waitingForGroup = true
                }
            }
        }
        return bestMatch
    }

    private func matches(_ line: Payline, _ value: HandValue) -> Bool {
        let lineValue = line.handValue
        if lineValue.rank != value.rank {
            return false
        }
        if line.flags.contains(.noWild) && value.isWild {
            return false
        }
        if lineValue.isSuited && lineValue.suit != value.suit {
            return false
        }
        if lineValue.firstKicker != 0 {
            if lineValue.firstKicker != value.firstKicker {
                return false
            }
            if lineValue.secondKicker != 0 {
                if lineValue.secondKicker != value.secondKicker {
                    return false
                }
                if lineValue.thirdKicker != 0 && lineValue.thirdKicker != value.thirdKicker {
                    return false
                }
            }
        }
        return true
    }
}

final class Game {
    let id: GameID
    let name: String
    /// Solver data file name (in the app bundle)
    let solverFilename: String
    let deckSize: Int
    /// Sorted by descending estimated return
    let schedules: [PaySchedule]

    init(id: GameID, name: String, solverFilename: String, deckSize: Int = 52, schedules: [PaySchedule]) {
        self.id = id
        self.name = name
        self.solverFilename = solverFilename
        self.deckSize = deckSize
        self.schedules = schedules.sorted { $0.estimatedReturn > $1.estimatedReturn }
    }
}

/// All games and their pay schedules, ordered by game ID.
final class PayScheduleCatalog {
    static let shared = PayScheduleCatalog(games: PayTables.games)

    let games: [Game]

    init(games: [Game]) {
        self.games = games.sorted { $0.id < $1.id }
    }

    func game(_ id: GameID) -> Game? {
        games.first { $0.id == id }
    }

    func index(of id: GameID) -> Int? {
        games.firstIndex { $0.id == id }
    }

    func schedule(_ id: GameID, estimatedReturn: Double) -> PaySchedule? {
        game(id)?.schedules.first { $0.estimatedReturn == estimatedReturn }
    }
}
