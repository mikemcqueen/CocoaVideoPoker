//
//  HandValue.swift
//  VideoPoker
//
//  Copyright (C) 2009-2011 Mike McQueen. All rights reserved.
//

enum HandRank: UInt {
    case highCard      = 0
    case onePair       = 1
    case twoPair       = 2
    case threeKind     = 3
    case straight      = 4
    case flush         = 5
    case fullHouse     = 6
    case fourKind      = 7
    case straightFlush = 8
    case fiveKind      = 9

    /// Pay table "group" headers (e.g. JACKS OR BETTER), not a real hand rank
    case group         = 15
}

/// A packed hand value:
///
///     bits 28-31  rank
///     bits 24-27  first kicker (card number)
///     bits 20-23  second kicker
///     bits 16-19  third kicker
///     bit  5      wild card(s) used
///     bit  4      suited (pay table value names a suit)
///     bit  3      exact (four of a kind values must match the kickers)
///     bits 0-2    suit
///
/// Greater values are better hands. Zero is no paying hand.
struct HandValue: RawRepresentable, Hashable, Comparable {
    let rawValue: UInt

    init(rawValue: UInt) {
        self.rawValue = rawValue
    }

    struct Flags: OptionSet {
        let rawValue: UInt

        static let exact  = Flags(rawValue: 0x08)
        static let suited = Flags(rawValue: 0x10)
        static let wild   = Flags(rawValue: 0x20)
    }

    static let none = HandValue(rawValue: 0)

    static func kickers(_ first: Int, _ second: Int = 0) -> UInt {
        UInt(first) << 24 | UInt(second) << 20
    }

    init(_ rank: HandRank, _ cardValues: UInt = 0, _ flags: Flags = [], suit: Suit? = nil) {
        var flags = flags
        var suitBits: UInt = 0
        if let suit = suit {
            suitBits = UInt(suit.rawValue)
            flags.insert(.suited)
        }
        rawValue = rank.rawValue << 28 | cardValues | suitBits | flags.rawValue
    }

    var rank: UInt           { (rawValue >> 28) & 0xf }
    var firstKicker: UInt    { (rawValue >> 24) & 0xf }
    var secondKicker: UInt   { (rawValue >> 20) & 0xf }
    var thirdKicker: UInt    { (rawValue >> 16) & 0xf }
    var suit: UInt           { rawValue & 0x7 }
    var isExact: Bool        { rawValue & Flags.exact.rawValue != 0 }
    var isSuited: Bool       { rawValue & Flags.suited.rawValue != 0 }
    var isWild: Bool         { rawValue & Flags.wild.rawValue != 0 }

    static func < (lhs: HandValue, rhs: HandValue) -> Bool {
        lhs.rawValue < rhs.rawValue
    }

    private static func group(_ id: UInt) -> HandValue {
        HandValue(.group, id)
    }

    private static func fourOf(_ number: Int, with kicker: Int = 0) -> HandValue {
        HandValue(.fourKind, kickers(number, kicker), .exact)
    }

    // Pay table groups
    static let fourAcesWith234          = group(1)
    static let four2s3s4sWithAce234     = group(2)
    static let four2s3s4s               = group(3)
    static let four5sThruKings          = group(4)
    static let royalFlushNoDeuces       = group(5)
    static let royalFlushWithDeuce      = group(6)
    static let jacksOrBetter            = group(7)
    static let kingsOrBetter            = group(8)
    static let royalFlushNoJokers       = group(9)
    static let royalFlushWithJoker      = group(10)

    static let fiveOfAKind              = HandValue(.fiveKind)
    static let royalFlush               = HandValue(.straightFlush, kickers(CardNumber.ace))
    static let royalFlushWild           = HandValue(.straightFlush, kickers(CardNumber.ace), .wild)
    static let royalFlushClubs          = HandValue(.straightFlush, kickers(CardNumber.ace), suit: .clubs)
    static let royalFlushDiamonds       = HandValue(.straightFlush, kickers(CardNumber.ace), suit: .diamonds)
    static let royalFlushHearts         = HandValue(.straightFlush, kickers(CardNumber.ace), suit: .hearts)
    static let royalFlushSpades         = HandValue(.straightFlush, kickers(CardNumber.ace), suit: .spades)
    static let straightFlush            = HandValue(.straightFlush)
    static let fourAcesWithTwo          = fourOf(CardNumber.ace, with: 2)
    static let fourAcesWithThree        = fourOf(CardNumber.ace, with: 3)
    static let fourAcesWithFour         = fourOf(CardNumber.ace, with: 4)
    static let fourTwosWithAce          = fourOf(2, with: CardNumber.ace)
    static let fourTwosWithThree        = fourOf(2, with: 3)
    static let fourTwosWithFour         = fourOf(2, with: 4)
    static let fourThreesWithAce        = fourOf(3, with: CardNumber.ace)
    static let fourThreesWithTwo        = fourOf(3, with: 2)
    static let fourThreesWithFour       = fourOf(3, with: 4)
    static let fourFoursWithAce         = fourOf(4, with: CardNumber.ace)
    static let fourFoursWithTwo         = fourOf(4, with: 2)
    static let fourFoursWithThree       = fourOf(4, with: 3)
    static let fourAces                 = fourOf(CardNumber.ace)
    static let fourTwos                 = fourOf(2)
    static let fourDeuces               = fourTwos
    static let fourThrees               = fourOf(3)
    static let fourFours                = fourOf(4)
    static let fourFives                = fourOf(5)
    static let fourSixes                = fourOf(6)
    static let fourSevens               = fourOf(7)
    static let fourEights               = fourOf(8)
    static let fourNines                = fourOf(9)
    static let fourTens                 = fourOf(10)
    static let fourJacks                = fourOf(CardNumber.jack)
    static let fourQueens               = fourOf(CardNumber.queen)
    static let fourKings                = fourOf(CardNumber.king)
    static let fourOfAKind              = HandValue(.fourKind, 0, .exact)
    static let fullHouse                = HandValue(.fullHouse)
    static let flush                    = HandValue(.flush)
    static let straight                 = HandValue(.straight)
    static let threeKind                = HandValue(.threeKind)
    static let twoPair                  = HandValue(.twoPair)
    static let pairOfJacks              = HandValue(.onePair, kickers(CardNumber.jack))
    static let pairOfQueens             = HandValue(.onePair, kickers(CardNumber.queen))
    static let pairOfKings              = HandValue(.onePair, kickers(CardNumber.king))
    static let pairOfAces               = HandValue(.onePair, kickers(CardNumber.ace))
    static let onePair                  = HandValue(.onePair)

    private static let names: [HandValue: String] = [
        fourAcesWith234:        "4 ACES WITH ANY 2,3,4",
        four2s3s4sWithAce234:   "4 2s, 3s, 4s W/ACE,2,3,4",
        four2s3s4s:             "4 2s, 3s, 4s",
        four5sThruKings:        "4 5s THRU KINGS",
        royalFlushNoDeuces:     "ROYAL FLUSH NO DEUCES",
        royalFlushWithDeuce:    "ROYAL FLUSH WITH DEUCE",
        royalFlushNoJokers:     "NATURAL ROYAL FLUSH",
        royalFlushWithJoker:    "WILD ROYAL FLUSH",
        fiveOfAKind:            "5 OF A KIND",
        royalFlush:             "ROYAL FLUSH",
        straightFlush:          "STRAIGHT FLUSH",
        fourOfAKind:            "4 OF A KIND",
        fourAces:               "4 ACES",
        fourDeuces:             "4 DEUCES",
        fullHouse:              "FULL HOUSE",
        flush:                  "FLUSH",
        straight:               "STRAIGHT",
        threeKind:              "3 OF A KIND",
        twoPair:                "TWO PAIR",
        kingsOrBetter:          "KINGS OR BETTER",
        jacksOrBetter:          "JACKS OR BETTER",
    ]

    /// Pay table display name
    var name: String {
        HandValue.names[self] ?? "*** UNKNOWN ***"
    }
}
