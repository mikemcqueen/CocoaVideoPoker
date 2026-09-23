//
//  OptionsViewController.swift
//  VideoPoker
//
//  Created by Mike McQueen on 8/1/09.
//

import UIKit

protocol OptionsViewControllerDelegate: AnyObject {
    func optionsViewControllerDidFinish(_ controller: OptionsViewController)
}

/// Options (not implemented yet: the switches do nothing)
final class OptionsViewController: UITableViewController {
    weak var delegate: OptionsViewControllerDelegate?

    @IBOutlet var trainMode: UITableViewCell!
    @IBOutlet var showReturn: UITableViewCell!
    @IBOutlet var showHoldBest: UITableViewCell!
    @IBOutlet var gameSpeed: UITableViewCell!
    @IBOutlet var autoAddFunds: UITableViewCell!

    private var sections: [(title: String, cells: [UITableViewCell])] {
        [("Train", [trainMode, showReturn, showHoldBest]),
         ("Game", [gameSpeed, autoAddFunds])]
    }

    override func viewDidLoad() {
        super.viewDidLoad()
        navigationItem.rightBarButtonItem = UIBarButtonItem(barButtonSystemItem: .done, target: self, action: #selector(done))
        navigationItem.title = "Options"
    }

    @IBAction func done() {
        delegate?.optionsViewControllerDidFinish(self)
    }

    @IBAction func trainModeChanged() {}
    @IBAction func showReturnChanged() {}
    @IBAction func showHoldBestChanged() {}
    @IBAction func gameSpeedChanged() {}
    @IBAction func autoAddFundsChanged() {}

    override func numberOfSections(in tableView: UITableView) -> Int {
        sections.count
    }

    override func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        sections[section].cells.count
    }

    override func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        sections[indexPath.section].cells[indexPath.row]
    }

    override func tableView(_ tableView: UITableView, titleForHeaderInSection section: Int) -> String? {
        sections[section].title
    }

    override func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        NSLog("DidSelect Row \(indexPath.row)")
    }
}
