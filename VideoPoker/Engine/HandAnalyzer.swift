//
//  HandAnalyzer.swift
//  VideoPoker
//
//  Copyright (C) 2009-2011 Mike McQueen. All rights reserved.
//

/// Determines the value of a 5 card hand, optionally with a wild card number
/// (deuces or jokers).
///
/// Note the checks share state: wild cards "used" by a pair check are no longer
/// available to the next check, and kickers are left over between checks. The
/// solver data was generated with this exact behavior, so keep it intact.
struct HandAnalyzer {
    private static let maxValues = 16
    private static let cardsPerHand = 5

    private let cards: [Card]
    private let wildCardNumber: Int

    private var values = [Int](repeating: 0, count: maxValues)   // count of each card number
    private var suits = [Int](repeating: 0, count: Suit.allCases.count)
    private var kickers = [Int](repeating: 0, count: cardsPerHand)
    private var kickerCount = 0
    private var wildCardCount = 0

    init(cards: [Card], wildCardNumber: Int = 0) {
        self.cards = cards
        self.wildCardNumber = wildCardNumber
    }

    static func value(of cards: [Card], wildCardNumber: Int = 0) -> HandValue {
        var analyzer = HandAnalyzer(cards: cards, wildCardNumber: wildCardNumber)
        return analyzer.value()
    }

    private typealias Check = (inout HandAnalyzer) -> UInt

    /// Hands to check, best first. A check is skipped (when no wild cards are
    /// held) unless the hand's max same-suit count, max same-number count and
    /// unique number count match the non-zero requirements.
    private static let checks: [(value: HandValue, sameSuit: Int, sameNumber: Int, uniqueNumbers: Int, check: Check)] = [
        (.fourOfAKind,   0, 0, 0, { $0.fourDeuces() }),
        (.straightFlush, 5, 0, 5, { $0.straightFlush() }),
        (.fiveOfAKind,   0, 0, 0, { $0.fiveOfAKind() }),
        (.fourOfAKind,   0, 4, 2, { $0.fourKind() }),
        (.fullHouse,     0, 3, 2, { $0.fullHouse() }),
        (.flush,         5, 0, 5, { $0.flush() }),
        (.straight,      0, 0, 5, { $0.straight() }),
        (.threeKind,     0, 3, 3, { $0.threeKind() }),
        (.twoPair,       0, 2, 3, { $0.twoPair() }),
        (.onePair,       0, 2, 4, { $0.onePair() }),
    ]

    mutating func value() -> HandValue {
        let (maxSuitCount, maxNumberCount, uniqueNumberCount) = countSuitsAndValues()
        let wildCards = wildCardCount

        for entry in HandAnalyzer.checks {
            wildCardCount = wildCards

            if wildCards > 0 ||
                ((entry.sameSuit == 0 || entry.sameSuit == maxSuitCount) &&
                 (entry.sameNumber == 0 || entry.sameNumber == maxNumberCount) &&
                 (entry.uniqueNumbers == 0 || entry.uniqueNumbers == uniqueNumberCount))
            {
                let cardValue = entry.check(&self)
                if cardValue > 0 {
                    return HandValue(rawValue: entry.value.rawValue | cardValue)
                }
            }
        }
        return .none
    }

    private mutating func countSuitsAndValues() -> (maxSuitCount: Int, maxNumberCount: Int, uniqueNumberCount: Int) {
        var maxSuitCount = 0
        var maxNumberCount = 0
        var uniqueNumberCount = 0

        for card in cards {
            if card.number == wildCardNumber {
                wildCardCount += 1
                continue
            }
            values[card.number] += 1
            let numberCount = values[card.number]
            maxNumberCount = max(maxNumberCount, numberCount)
            if numberCount == 1 {
                uniqueNumberCount += 1
            }
            suits[card.suit.rawValue] += 1
            maxSuitCount = max(maxSuitCount, suits[card.suit.rawValue])
        }
        values[CardNumber.aceLow] = values[CardNumber.ace]
        return (maxSuitCount, maxNumberCount, uniqueNumberCount)
    }

    private func makeKicker(wild: Bool = false) -> UInt {
        if kickerCount == 0 {
            return 0
        }
        var kicker: UInt = 0
        for index in 0..<6 {
            if index < kickerCount {
                kicker |= UInt(kickers[index])
            }
            kicker <<= 4
        }
        return kicker | (wild ? HandValue.Flags.wild.rawValue : 0)
    }

    private mutating func highHand(_ values: [Int]) -> UInt {
        kickerCount = 0
        if let number = (CardNumber.deuce...CardNumber.ace).reversed().first(where: { values[$0] > 0 }) {
            kickers[0] = number
            kickerCount = 1
        }
        return makeKicker()
    }

    /// Highest number that makes `count` of a kind (using wild cards), except
    /// `ignoring`. Wild cards needed are used up.
    private mutating func ofAKind(_ count: Int, ignoring ignoreNumber: Int = 0) -> Int {
        for number in (CardNumber.deuce...CardNumber.ace).reversed() where number != ignoreNumber {
            if values[number] + wildCardCount >= count {
                wildCardCount -= max(0, count - values[number])
                return number
            }
        }
        return 0
    }

    private mutating func onePair() -> UInt {
        let pair = ofAKind(2)
        if pair == 0 {
            return 0
        }
        kickers[0] = pair
        kickerCount = 1
        return makeKicker()
    }

    private mutating func twoPair() -> UInt {
        kickerCount = 0
        let highPair = ofAKind(2)
        if highPair != 0 {
            let lowPair = ofAKind(2, ignoring: highPair)
            if lowPair != 0 {
                kickers[0] = highPair
                kickers[1] = lowPair
                kickerCount = 2
            }
        }
        return makeKicker()
    }

    private mutating func threeKind() -> UInt {
        let trips = ofAKind(3)
        if trips == 0 {
            return 0
        }
        kickers[0] = trips
        kickerCount = 1
        return makeKicker()
    }

    private mutating func checkStraight(_ values: [Int]) -> UInt {
        let cardsNeeded = HandAnalyzer.cardsPerHand - wildCardCount
        var cardsFound = 0
        var highCard = CardNumber.ace
        while highCard >= 5 {
            cardsFound = (0..<HandAnalyzer.cardsPerHand).filter { values[highCard - $0] > 0 }.count
            if cardsFound >= cardsNeeded {
                break
            }
            cardsFound = 0
            highCard -= 1
        }
        if cardsFound >= cardsNeeded {
            kickers[0] = highCard
            kickerCount = 1
            return makeKicker(wild: HandAnalyzer.cardsPerHand > cardsFound)
        }
        return 0
    }

    private mutating func straight() -> UInt {
        checkStraight(values)
    }

    private mutating func flush() -> UInt {
        guard let suit = Suit.allCases.first(where: { suits[$0.rawValue] + wildCardCount >= 5 }) else {
            return 0
        }
        // doesn't take wild cards into account
        var suitValues = [Int](repeating: 0, count: HandAnalyzer.maxValues)
        for card in cards where card.suit == suit {
            suitValues[card.number] += 1
        }
        return UInt(suit.rawValue) | highHand(suitValues)
    }

    private mutating func fullHouse() -> UInt {
        let trips = ofAKind(3)
        if trips != 0 {
            let pair = ofAKind(2, ignoring: trips)
            if pair != 0 {
                kickers[0] = trips
                kickers[1] = pair
                kickerCount = 2
            }
        }
        return makeKicker()
    }

    private mutating func fourKind() -> UInt {
        var quads = 0
        var kicker = 0
        for number in (CardNumber.deuce...CardNumber.ace).reversed() {
            if values[number] + wildCardCount >= 4 {
                quads = number
            } else if values[number] > 0 && kicker == 0 {
                kicker = number
            }
            if kicker != 0 && quads != 0 {
                break
            }
        }
        if quads == 0 {
            return 0
        }
        kickers[0] = quads
        kickers[1] = kicker
        kickerCount = 2
        return makeKicker()
    }

    private mutating func straightFlush() -> UInt {
        var highStraight: UInt = 0
        var highSuit: UInt = 0

        for suit in Suit.allCases.reversed() where suits[suit.rawValue] + wildCardCount >= 5 {
            var suitValues = [Int](repeating: 0, count: HandAnalyzer.maxValues)
            for card in cards where card.number != wildCardNumber && card.suit == suit {
                suitValues[card.number] += 1
                if card.number == CardNumber.ace {
                    suitValues[CardNumber.aceLow] += 1
                }
            }
            let straight = checkStraight(suitValues)
            if straight > highStraight {
                highStraight = straight
                highSuit = UInt(suit.rawValue)
            }
        }
        return highSuit | highStraight
    }

    private mutating func fourDeuces() -> UInt {
        kickerCount = 0
        if wildCardCount == 4 && wildCardNumber == CardNumber.deuce {
            kickers[0] = wildCardNumber
            kickerCount = 1
        }
        return makeKicker()
    }

    private mutating func fiveOfAKind() -> UInt {
        guard let fives = (CardNumber.deuce...CardNumber.ace).reversed().first(where: { values[$0] + wildCardCount >= 5 }) else {
            return 0
        }
        kickers[0] = fives
        kickerCount = 1
        return makeKicker()
    }
}
