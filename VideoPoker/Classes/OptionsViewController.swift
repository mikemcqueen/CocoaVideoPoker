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

/// Options (the Train and Game switches aren't implemented yet)
final class OptionsViewController: UITableViewController {
    weak var delegate: OptionsViewControllerDelegate?

    @IBOutlet var trainMode: UITableViewCell!
    @IBOutlet var showReturn: UITableViewCell!
    @IBOutlet var showHoldBest: UITableViewCell!
    @IBOutlet var gameSpeed: UITableViewCell!
    @IBOutlet var autoAddFunds: UITableViewCell!

    private lazy var classicGraphics: UITableViewCell = {
        let cell = UITableViewCell(style: .default, reuseIdentifier: nil)
        cell.textLabel?.text = "Classic Graphics"
        cell.selectionStyle = .none
        let toggle = UISwitch()
        toggle.isOn = GraphicsStyle.current == .classic
        toggle.accessibilityIdentifier = "classicGraphics"
        toggle.addTarget(self, action: #selector(classicGraphicsChanged(_:)), for: .valueChanged)
        cell.accessoryView = toggle
        return cell
    }()

    private var sections: [(title: String, cells: [UITableViewCell])] {
        [("Train", [trainMode, showReturn, showHoldBest]),
         ("Game", [gameSpeed, autoAddFunds]),
         ("Graphics", [classicGraphics])]
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

    @objc private func classicGraphicsChanged(_ sender: UISwitch) {
        GraphicsStyle.current = sender.isOn ? .classic : .modern
    }

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
