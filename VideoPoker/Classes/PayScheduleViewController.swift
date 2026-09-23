//
//  PayScheduleViewController.swift
//  VideoPoker
//
//  Created by Mike McQueen on 8/26/10.
//  Copyright 2010 Mike McQueen. All rights reserved.
//

import UIKit

/// Shows either the game's current pay schedule (embedded in the Train tab)
/// or a fixed pay schedule (a page of Choose Game)
final class PayScheduleViewController: UIViewController, PayScheduleViewDelegate {
    @IBOutlet weak var gameContainer: GameModelContainer?
    @IBOutlet var payScheduleView: PayScheduleView!

    /// Fixed schedule when shown in Choose Game; nil to follow the game
    private var fixedSchedule: PaySchedule?

    init(paySchedule: PaySchedule) {
        fixedSchedule = paySchedule
        super.init(nibName: "PayScheduleView", bundle: nil)
    }

    required init?(coder: NSCoder) {
        super.init(coder: coder)
    }

    var game: GameModel? {
        gameContainer?.game
    }

    func payScheduleDidChange() {
        guard let payScheduleView = payScheduleView else {
            return  // not loaded yet
        }
        payScheduleView.frame.size.height = PayScheduleView.height(for: paySchedule)
        payScheduleView.setNeedsDisplay()
        payScheduleView.superview?.setNeedsLayout()
    }

    // PayScheduleViewDelegate

    var paySchedule: PaySchedule {
        fixedSchedule ?? game!.schedule
    }

    var bet: Int {
        // No bet highlight in Choose Game
        fixedSchedule == nil ? game?.bet ?? 0 : 0
    }
}
