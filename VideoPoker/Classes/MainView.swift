//
//  MainView.swift
//  VideoPoker
//
//  Created by Mike McQueen on 8/1/09.
//

import UIKit

/// The Train tab: pay table, returns, cards, and the controls below them
final class MainView: UIView {
    var payScheduleView: UIView?
    var returnView: UIView?
    var cardTableView: UIView?

    @IBOutlet var dealButton: UIButton!
    @IBOutlet var holdBestButton: UIButton!
    @IBOutlet var betOneButton: UIButton!
    @IBOutlet var gameOverLabel: UILabel!
    @IBOutlet var handValueLabel: UILabel!
    @IBOutlet var payScheduleLabel: UILabel!
    @IBOutlet var betLabel: UILabel!
    @IBOutlet var winLabel: StrokeLabel!
    @IBOutlet var creditsLabel: StrokeLabel!

    /// Pay table height the controls below the card table were laid out for in the nib
    private static let nibPayScheduleHeight: CGFloat = 140.0
    /// Nib y positions of the controls below the card table
    private var controlBaseY: [CGFloat]?

    private var controlsBelowCards: [UIView] {
        [dealButton, holdBestButton, betOneButton, gameOverLabel, handValueLabel, betLabel, winLabel, creditsLabel]
    }

    override func layoutSubviews() {
        // payScheduleLabel is placed correctly automatically; stack the rest below it
        guard let payScheduleView = payScheduleView, let returnView = returnView, let cardTableView = cardTableView else {
            return
        }
        payScheduleView.frame = payScheduleView.bounds.offsetBy(dx: 0.0, dy: payScheduleLabel.frame.maxY + 1)
        returnView.frame = returnView.bounds.offsetBy(dx: 0.0, dy: payScheduleView.frame.maxY + 1)
        cardTableView.frame = cardTableView.bounds.offsetBy(dx: 0.0, dy: returnView.frame.maxY + 1)

        // Shift the controls below the card table by however much the pay table grew
        let controls = controlsBelowCards
        let baseY = controlBaseY ?? controls.map { $0.frame.origin.y }
        controlBaseY = baseY
        let delta = payScheduleView.bounds.height - MainView.nibPayScheduleHeight
        for (control, y) in zip(controls, baseY) {
            control.frame.origin.y = y + delta
        }
    }

    func setDealUI() {
        dealButton.setTitle("DEAL", for: .normal)
        gameOverLabel.isHidden = false
        holdBestButton.isHidden = true
        betOneButton.isHidden = false
    }

    func setDrawUI() {
        dealButton.setTitle("DRAW", for: .normal)
        gameOverLabel.isHidden = true
        holdBestButton.isHidden = false
        betOneButton.isHidden = true
        winLabel.isHidden = true
    }

    func showWin(_ payout: Int) {
        winLabel.text = "WIN \(payout)"
        winLabel.isHidden = false
    }

    func showCash(_ cash: Double) {
        creditsLabel.text = String(format: "CASH $%.2f", cash)
        creditsLabel.isHidden = false
    }
}
