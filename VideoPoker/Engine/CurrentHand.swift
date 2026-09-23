//
//  CurrentHand.swift
//  VideoPoker
//
//  Created by Mike McQueen on 7/16/10.
//

/// The 5 cards in play, which of them are held, and the best cards to hold.
final class CurrentHand {
    private(set) var cards: [Card] = []
    private(set) var bestCards: [Card] = []
    var heldCards: [Card] = []

    func card(at index: Int) -> Card {
        cards[index]
    }

    func setCard(_ card: Card, at index: Int) {
        cards[index] = card
    }

    func setCard(_ card: Card, at index: Int, solver: SolverData) {
        setCard(card, at: index)
        solve(solver)
    }

    func setCards(_ cards: [Card], solver: SolverData) {
        self.cards = cards
        solve(solver)
    }

    /// Solving a hand with duplicate cards (possible in the Solve tab) would
    /// look up solver data out of range, so invalid hands have no best cards
    private func solve(_ solver: SolverData) {
        guard isValid else {
            bestCards = []
            return
        }
        let sortedCards = cards.sorted(by: >)
        solver.solve(sortedCards)

        let bestCardBits = solver.bestResultsIndex
        bestCards = sortedCards.indices.filter { bestCardBits & (1 << $0) != 0 }.map { sortedCards[$0] }
    }

    func isBestCard(at index: Int) -> Bool {
        bestCards.contains(card(at: index))
    }

    func clearHeldCards() {
        heldCards.removeAll()
    }

    func isHeld(_ card: Card) -> Bool {
        heldCards.contains(card)
    }

    func toggleHeld(_ card: Card) {
        if isHeld(card) {
            heldCards.removeAll { $0 == card }
        } else {
            heldCards.append(card)
        }
    }

    /// Solver results index for holding `cards`: bit n is set for the nth card
    /// of the hand sorted by descending value.
    func cardBits(of cards: [Card]) -> Int {
        let sortedCards = self.cards.sorted(by: >)
        return cards.reduce(0) { bits, card in
            guard let index = sortedCards.firstIndex(of: card) else {
                preconditionFailure("held card not in hand")
            }
            return bits | (1 << index)
        }
    }

    func reset() {
        cards = [Card](repeating: .undefined, count: 5)
        clearHeldCards()
    }

    /// True if there are cards and no duplicates
    var isValid: Bool {
        !cards.isEmpty && Set(cards).count == cards.count
    }
}
