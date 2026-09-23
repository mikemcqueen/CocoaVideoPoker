//
//  ReturnViewController.swift
//  VideoPoker
//
//  Created by Mike McQueen on 8/10/10.
//

import UIKit

protocol ReturnViewDelegate: AnyObject {
    func returnViewDetailButtonWasPressed()
}

/// Shows the expected return of your held cards and of the best cards to hold
final class ReturnViewController: UIViewController {
    // Connected in the nibs; must conform to ReturnViewDelegate
    @IBOutlet weak var delegate: AnyObject?

    @IBOutlet var yourEvLabel: UILabel!
    @IBOutlet var bestEvLabel: UILabel!
    @IBOutlet var invalidHandLabel: UILabel!
    @IBOutlet var detailButton: UIButton!

    override func viewDidLoad() {
        super.viewDidLoad()
        detailButton.accessibilityIdentifier = "handDetails"
    }

    /// ev is in coins for a 5 coin bet
    func setYourEv(_ ev: Double) {
        setLabel(yourEvLabel, returnPercent: 100.0 * (ev / 5.0))
    }

    func setBestEv(_ ev: Double) {
        setLabel(bestEvLabel, returnPercent: 100.0 * (ev / 5.0))
    }

    private func setLabel(_ label: UILabel, returnPercent: Double) {
        label.text = String(format: "%.2f%%", returnPercent)
    }

    /// Hides the returns and shows "not a valid hand" instead
    func setInvalid(_ invalid: Bool) {
        if invalid == invalidHandLabel.isHidden {
            for child in view.subviews {
                child.isHidden = invalid
                child.setNeedsDisplay()
            }
            invalidHandLabel.isHidden = !invalid
        }
    }

    @IBAction func pressDetailButton(_ sender: UIButton) {
        (delegate as? ReturnViewDelegate)?.returnViewDetailButtonWasPressed()
    }
}
