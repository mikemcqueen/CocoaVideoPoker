//
//  GraphicsStyle.swift
//  VideoPoker
//

import Foundation

/// Which set of card and pay table graphics to draw
enum GraphicsStyle {
    /// Casino machine look: bright blue and yellow pay table, big-number cards
    case modern
    /// The original bitmap cards and dark blue pay table
    case classic

    private static let classicKey = "classicGraphics"

    static let didChangeNotification = Notification.Name("GraphicsStyleDidChange")

    static var current: GraphicsStyle {
        get {
            UserDefaults.standard.bool(forKey: classicKey) ? .classic : .modern
        }
        set {
            UserDefaults.standard.set(newValue == .classic, forKey: classicKey)
            NotificationCenter.default.post(name: didChangeNotification, object: nil)
        }
    }
}
