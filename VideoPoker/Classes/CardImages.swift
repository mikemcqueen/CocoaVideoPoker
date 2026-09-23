//
//  CardImages.swift
//  VideoPoker
//
//  Created by Mike McQueen on 7/27/09.
//  Copyright 2009 Mike McQueen. All rights reserved.
//

import UIKit

/// Card faces cut from classic-playing-cards.png, plus the card back and HELD overlay
enum CardImages {
    static func load(_ filename: String) -> CGImage? {
        let url = Bundle.main.resourceURL!.appendingPathComponent(filename)
        return UIImage(contentsOfFile: url.path)?.cgImage
    }

    private static let faces: [CGImage?] = {
        guard let sheet = load("classic-playing-cards.png") else {
            return []
        }
        // Sheet rows are clubs, spades, hearts, diamonds; columns are ace (low) through king
        let rowForSuit: [Suit: Int] = [.clubs: 0, .spades: 1, .hearts: 2, .diamonds: 3]
        let size = CGSize(width: 71, height: 96)
        return (0..<52).map { value in
            let card = Card(value: UInt8(value))
            let column = card.number == CardNumber.ace ? CardNumber.aceLow : card.number
            let origin = CGPoint(x: 1 + (size.width + 2) * CGFloat(column - 1),
                                 y: 1 + (size.height + 2) * CGFloat(rowForSuit[card.suit]!))
            return sheet.cropping(to: CGRect(origin: origin, size: size))
        }
    }()

    // TODO: JOKER
    static func face(for card: Card) -> CGImage? {
        Int(card.value) < faces.count ? faces[Int(card.value)] : nil
    }

    static let back = load("blueback.png")
    static let held = load("held.png")
}
