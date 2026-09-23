//
//  AppDelegate.swift
//  VideoPoker
//
//  Created by Mike McQueen on 8/1/09.
//  Copyright (C) 2009 Mike McQueen. All rights reserved.
//

import UIKit

final class AppDelegate: UIResponder, UIApplicationDelegate {
    @IBOutlet var window: UIWindow?
    @IBOutlet var tabController: UITabBarController!
    /// Not connected in MainWindow.xib (as before): saving a session that has
    /// no hands yet fails model validation, which would abort
    @IBOutlet var game: GameModel?

    func applicationDidFinishLaunching(_ application: UIApplication) {
        window?.frame = window?.screen.bounds ?? .zero
        window?.rootViewController = tabController
        window?.makeKeyAndVisible()
    }

    func applicationDidEnterBackground(_ application: UIApplication) {
        game?.save()
    }

    func applicationWillTerminate(_ application: UIApplication) {
        game?.save()
    }
}
