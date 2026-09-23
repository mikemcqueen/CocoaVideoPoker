//
//  CardTableView.swift
//  VideoPoker
//

import UIKit

/// The row of 5 card views
final class CardTableView: UIView {
    @IBOutlet var cardViews: [CardView]!

    override func awakeFromNib() {
        super.awakeFromNib()
        for cardView in cardViews {
            cardView.isAccessibilityElement = true
            cardView.accessibilityIdentifier = "card\(cardView.index)"
        }
    }

    func cardView(at index: Int) -> CardView {
        cardViews.first { $0.index == index }!
    }

    func redrawSubviews() {
        subviews.forEach { $0.setNeedsDisplay() }
    }
}
