//
//  EngineTests.swift
//  VideoPokerTests
//

import XCTest
@testable import VideoPoker

final class EngineTests: XCTestCase {
    private let catalog = PayScheduleCatalog.shared

    private func cards(_ text: String) -> [Card] {
        // e.g. "Ah Kh Qh Jh Th"
        text.split(separator: " ").map { token in
            let numbers: [Character: Int] = ["T": 10, "J": 11, "Q": 12, "K": 13, "A": 14]
            let suits: [Character: Suit] = ["c": .clubs, "d": .diamonds, "h": .hearts, "s": .spades]
            let number = numbers[token.first!] ?? Int(String(token.first!))!
            return Card(number: number, suit: suits[token.last!]!)
        }
    }

    private var jacks: PaySchedule {
        catalog.schedule(.jacksOrBetter, estimatedReturn: 99.54)!
    }

    private func loadSolver(_ schedule: PaySchedule) throws -> SolverData {
        try SolverData(schedule: schedule, deckSize: 52, url: GameModel.solverURL(for: schedule.gameID))
    }

    func testCardEncoding() {
        XCTAssertEqual(Card(number: 2, suit: .clubs).value, 0)
        XCTAssertEqual(Card(number: CardNumber.ace, suit: .spades).value, 51)
        var card = Card(number: 10, suit: .hearts)
        card.number = CardNumber.queen
        card.suit = .diamonds
        XCTAssertEqual(card, Card(number: CardNumber.queen, suit: .diamonds))
    }

    func testCatalog() {
        XCTAssertEqual(catalog.games.map(\.name), [
            "All American", "BonusPoker", "Bonus Poker Deluxe", "Deuces Wild", "Double Bonus Poker",
            "Double Double Bonus", "Double Deuces", "Jacks or Better", "Jokers Wild", "Loose Deuces",
            "Super Aces", "Triple Double Bonus", "White Hot Aces",
        ])
        let game = catalog.game(.jacksOrBetter)!
        XCTAssertEqual(game.schedules.map(\.estimatedReturn), [100.00, 99.54, 98.44, 98.39, 97.29, 97.24, 96.14, 94.99])
        XCTAssertEqual(catalog.game(.jokersWild)!.deckSize, 53)

        // 9/6 Jacks or Better
        XCTAssertEqual(jacks.paylineCount, 9)
        XCTAssertEqual((0..<9).map { jacks.payout(ofPayline: $0, coinCount: 1) }, [250, 50, 25, 9, 6, 4, 3, 2, 1])
        XCTAssertEqual(jacks.payout(for: .royalFlush, coinCount: 5), 4000)
        XCTAssertEqual(jacks.payout(for: .royalFlush, coinCount: 4), 1000)
    }

    func testJacksOrBetterHands() {
        let expected: [(String, HandValue)] = [
            ("Ah Kh Qh Jh Th", .royalFlush),
            ("9s Ks Qs Js Ts", .straightFlush),
            ("5d 4d 3d 2d Ad", .straightFlush),
            ("7c 7d 7h 7s 2c", .fourOfAKind),
            ("7c 7d 7h 2s 2c", .fullHouse),
            ("2h 9h Jh 4h 6h", .flush),
            ("5d 4c 3d 2d Ad", .straight),
            ("Tc Jd Qh Ks Ac", .straight),
            ("9c 9d 9h Ks 2c", .threeKind),
            ("9c 9d 3h 3s 2c", .twoPair),
            ("Jc Jd 3h 8s 2c", .jacksOrBetter),
            ("Ac Ad 3h 8s 2c", .jacksOrBetter),
            ("Tc Td 3h 8s 2c", .none),
            ("Ac Kd 3h 8s 2c", .none),
        ]
        for (hand, value) in expected {
            XCTAssertEqual(jacks.payingValue(of: cards(hand)), value, hand)
        }
    }

    func testWildHands() {
        let deuces = catalog.game(.deucesWild)!.schedules[0]
        XCTAssertEqual(deuces.payingValue(of: cards("2c 2d 2h 2s 9c")), .fourDeuces)
        XCTAssertEqual(deuces.payingValue(of: cards("Ah Kh Qh Jh Th")), .royalFlushNoDeuces)
        XCTAssertEqual(deuces.payingValue(of: cards("Ah Kh Qh Jh 2c")), .royalFlushWithDeuce)
        XCTAssertEqual(deuces.payingValue(of: cards("9h 9c 9d 2h 2c")), .fiveOfAKind)
        XCTAssertEqual(deuces.payingValue(of: cards("9h 9c 5d 4h 2c")), .threeKind)
        XCTAssertEqual(deuces.payingValue(of: cards("9h 8c 5d 4h 2c")), .none)
    }

    func testHandIndexIsDenseAndOrdered() {
        // Cards sorted descending map onto 0..<C(n, k) in order
        var expected = 0
        for a in (0..<8).reversed() {
            for b in (0..<a).reversed() {
                for c in (0..<b).reversed() {
                    let hand = [a, b, c].map { Card(value: UInt8($0)) }
                    XCTAssertEqual(Deck.handIndex(of: hand, deckSize: 8), expected)
                    expected += 1
                }
            }
        }
        XCTAssertEqual(expected, binomial(8, 3))
    }

    func testSolverDealtRoyalFlush() throws {
        let solver = try loadSolver(jacks)
        solver.solve(cards("Ah Kh Qh Jh Th").sorted(by: >))
        XCTAssertEqual(solver.bestResultsIndex, 31)
        XCTAssertEqual(solver.bestResults.ev, 4000.0)
    }

    /// Checks the solver's results for every way to play some hands against
    /// dealing out every possible draw
    func testSolverMatchesBruteForce() throws {
        let solver = try loadSolver(jacks)
        for text in ["Ah Kh 7c 7d 2s", "Jc Tc 9c 8h 3d", "5s 5d 5h Qc Qs"] {
            let hand = cards(text).sorted(by: >)
            solver.solve(hand)
            let deck = (0..<52).map { Card(value: UInt8($0)) }.filter { !hand.contains($0) }

            for bits in 0..<32 {
                let held = hand.indices.filter { bits & (1 << $0) != 0 }.map { hand[$0] }
                var counts = [Int](repeating: 0, count: jacks.paylineCount + 1)
                forEachCombination(of: deck, count: 5 - held.count) { drawn in
                    let value = jacks.payingValue(of: held + drawn)
                    counts[value == .none ? 0 : jacks.paylineIndex(of: value) + 1] += 1
                }
                XCTAssertEqual(solver.results[bits].payouts, counts, "\(text) hold bits \(bits)")
            }
        }
    }

    private func forEachCombination(of cards: [Card], count: Int, _ body: ([Card]) -> Void) {
        var chosen: [Card] = []
        func choose(from start: Int) {
            if chosen.count == count {
                body(chosen)
                return
            }
            guard start < cards.count else { return }
            for index in start...(cards.count - (count - chosen.count)) {
                chosen.append(cards[index])
                choose(from: index + 1)
                chosen.removeLast()
            }
        }
        choose(from: 0)
    }

    func testCurrentHand() throws {
        let solver = try loadSolver(jacks)
        let hand = CurrentHand()
        hand.reset()
        XCTAssertFalse(hand.isValid)

        hand.setCards(cards("2c Ah 7d Kh 9s"), solver: solver)
        XCTAssertTrue(hand.isValid)
        // sorted descending: Ah Kh 9s 7d 2c
        XCTAssertEqual(hand.cardBits(of: cards("Ah Kh")), 0b00011)
        XCTAssertEqual(hand.cardBits(of: cards("2c")), 0b10000)

        hand.toggleHeld(hand.card(at: 1))
        XCTAssertTrue(hand.isHeld(cards("Ah")[0]))
        hand.toggleHeld(hand.card(at: 1))
        XCTAssertTrue(hand.heldCards.isEmpty)

        // duplicate cards aren't a valid hand, and aren't solved
        hand.setCard(cards("Ah")[0], at: 0, solver: solver)
        XCTAssertFalse(hand.isValid)
        XCTAssertTrue(hand.bestCards.isEmpty)
    }
}
