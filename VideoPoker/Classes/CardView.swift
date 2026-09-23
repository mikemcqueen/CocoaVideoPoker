//
//  CardView.swift
//  VideoPoker
//

import UIKit

/// One card of a CardTableView; its tag is its index in the hand.
final class CardView: UIControl {
    @IBOutlet weak var cardTable: CardTableViewController?

    var index: Int {
        tag
    }

    var card: Card {
        cardTable?.card(at: index) ?? Card(value: 0)
    }

    override func draw(_ rect: CGRect) {
        guard let context = UIGraphicsGetCurrentContext() else {
            return
        }
        let card = self.card
        var held = false
        let image: CGImage?
        if card == .undefined {
            image = cardTable?.backImage(at: index)
        } else {
            image = cardTable?.image(for: card)
            held = cardTable?.isHeld(card) ?? false
        }

        context.translateBy(x: 0.0, y: rect.size.height)
        context.scaleBy(x: 1.0, y: -1.0)
        if let image = image {
            context.draw(image, in: rect)
        }
        if held, let heldImage = CardImages.held {
            let x = CGFloat(Int((rect.size.width - CGFloat(heldImage.width)) / 2))
            let heldRect = CGRect(x: x, y: 10,
                                  width: x + CGFloat(heldImage.width),
                                  height: 10 + CGFloat(heldImage.height))
            context.draw(heldImage, in: heldRect)
        }
    }
}
