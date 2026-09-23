//
//  ChooseGameViewController.swift
//  VideoPoker
//
//  Created by Mike McQueen on 8/24/10.
//  Copyright 2010 Mike McQueen. All rights reserved.
//

import UIKit

protocol ChooseGameViewControllerDelegate: AnyObject {
    func chooseGameDone(_ schedule: PaySchedule)
    func chooseGameCancel()
}

/// Pick a game, then page through its pay schedules
final class ChooseGameViewController: UIViewController {
    weak var delegate: ChooseGameViewControllerDelegate?

    @IBOutlet var label: UILabel!
    @IBOutlet var scrollView: UIScrollView!
    @IBOutlet var pageControl: UIPageControl!
    @IBOutlet var gamePicker: UIPickerView!

    private let initialSchedule: PaySchedule
    private var game: Game?
    /// Pay schedule pages per game, created as needed
    private var pages: [GameID: [PayScheduleViewController?]] = [:]
    /// Set while scrolling to a page picked with the page control
    private var pageControlUsed = false

    private let catalog = PayScheduleCatalog.shared

    init(schedule: PaySchedule) {
        initialSchedule = schedule
        super.init(nibName: "ChooseGameView", bundle: nil)
        for game in catalog.games {
            pages[game.id] = Array(repeating: nil, count: game.schedules.count)
        }
    }

    required init?(coder: NSCoder) {
        fatalError("init(coder:) is not supported")
    }

    override func viewDidLoad() {
        super.viewDidLoad()
        ScaleToFitView.install(in: view, designSize: CGSize(width: 320.0, height: 416.0))
    }

    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        let gameID = initialSchedule.gameID
        gamePicker.selectRow(catalog.index(of: gameID)!, inComponent: 0, animated: false)
        setGame(catalog.game(gameID)!)
    }

    private func setGame(_ newGame: Game) {
        if newGame === game {
            NSLog("setGame: same game")
            return
        }

        scrollView.subviews.forEach { $0.removeFromSuperview() }
        game = newGame

        scrollView.frame.size.height = PayScheduleView.height(for: newGame.schedules[0])

        // page control and picker sit directly below the pay table
        pageControl.frame.origin.y = scrollView.frame.maxY
        gamePicker.frame.origin.y = pageControl.frame.maxY

        loadPage(0)
        loadPage(1)

        let pageCount = newGame.schedules.count
        scrollView.contentSize = CGSize(width: scrollView.frame.width * CGFloat(pageCount), height: scrollView.frame.height)
        pageControl.numberOfPages = pageCount
        pageControl.currentPage = 0

        scrollView.setNeedsDisplay()
        pageControl.setNeedsDisplay()

        updateLabel()
    }

    private var currentSchedule: PaySchedule {
        game!.schedules[pageControl.currentPage]
    }

    private func updateLabel() {
        let schedule = currentSchedule
        label.text = String(format: "%@ - %4.2f%%", schedule.game.name, schedule.estimatedReturn)
    }

    private func loadPage(_ page: Int) {
        guard let game = game, page >= 0, page < game.schedules.count else {
            return
        }

        let controller: PayScheduleViewController
        if let existing = pages[game.id]![page] {
            controller = existing
        } else {
            controller = PayScheduleViewController(paySchedule: game.schedules[page])
            pages[game.id]![page] = controller
        }

        if controller.view.superview == nil {
            var frame = scrollView.frame
            frame.origin.x = frame.size.width * CGFloat(page)
            frame.origin.y = 0
            controller.view.frame = frame
            scrollView.addSubview(controller.view)
        }
    }

    /// Loads the page and the pages on either side of it (to avoid flashes when the user starts scrolling)
    private func loadPages(around page: Int) {
        loadPage(page - 1)
        loadPage(page)
        loadPage(page + 1)
    }

    @IBAction func pageControlWasPressed(_ sender: Any) {
        let page = pageControl.currentPage
        loadPages(around: page)

        var frame = scrollView.frame
        frame.origin.x = frame.size.width * CGFloat(page)
        frame.origin.y = 0
        scrollView.scrollRectToVisible(frame, animated: true)

        // See scrollViewDidScroll
        pageControlUsed = true

        updateLabel()
    }

    @objc func done() {
        delegate?.chooseGameDone(currentSchedule)
    }

    @objc func cancel() {
        delegate?.chooseGameCancel()
    }
}

extension ChooseGameViewController: UIPickerViewDataSource, UIPickerViewDelegate {
    func numberOfComponents(in pickerView: UIPickerView) -> Int {
        1
    }

    func pickerView(_ pickerView: UIPickerView, numberOfRowsInComponent component: Int) -> Int {
        catalog.games.count
    }

    func pickerView(_ pickerView: UIPickerView, titleForRow row: Int, forComponent component: Int) -> String? {
        catalog.games[row].name
    }

    func pickerView(_ pickerView: UIPickerView, didSelectRow row: Int, inComponent component: Int) {
        setGame(catalog.games[row])
    }
}

extension ChooseGameViewController: UIScrollViewDelegate {
    func scrollViewDidScroll(_ scrollView: UIScrollView) {
        // Avoid a feedback loop between the page control and scrolling to the page it picked
        if pageControlUsed {
            return
        }

        // Switch the indicator when more than 50% of the previous/next page is visible
        let pageWidth = scrollView.frame.size.width
        let page = Int(floor((scrollView.contentOffset.x - pageWidth / 2) / pageWidth)) + 1
        pageControl.currentPage = page

        loadPages(around: page)
        updateLabel()
    }

    func scrollViewWillBeginDragging(_ scrollView: UIScrollView) {
        pageControlUsed = false
    }

    func scrollViewDidEndDecelerating(_ scrollView: UIScrollView) {
        pageControlUsed = false
    }
}
