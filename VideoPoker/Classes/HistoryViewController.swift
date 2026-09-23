//
//  HistoryViewController.swift
//  VideoPoker
//
//  Created by Mike McQueen on 10/27/10.
//  Copyright 2010 Mike McQueen. All rights reserved.
//

import CoreData
import UIKit

/// The History tab: past sessions, newest first
final class HistoryViewController: UIViewController {
    @IBOutlet var game: GameModel!
    @IBOutlet var tableView: UITableView!
    // Loaded from SessionTableCell.xib as needed
    @IBOutlet var tableCell: UITableViewCell?

    private var fetchedResults: NSFetchedResultsController<Session>?

    private let dateFormatter: DateFormatter = {
        let formatter = DateFormatter()
        formatter.dateStyle = .medium
        formatter.timeStyle = .short
        return formatter
    }()

    override func viewDidLoad() {
        super.viewDidLoad()
        // TODO: TEMPORARY
        navigationItem.rightBarButtonItem = UIBarButtonItem(title: "Delete all", style: .done,
                                                            target: self, action: #selector(askDeleteAll))
        initFetchedResultsController()
    }

    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        fetchAndRedraw()
    }

    private func initFetchedResultsController() {
        guard let context = game.context else {
            return
        }
        let request = Session.fetchRequest()
        request.sortDescriptors = [NSSortDescriptor(key: "startDate", ascending: false)]
        fetchedResults = NSFetchedResultsController(fetchRequest: request, managedObjectContext: context,
                                                    sectionNameKeyPath: nil, cacheName: nil) // TODO: cache?
    }

    private func fetchAndRedraw() {
        do {
            try fetchedResults?.performFetch()
        } catch {
            fatalError("HistoryView: performFetch: \(error)")
        }
        tableView.reloadData()
    }

    @objc private func askDeleteAll() {
        GameModel.askDeleteHistory(from: self) { _ in
            // TODO: doesn't work yet; need to invalidate/re-create the current session & hand
        }
    }
}

extension HistoryViewController: UITableViewDataSource, UITableViewDelegate {
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        fetchedResults?.sections?[section].numberOfObjects ?? 0
    }

    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        var cell = tableView.dequeueReusableCell(withIdentifier: "HistoryTableCell")
        if cell == nil {
            Bundle.main.loadNibNamed("SessionTableCell", owner: self, options: nil)
            cell = tableCell
            tableCell = nil
        }

        let session = fetchedResults!.object(at: indexPath)
        let gameName = session.gameID.flatMap { PayScheduleCatalog.shared.game($0)?.name }
        (cell?.viewWithTag(1) as? UILabel)?.text = gameName ?? "Error"
        (cell?.viewWithTag(2) as? UILabel)?.text = session.startDate.map { dateFormatter.string(from: $0) }
        (cell?.viewWithTag(3) as? UILabel)?.text = "\(session.perfectPlayCount)"
        (cell?.viewWithTag(4) as? UILabel)?.text = "\(session.handCount)"
        return cell!
    }

    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        tableView.deselectRow(at: indexPath, animated: false)
        let controller = SessionDetailsViewController(nibName: "SessionDetailsView", bundle: nil)
        controller.game = game
        controller.session = fetchedResults!.object(at: indexPath)
        navigationController?.pushViewController(controller, animated: true)
    }
}
