//
//  MainViewController.swift
//  VideoPoker
//
//  Created by Mike McQueen on 8/1/09.
//

import UIKit

/// The Train tab: play hands, with the returns of your hold and the best hold
final class MainViewController: UIViewController, GameModelContainer {
    @IBOutlet var game: GameModel!
    @IBOutlet var mainView: MainView!
    @IBOutlet var payScheduleController: PayScheduleViewController!
    @IBOutlet var cardTable: CardTableViewController!
    @IBOutlet var returnViewController: ReturnViewController!

    private let hand = CurrentHand()
    /// True between deal and draw
    private var drawNext = false

    private static let navy = UIColor(red: 0.0, green: 0.0, blue: 128.0 / 255.0, alpha: 1.0)

    override func viewDidLoad() {
        super.viewDidLoad()

        hand.reset()

        if let navBar = navigationController?.navigationBar {
            navBar.tintColor = MainViewController.navy
            navBar.barStyle = .default
            navBar.isTranslucent = false
        }

        navigationItem.rightBarButtonItem = UIBarButtonItem(title: "Choose Game", style: .done,
                                                            target: self, action: #selector(chooseGame))

        let infoButton = UIButton(type: .infoLight)
        infoButton.addTarget(self, action: #selector(showInfo), for: .touchUpInside)
        navigationItem.leftBarButtonItem = UIBarButtonItem(customView: infoButton)

        mainView.addSubview(payScheduleController.view)
        mainView.payScheduleView = payScheduleController.view
        payScheduleController.payScheduleDidChange()

        mainView.addSubview(returnViewController.view)
        mainView.returnView = returnViewController.view

        mainView.addSubview(cardTable.view)
        mainView.cardTableView = cardTable.view

        // TODO: cardTable.disableAll()
        for cardView in cardTable.cardViews {
            cardView.isEnabled = false
        }

        mainView.handValueLabel.text = nil
        mainView.gameOverLabel.isHidden = false

        mainView.winLabel.strokeWidth = 2
        mainView.winLabel.strokeColor = .yellow
        mainView.creditsLabel.strokeWidth = 2
        mainView.creditsLabel.strokeColor = .yellow

        ScaleToFitView.install(in: view, designSize: CGSize(width: 320.0, height: 367.0))
    }

    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)

        // The Solve tab shares the game model with its own hand.
        // TODO: if a valid hand was in play, returns should be recalculated;
        // if not, should they reset to zero?
        game.hand = hand
        if hand.isValid {
            game.setCards(hand.cards)
        }
    }

    // MARK: - Actions

    @IBAction func betOne() {
        game.betOne()
        updateBet()
    }

    private func updateBet() {
        payScheduleController.view.setNeedsDisplay()
        mainView.betLabel.text = "BET \(game.bet)"
    }

    @IBAction func dealOrDraw() {
        if !drawNext {
            guard game.deal() else {
                offerToAddFunds()
                return
            }
            // TODO: cardTable.invalidateAllCardViews()
            for cardView in cardTable.cardViews {
                cardView.isEnabled = true
                cardView.setNeedsDisplay()
            }

            returnViewController.setYourEv(game.ev(forCardBits: 0))
            returnViewController.setBestEv(game.bestEv)

            mainView.setDrawUI()

            // can only view hand details after deal; can't change game after deal
            returnViewController.detailButton.isEnabled = true
            navigationItem.rightBarButtonItem?.isEnabled = false

            mainView.showCash(game.cash)
        } else {
            let payout = game.draw()

            for cardView in cardTable.cardViews {
                cardView.setNeedsDisplay()
                cardView.isEnabled = false
            }

            mainView.setDealUI()

            returnViewController.detailButton.isEnabled = false
            navigationItem.rightBarButtonItem?.isEnabled = true

            if payout > 0 {
                mainView.showWin(payout)
                mainView.showCash(game.cash)
            }
        }

        if game.handValueChanged {
            mainView.payScheduleView?.setNeedsDisplay()
        }

        drawNext.toggle()
    }

    @IBAction func holdBestCards() {
        game.hand.heldCards = game.hand.bestCards
        cardTable.redrawCards()
        updateYourEv()
    }

    private func updateYourEv() {
        returnViewController.setYourEv(game.updateEv())
    }

    @objc private func showInfo() {
        let controller = OptionsViewController(nibName: "OptionsView", bundle: nil)
        controller.delegate = self
        let navController = UINavigationController(rootViewController: controller)
        navController.modalTransitionStyle = .flipHorizontal
        present(navController, animated: true)
    }

    @objc private func chooseGame() {
        let chooseController = ChooseGameViewController(schedule: game.schedule)
        chooseController.delegate = self
        chooseController.title = "Choose Game"

        let navController = UINavigationController(rootViewController: chooseController)
        navController.navigationBar.tintColor = MainViewController.navy

        chooseController.navigationItem.rightBarButtonItem =
            UIBarButtonItem(title: "Done", style: .done, target: chooseController, action: #selector(ChooseGameViewController.done))
        chooseController.navigationItem.leftBarButtonItem =
            UIBarButtonItem(title: "Cancel", style: .done, target: chooseController, action: #selector(ChooseGameViewController.cancel))

        present(navController, animated: true)
    }

    private func offerToAddFunds() {
        let fundsToAdd: Int32 = 10000 // cents
        let alert = UIAlertController(title: "Out of Cash",
                                      message: "You don't have enough cash for this bet. Add $\(fundsToAdd / 100)?",
                                      preferredStyle: .alert)
        alert.addAction(UIAlertAction(title: "Cancel", style: .cancel))
        alert.addAction(UIAlertAction(title: "Add Funds", style: .default) { _ in
            self.game.addFunds(fundsToAdd)
            self.mainView.showCash(self.game.cash)
        })
        present(alert, animated: true)
    }

    private func showHandDetails() {
        let controller = HandDetailsViewController(nibName: "HandDetailsView", bundle: nil)
        controller.game = game
        controller.hidesBottomBarWhenPushed = true
        navigationController?.pushViewController(controller, animated: true)
    }
}

extension MainViewController: OptionsViewControllerDelegate {
    func optionsViewControllerDidFinish(_ controller: OptionsViewController) {
        dismiss(animated: true)
    }
}

extension MainViewController: ChooseGameViewControllerDelegate {
    func chooseGameDone(_ schedule: PaySchedule) {
        if !GameModel.hasSolverData(for: schedule.gameID) {
            let alert = UIAlertController(title: "Game Unavailable",
                                          message: "Solver data for \(schedule.game.name) isn't included in this build.",
                                          preferredStyle: .alert)
            alert.addAction(UIAlertAction(title: "OK", style: .default))
            presentedViewController?.present(alert, animated: true)
            return
        }

        if game.switchTo(schedule) {
            payScheduleController.payScheduleDidChange()
        }
        dismiss(animated: true)
    }

    func chooseGameCancel() {
        dismiss(animated: true)
    }
}

extension MainViewController: CardTableDataSource, CardTableDelegate {
    func cardViewWasPressed(_ cardView: CardView) {
        game.hand.toggleHeld(cardView.card)
        cardView.setNeedsDisplay()
        updateYourEv()
    }

    func card(at index: Int) -> Card {
        game.hand.card(at: index)
    }

    func isHeld(_ card: Card) -> Bool {
        game.hand.isHeld(card)
    }
}

extension MainViewController: ReturnViewDelegate {
    func returnViewDetailButtonWasPressed() {
        showHandDetails()
    }
}
