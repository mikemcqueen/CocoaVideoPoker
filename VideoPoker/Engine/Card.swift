//
//  Card.swift
//  VideoPoker
//
//  Copyright (C) 2009 Mike McQueen. All rights reserved.
//

enum Suit: Int, CaseIterable {
    case clubs
    case diamonds
    case hearts
    case spades
}

/// Card numbers ("ranks"). A deuce is 2, an ace is 14 (or 1 when played low).
enum CardNumber {
    static let aceLow = 1
    static let deuce  = 2
    static let jack   = 11
    static let queen  = 12
    static let king   = 13
    static let ace    = 14
    static let joker  = 15
}

/// A card is a single byte: `(number - 2) * 4 + suit`, so 0-3 are the deuces,
/// 48-51 the aces, and 52 the joker.
struct Card: Hashable, Comparable {
    var value: UInt8

    static let undefined = Card(value: 99)

    init(value: UInt8) {
        self.value = value
    }

    init(number: Int, suit: Suit) {
        value = Card.makeValue(number: number, suit: suit)
    }

    static func makeValue(number: Int, suit: Suit) -> UInt8 {
        UInt8((number - 2) * Suit.allCases.count + suit.rawValue)
    }

    var number: Int {
        get { Int(value) / Suit.allCases.count + 2 }
        set { value = Card.makeValue(number: newValue, suit: suit) }
    }

    var suit: Suit {
        get { Suit(rawValue: Int(value) % Suit.allCases.count)! }
        set { value = Card.makeValue(number: number, suit: newValue) }
    }

    static func < (lhs: Card, rhs: Card) -> Bool {
        lhs.value < rhs.value
    }

    /// Packs up to 5 cards, 6 bits each, into a single value (stored in hand history).
    static func uniqueValue<C: Collection>(of cards: C) -> Int where C.Element == Card {
        precondition(cards.count <= 5, "uniqueValue only works on 5 card hands or less")
        return cards.reduce(0) { ($0 << 6) + Int($1.value) }
    }
}
