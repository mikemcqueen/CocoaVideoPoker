//
//  SessionDetailsViewController.swift
//  VideoPoker
//
//  Created by Mike McQueen on 11/18/10.
//  Copyright 2010 Mike McQueen. All rights reserved.
//

import CoreData
import UIKit

/// The hands of a session, newest first
final class SessionDetailsViewController: UIViewController {
    var game: GameModel!
    var session: Session!

    @IBOutlet var tableView: UITableView!
    // Loaded from HandHistoryTable.xib as needed
    @IBOutlet var tableCell: UITableViewCell?

    private var fetchedResults: NSFetchedResultsController<Hand>?

    override func viewDidLoad() {
        super.viewDidLoad()
        guard let context = game.context else {
            return
        }
        let request = Hand.fetchRequest()
        request.predicate = NSPredicate(format: "session == %@", session)
        request.sortDescriptors = [NSSortDescriptor(key: "startDate", ascending: false)]
        fetchedResults = NSFetchedResultsController(fetchRequest: request, managedObjectContext: context,
                                                    sectionNameKeyPath: nil, cacheName: nil) // TODO: cache?
    }

    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        do {
            try fetchedResults?.performFetch()
        } catch {
            fatalError("SessionDetailsView: performFetch: \(error)")
        }
        tableView.reloadData()
    }
}

extension SessionDetailsViewController: UITableViewDataSource, UITableViewDelegate {
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        fetchedResults?.sections?[section].numberOfObjects ?? 0
    }

    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        var cell = tableView.dequeueReusableCell(withIdentifier: "HandTableCell")
        if cell == nil {
            Bundle.main.loadNibNamed("HandHistoryTable", owner: self, options: nil)
            cell = tableCell
            tableCell = nil
        }
        // TODO: configure cell with the hand's data
        return cell!
    }

    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        tableView.deselectRow(at: indexPath, animated: false)
    }
}
