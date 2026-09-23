//
//  HandDetailsViewController.swift
//  VideoPoker
//
//  Created by Mike McQueen on 7/19/10.
//

import UIKit

/// How often each paying hand is drawn, and its return, holding your cards or the best cards
final class HandDetailsViewController: UIViewController {
    var game: GameModel!

    @IBOutlet var cardTable: CardTableViewController!
    @IBOutlet var detailTableView: UITableView!
    // Loaded from HandDetailsTableCell.xib as needed
    @IBOutlet var tableCell: UITableViewCell?
    @IBOutlet var tableHeader: UIView?

    private let segments = UISegmentedControl(items: ["Your", "Best", "Custom"])

    override func viewDidLoad() {
        super.viewDidLoad()

        hidesBottomBarWhenPushed = true
        view.addSubview(cardTable.view)

        segments.addTarget(self, action: #selector(selectHand), for: .valueChanged)
        segments.selectedSegmentIndex = 0
        navigationItem.titleView = segments

        detailTableView.rowHeight = 25.0

        ScaleToFitView.install(in: view, designSize: CGSize(width: 320.0, height: 416.0))
    }

    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        detailTableView.reloadData()
    }

    @objc private func selectHand() {
        for cardView in cardTable.cardViews {
            cardView.isEnabled = false
        }
        cardTable.redrawCards()
        detailTableView.reloadData()
        detailTableView.setNeedsDisplay()
    }

    /// The cards held in the selected hand
    private var heldCards: [Card] {
        segments.selectedSegmentIndex == 1 ? game.hand.bestCards : game.hand.heldCards
    }

    private var heldResults: SolverResults {
        game.solver.results[game.hand.cardBits(of: heldCards)]
    }

    private func loadCellNib() {
        Bundle.main.loadNibNamed("HandDetailsTableCell", owner: self, options: nil)
    }
}

extension HandDetailsViewController: UITableViewDataSource, UITableViewDelegate {
    func numberOfSections(in tableView: UITableView) -> Int {
        1
    }

    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        game.schedule.paylineCount
    }

    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        var cell = tableView.dequeueReusableCell(withIdentifier: "HandDetailsTableCell")
        if cell == nil {
            loadCellNib()
            cell = tableCell
            tableCell = nil
        }
        let payline = indexPath.row

        let nameLabel = cell?.viewWithTag(1) as? UILabel
        nameLabel?.text = game.schedule.handValue(ofPayline: payline).name

        // results index 0 is "no paying hand"
        let payoutCount = heldResults.payouts[payline + 1]
        let countLabel = cell?.viewWithTag(2) as? UILabel
        countLabel?.text = "\(payoutCount)"

        let coinCount = 5
        let remainingCards = 5 - heldCards.count
        let combinations = remainingCards > 0 ? binomial(47, remainingCards) : 1
        let payout = game.schedule.payout(ofPayline: payline, coinCount: coinCount)
        var expectedReturn = 100.0 * Float(UInt32(truncatingIfNeeded: payout * payoutCount)) / Float(UInt32(combinations))
        expectedReturn /= Float(coinCount)
        let returnLabel = cell?.viewWithTag(3) as? UILabel
        returnLabel?.text = String(format: "%6.2f%%", expectedReturn)

        return cell!
    }

    func tableView(_ tableView: UITableView, heightForHeaderInSection section: Int) -> CGFloat {
        50.0
    }

    func tableView(_ tableView: UITableView, viewForHeaderInSection section: Int) -> UIView? {
        loadCellNib()
        let header = tableHeader
        tableHeader = nil
        tableCell = nil

        let results = heldResults
        (header?.viewWithTag(2) as? UILabel)?.text = "\(results.totalPayouts)"
        // TODO: coin count
        (header?.viewWithTag(3) as? UILabel)?.text = String(format: "%.2f%%", results.ev / 5.0)
        return header
    }
}

extension HandDetailsViewController: CardTableDataSource, CardTableDelegate {
    func cardViewWasPressed(_ cardView: CardView) {
    }

    func card(at index: Int) -> Card {
        game.hand.card(at: index)
    }

    func isHeld(_ card: Card) -> Bool {
        heldCards.contains(card)
    }
}
