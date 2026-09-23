//
//  Deck.swift
//  VideoPoker
//
//  Copyright (C) 2009 Mike McQueen. All rights reserved.
//

struct Deck {
    private(set) var cards: [Card]
    private var topCard = 0

    init(size: Int) {
        cards = (0..<size).map { Card(value: UInt8($0)) }
    }

    mutating func shuffle() {
        cards.shuffle()
        topCard = 0
    }

    mutating func deal() -> Card {
        defer { topCard += 1 }
        return cards[topCard]
    }

    /// Index of a hand (cards sorted by descending value) among all
    /// `C(deckSize, cards.count)` hands. This is the key into the solver data.
    static func handIndex<C: RandomAccessCollection>(of cards: C, deckSize: Int) -> Int
        where C.Element == Card, C.Index == Int
    {
        let cardCount = cards.count
        var handIndex = 0
        var highestCardForIndex = deckSize - 1
        for (offset, card) in cards.enumerated() {
            let cardValue = Int(card.value)
            let remainingCardsInHand = cardCount - offset - 1
            if remainingCardsInHand > 0 {
                var highCardValue = highestCardForIndex
                while highCardValue > cardValue {
                    if remainingCardsInHand > 1 {
                        handIndex += binomial(highCardValue, remainingCardsInHand)
                    } else {
                        handIndex += highCardValue
                    }
                    highCardValue -= 1
                }
            } else {
                handIndex += highestCardForIndex - cardValue
            }
            highestCardForIndex = cardValue - 1
        }
        return handIndex
    }
}

/// Number of ways to choose `k` items from `n`.
func binomial(_ n: Int, _ k: Int) -> Int {
    guard k >= 0, k <= n else { return 0 }
    var result = 1
    for i in 0..<min(k, n - k) {
        result = result * (n - i) / (i + 1)
    }
    return result
}
