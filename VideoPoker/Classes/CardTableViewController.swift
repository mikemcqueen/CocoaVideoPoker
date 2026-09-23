//
//  CardTableViewController.swift
//  VideoPoker
//
//  Created by Mike McQueen on 7/27/10.
//

import UIKit

protocol CardTableDataSource: AnyObject {
    func card(at index: Int) -> Card
    func isHeld(_ card: Card) -> Bool
    func image(for card: Card) -> CGImage?
    func backImage(at index: Int) -> CGImage?
}

protocol CardTableDelegate: AnyObject {
    func cardViewWasPressed(_ cardView: CardView)
}

extension CardTableDataSource {
    func image(for card: Card) -> CGImage? {
        CardImages.face(for: card)
    }

    func backImage(at index: Int) -> CGImage? {
        CardImages.back
    }
}

final class CardTableViewController: UIViewController {
    // Connected in the nibs; must conform to CardTableDataSource / CardTableDelegate
    // (IBOutlets can't be typed as Swift protocols)
    @IBOutlet weak var dataSource: AnyObject?
    @IBOutlet weak var delegate: AnyObject?

    var cardTableView: CardTableView {
        view as! CardTableView
    }

    var cardViews: [CardView] {
        cardTableView.cardViews
    }

    @IBAction func tapCardView(_ sender: CardView) {
        if sender.isEnabled {
            (delegate as? CardTableDelegate)?.cardViewWasPressed(sender)
        } else {
            NSLog("tapCardView: disabled")
        }
    }

    private var source: CardTableDataSource? {
        dataSource as? CardTableDataSource
    }

    func isHeld(_ card: Card) -> Bool {
        source?.isHeld(card) ?? false
    }

    func card(at index: Int) -> Card {
        source?.card(at: index) ?? Card(value: 0)
    }

    func image(for card: Card) -> CGImage? {
        source?.image(for: card)
    }

    func backImage(at index: Int) -> CGImage? {
        source?.backImage(at: index)
    }

    func redrawCards() {
        cardTableView.redrawSubviews()
    }
}
