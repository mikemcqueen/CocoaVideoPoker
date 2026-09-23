//
//  SolverData.swift
//  VideoPoker
//
//  Copyright (C) 2010-2011 Mike McQueen. All rights reserved.
//

import Foundation

/// Results for one of the 32 ways to play (hold cards from) a hand.
struct SolverResults {
    /// Number of draws ending in each payline; index 0 is "no paying hand",
    /// index n is the pay schedule's payline n - 1.
    var payouts: [Int] = []
    /// Expected return, in coins for a 5 coin bet
    var ev: Double = 0.0

    var totalPayouts: Int {
        payouts.dropFirst().reduce(0, +)
    }
}

enum SolverDataError: Error {
    case truncated
    case badTag
    case mismatch(deckSize: Int, paylineCount: Int)
    case corrupt(String)
}

/// Precomputed counts of the paying hands that contain each 0-5 card
/// combination, from which the results of every way to play a hand can be
/// calculated.
///
/// File format (little endian): tags are 4 bytes ("SD" + section + version).
///
///     tag, deckSize (u32), paylineCount (u32)
///     paylineCount u32 counts for all hands
///     tag, deckSize * paylineCount u32 counts for 1 card
///     tag, 2 card map (u32 count), tag, 3 card map (u16), tag, 4 card map (u8)
///     tag, 4 bit payline index of every 5 card hand
///
/// A map is keyCount (u32), then per key: key (u16 for 2 cards, else u32,
/// unused), n (u8), then n * (payline (u8), count)
final class SolverData {
    let schedule: PaySchedule
    let paylineCount: Int
    let deckSize: Int
    let cardsPerHand: Int

    private var handResults: [Int] = []
    private var oneCardResults: [Int] = []
    private var twoCardResults = ResultsMap()
    private var threeCardResults = ResultsMap()
    private var fourCardResults = ResultsMap()
    private var fiveCardResults: [UInt8] = []

    private(set) var results = [SolverResults](repeating: SolverResults(), count: 32)
    /// Index into `results` of the best play, also the bits of which cards to hold
    private(set) var bestResultsIndex = 0

    var bestResults: SolverResults {
        results[bestResultsIndex]
    }

    init(schedule: PaySchedule, deckSize: Int, cardsPerHand: Int = 5, url: URL) throws {
        self.schedule = schedule
        self.deckSize = deckSize
        self.cardsPerHand = cardsPerHand
        self.paylineCount = schedule.paylineCount + 1 // +1 = "no match"

        var reader = Reader(data: try Data(contentsOf: url, options: .mappedIfSafe))

        try reader.readTag()
        let fileDeckSize = Int(try reader.read(UInt32.self))
        let filePaylineCount = Int(try reader.read(UInt32.self))
        guard fileDeckSize == deckSize, filePaylineCount == paylineCount else {
            throw SolverDataError.mismatch(deckSize: fileDeckSize, paylineCount: filePaylineCount)
        }

        handResults = try reader.readInts(UInt32.self, count: paylineCount)
        try reader.readTag()
        oneCardResults = try reader.readInts(UInt32.self, count: deckSize * paylineCount)
        try reader.readTag()
        twoCardResults = try ResultsMap(&reader, keySize: 2, countType: UInt32.self, paylineCount: paylineCount)
        try reader.readTag()
        threeCardResults = try ResultsMap(&reader, keySize: 4, countType: UInt16.self, paylineCount: paylineCount)
        try reader.readTag()
        fourCardResults = try ResultsMap(&reader, keySize: 4, countType: UInt8.self, paylineCount: paylineCount)
        try reader.readTag()
        fiveCardResults = try reader.readArray(UInt8.self, count: binomial(deckSize, cardsPerHand) / 2 + 1)
    }

    /// Calculates the results of every way to play `hand`, which must be sorted
    /// by descending card value. Returns the best expected value.
    @discardableResult
    func solve(_ hand: [Card]) -> Double {
        var bestEv = 0.0

        for row in 0..<32 {
            // Inclusion-exclusion over the held cards (row) and every superset
            // of them (col): draws where exactly the held cards remain
            var payouts = [Int](repeating: 0, count: paylineCount)
            for col in 0..<32 where col & row == row {
                let cards = hand.indices.filter { col & (1 << $0) != 0 }.map { hand[$0] }
                let sign = (col ^ row).nonzeroBitCount % 2 == 0 ? 1 : -1
                let subsetCounts = counts(containing: cards)
                for payline in 0..<paylineCount {
                    payouts[payline] += sign * subsetCounts[payline]
                }
            }

            var totalPayoutValue = 0
            var totalPayoutCount = 0
            for payline in 0..<paylineCount {
                totalPayoutCount += payouts[payline]
                if payline > 0 {
                    totalPayoutValue += schedule.payout(ofPayline: payline - 1, coinCount: 5) * payouts[payline]
                }
            }
            let ev = totalPayoutValue > 0 ? Double(totalPayoutValue) / Double(totalPayoutCount) : 0.0

            results[row] = SolverResults(payouts: payouts, ev: ev)
            if ev > bestEv {
                bestEv = ev
                bestResultsIndex = row
            }
        }
        return bestEv
    }

    /// Count of hands per payline containing `cards`
    private func counts(containing cards: [Card]) -> [Int] {
        switch cards.count {
        case 0:
            return handResults
        case 1:
            let start = Int(cards[0].value) * paylineCount
            return Array(oneCardResults[start..<start + paylineCount])
        case 2, 3, 4:
            let map = [twoCardResults, threeCardResults, fourCardResults][cards.count - 2]
            var counts = [Int](repeating: 0, count: paylineCount)
            for (payline, count) in map.entries(forHandIndex: Deck.handIndex(of: cards, deckSize: deckSize)) {
                counts[payline] = count
            }
            return counts
        case 5:
            var counts = [Int](repeating: 0, count: paylineCount)
            counts[nibble(Deck.handIndex(of: cards, deckSize: deckSize))] = 1
            return counts
        default:
            preconditionFailure("counts(containing:) bad card count")
        }
    }

    private func nibble(_ index: Int) -> Int {
        let byte = fiveCardResults[index / 2]
        return Int(index % 2 == 0 ? byte >> 4 : byte & 0x0f)
    }
}

/// Sparse per-hand payline counts for 2, 3 or 4 card combinations
private struct ResultsMap {
    private var offsets: [Int] = []   // by hand index, into paylines/counts
    private var lengths: [UInt8] = []
    private var paylines: [UInt8] = []
    private var counts: [Int] = []

    init() {}

    init<Count: FixedWidthInteger & UnsignedInteger>(
        _ reader: inout Reader, keySize: Int, countType: Count.Type, paylineCount: Int) throws
    {
        let keyCount = Int(try reader.read(UInt32.self))
        offsets = [Int](repeating: 0, count: keyCount)
        lengths = [UInt8](repeating: 0, count: keyCount)

        for keyIndex in 0..<keyCount {
            try reader.skip(keySize) // the key (card bits) isn't used
            let length = try reader.read(UInt8.self)
            // keys were written in ascending card bits order, the reverse of hand index order
            let slot = keyCount - keyIndex - 1
            offsets[slot] = paylines.count
            lengths[slot] = length
            for _ in 0..<length {
                let payline = try reader.read(UInt8.self)
                guard payline < paylineCount else {
                    throw SolverDataError.corrupt("payline \(payline) >= paylineCount \(paylineCount)")
                }
                paylines.append(payline)
                counts.append(Int(try reader.read(Count.self)))
            }
        }
    }

    func entries(forHandIndex handIndex: Int) -> [(payline: Int, count: Int)] {
        let start = offsets[handIndex]
        return (start..<start + Int(lengths[handIndex])).map { (Int(paylines[$0]), counts[$0]) }
    }
}

private struct Reader {
    let data: Data
    var position: Int

    init(data: Data) {
        self.data = data
        position = data.startIndex
    }

    mutating func skip(_ count: Int) throws {
        guard data.endIndex - position >= count else {
            throw SolverDataError.truncated
        }
        position += count
    }

    mutating func read<T: FixedWidthInteger>(_ type: T.Type) throws -> T {
        let size = MemoryLayout<T>.size
        guard data.endIndex - position >= size else {
            throw SolverDataError.truncated
        }
        var value: T = 0
        withUnsafeMutableBytes(of: &value) { buffer in
            buffer.copyBytes(from: data[position..<position + size])
        }
        position += size
        return T(littleEndian: value)
    }

    mutating func readArray<T: FixedWidthInteger>(_ type: T.Type, count: Int) throws -> [T] {
        let size = MemoryLayout<T>.size * count
        guard data.endIndex - position >= size else {
            throw SolverDataError.truncated
        }
        var array = [T](repeating: 0, count: count)
        array.withUnsafeMutableBytes { buffer in
            buffer.copyBytes(from: data[position..<position + size])
        }
        position += size
        return array.map { T(littleEndian: $0) }
    }

    mutating func readInts<T: FixedWidthInteger>(_ type: T.Type, count: Int) throws -> [Int] {
        try readArray(type, count: count).map { Int($0) }
    }

    mutating func readTag() throws {
        let start = position
        try skip(4)
        guard data[start] == UInt8(ascii: "S"), data[start + 1] == UInt8(ascii: "D") else {
            throw SolverDataError.badTag
        }
    }
}
