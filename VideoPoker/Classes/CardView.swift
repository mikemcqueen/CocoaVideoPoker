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

    override func awakeFromNib() {
        super.awakeFromNib()
        NotificationCenter.default.addObserver(self, selector: #selector(graphicsStyleDidChange),
                                               name: GraphicsStyle.didChangeNotification, object: nil)
    }

    @objc private func graphicsStyleDidChange() {
        setNeedsDisplay()
    }

    /// Modern style shows HELD above the card, as casino machines do
    private lazy var heldLabel: UILabel = {
        let label = UILabel()
        label.text = "HELD"
        label.font = UIFont(name: "ArialRoundedMTBold", size: 10) ?? .boldSystemFont(ofSize: 10)
        label.textColor = UIColor(red: 1.0, green: 0.93, blue: 0.0, alpha: 1.0)
        label.shadowColor = UIColor(white: 0.0, alpha: 0.8)
        label.shadowOffset = CGSize(width: 0.6, height: 0.6)
        label.textAlignment = .center
        label.isHidden = true
        return label
    }()

    private func updateHeldLabel(held: Bool) {
        guard let superview = superview else {
            return
        }
        if heldLabel.superview !== superview {
            superview.addSubview(heldLabel)
        }
        heldLabel.frame = CGRect(x: frame.minX, y: frame.minY - 12, width: frame.width, height: 12)
        heldLabel.isHidden = !held || GraphicsStyle.current != .modern
    }

    override func draw(_ rect: CGRect) {
        guard let context = UIGraphicsGetCurrentContext() else {
            return
        }
        let card = self.card
        var held = false
        var image: CGImage?
        if card == .undefined {
            image = cardTable?.backImage(at: index)
        } else {
            held = cardTable?.isHeld(card) ?? false
            if GraphicsStyle.current == .modern {
                // the rounded card corners show the table behind it
                context.setFillColor((superview?.backgroundColor ?? .clear).cgColor)
                context.fill(bounds)
                ModernCardRenderer.draw(card, in: bounds)
            } else {
                image = cardTable?.image(for: card)
            }
        }

        updateHeldLabel(held: held)

        context.translateBy(x: 0.0, y: rect.size.height)
        context.scaleBy(x: 1.0, y: -1.0)
        if let image = image {
            context.draw(image, in: rect)
        }
        if held && GraphicsStyle.current == .classic, let heldImage = CardImages.held {
            let x = CGFloat(Int((rect.size.width - CGFloat(heldImage.width)) / 2))
            let heldRect = CGRect(x: x, y: 10,
                                  width: x + CGFloat(heldImage.width),
                                  height: 10 + CGFloat(heldImage.height))
            context.draw(heldImage, in: heldRect)
        }
    }
}
