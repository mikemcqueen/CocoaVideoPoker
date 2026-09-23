//
//  PayScheduleView.swift
//  VideoPoker
//
//  Created by Mike McQueen on 9/4/10.
//  Copyright 2010 Mike McQueen. All rights reserved.
//

import UIKit

protocol PayScheduleViewDelegate: AnyObject {
    var paySchedule: PaySchedule { get }
    /// Coins bet, highlighted in the table; 0 for none
    var bet: Int { get }
}

/// Draws a pay table: hand names, then payouts for 1-5 coins
final class PayScheduleView: UIView {
    // Connected in the nib; must conform to PayScheduleViewDelegate
    @IBOutlet weak var delegate: AnyObject?

    private static let borderWidth: CGFloat = 2.0
    private static let textSpacing: CGFloat = 2.0
    private static let lineHeight: CGFloat = 13.0 // TODO: font.height

    static func height(for schedule: PaySchedule) -> CGFloat {
        CGFloat(schedule.paylineCount) * lineHeight + borderWidth * 2.0 + 3.0
    }

    /// Text size rounded up to whole points, as the legacy sizeWithFont: did
    private static func size(of text: NSString, font: UIFont) -> CGSize {
        let size = text.size(withAttributes: [.font: font])
        return CGSize(width: ceil(size.width), height: ceil(size.height))
    }

    override func draw(_ rect: CGRect) {
        guard let context = UIGraphicsGetCurrentContext(),
              let delegate = delegate as? PayScheduleViewDelegate else {
            return
        }
        let borderWidth = PayScheduleView.borderWidth
        let textSpacing = PayScheduleView.textSpacing
        let lineHeight = PayScheduleView.lineHeight
        let rect = bounds

        let divisor: CGFloat = 10
        let columnCount = 6
        let unitWidth = rect.width / divisor

        let yellow = UIColor(red: 1.0, green: 1.0, blue: 0.0, alpha: 1.0)

        // Dark blue, outlined in yellow
        context.setStrokeColor(yellow.cgColor)
        context.setFillColor(red: 0.0, green: 0.0, blue: 0.5, alpha: 1.0)
        context.fill(rect)
        context.stroke(rect, width: borderWidth)

        // Column dividers, and the bet column highlighted in red
        let bet = delegate.bet
        var columns: [(offset: CGFloat, width: CGFloat)] = []
        var x = rect.origin.x
        for columnIndex in 0..<columnCount {
            let columnWidth: CGFloat
            if columnIndex == 0 {
                columnWidth = 4.125 * unitWidth
            } else if columnIndex < columnCount - 1 {
                columnWidth = unitWidth
            } else {
                columnWidth = rect.width - x
            }
            columns.append((x, columnWidth))
            x += columnWidth

            context.stroke(CGRect(x: x, y: rect.origin.y, width: 0.0, height: rect.height), width: borderWidth)

            if bet > 0 && bet == columnIndex {
                context.setFillColor(red: 0.7, green: 0.0, blue: 0.0, alpha: 1.0)
                context.fill(CGRect(x: x - columnWidth, y: rect.origin.y, width: columnWidth, height: rect.height).insetBy(dx: 1, dy: 1))
            }
        }

        let font = UIFont(name: "Helvetica-Bold", size: 12.0)!
        context.setFillColor(yellow.cgColor)

        var point = CGPoint(x: columns[0].offset + borderWidth + textSpacing, y: borderWidth)
        let maxWidth = columns[0].width - borderWidth * 2.0 - textSpacing * 2.0

        let schedule = delegate.paySchedule
        for payline in 0..<schedule.paylineCount {
            let text = schedule.handValue(ofPayline: payline).name as NSString

            // Shrink long hand names just enough to fit the column
            var nameFont = font
            var textSize = PayScheduleView.size(of: text, font: nameFont)
            if textSize.width > maxWidth {
                nameFont = font.withSize(floor(font.pointSize * maxWidth / textSize.width * 10.0) / 10.0)
                textSize = PayScheduleView.size(of: text, font: nameFont)
            }
            // Draw the hand name, vertically centered on the line
            let namePoint = CGPoint(x: point.x, y: point.y + (font.lineHeight - nameFont.lineHeight) / 2.0)
            text.draw(at: namePoint, withAttributes: [.font: nameFont, .foregroundColor: yellow])

            // Draw dots after the name to fill in the column
            var dotOffset = CGPoint(x: textSize.width + 1.0, y: lineHeight / 2.0 + 1)
            while dotOffset.x <= maxWidth {
                context.fill(CGRect(x: point.x + dotOffset.x, y: point.y + dotOffset.y, width: 1.0, height: 1.0))
                dotOffset.x += 2
            }

            for columnIndex in 1..<columnCount {
                let column = columns[columnIndex]
                let payoutRect = CGRect(x: column.offset, y: point.y, width: column.width, height: lineHeight)
                    .insetBy(dx: textSpacing, dy: 0.0)
                let payout = schedule.payout(ofPayline: payline, coinCount: columnIndex)
                let payoutText: String
                if columnIndex == columnCount - 1 && payout == 4000 {
                    // TODO: 4000 is hack; schedule.jackpot?
                    payoutText = String(format: "$%6.2f", Double(payout))
                } else {
                    payoutText = "\(payout)"
                }
                (payoutText as NSString).draw(in: payoutRect, withAttributes: [.font: font])
            }

            point.y += lineHeight
        }
    }
}
