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

/// Colors and fonts of a pay table
private struct PayTableStyle {
    let background: UIColor
    let betColumn: UIColor
    let text: UIColor
    let font: UIFont
    /// nil draws payouts in the default (black)
    let payoutColor: UIColor?
    let payoutAlignment: NSTextAlignment
    let textShadow: NSShadow?
    /// Size the payout columns to fit their payouts, rather than tenths of the width
    let fitsColumnsToPayouts: Bool
    /// Space between a payout and the divider to its right
    let payoutRightPadding: CGFloat

    private static let yellow = UIColor(red: 1.0, green: 1.0, blue: 0.0, alpha: 1.0)

    static let classic = PayTableStyle(
        background: UIColor(red: 0.0, green: 0.0, blue: 0.5, alpha: 1.0),
        betColumn: UIColor(red: 0.7, green: 0.0, blue: 0.0, alpha: 1.0),
        text: yellow,
        font: UIFont(name: "Helvetica-Bold", size: 12.0)!,
        payoutColor: nil,
        payoutAlignment: .left,
        textShadow: nil,
        fitsColumnsToPayouts: false,
        payoutRightPadding: 2.0)

    static let modern: PayTableStyle = {
        let shadow = NSShadow()
        shadow.shadowColor = UIColor(white: 0.0, alpha: 0.8)
        shadow.shadowOffset = CGSize(width: 0.6, height: 0.6)
        shadow.shadowBlurRadius = 0.0
        return PayTableStyle(
            background: UIColor(red: 0.05, green: 0.15, blue: 0.8, alpha: 1.0),
            betColumn: UIColor(red: 0.93, green: 0.1, blue: 0.1, alpha: 1.0),
            text: UIColor(red: 1.0, green: 0.93, blue: 0.0, alpha: 1.0),
            font: UIFont(name: "ArialRoundedMTBold", size: 11.5) ?? .boldSystemFont(ofSize: 11.5),
            payoutColor: UIColor(red: 1.0, green: 0.93, blue: 0.0, alpha: 1.0),
            payoutAlignment: .right,
            textShadow: shadow,
            fitsColumnsToPayouts: true,
            payoutRightPadding: 5.0)
    }()

    static var current: PayTableStyle {
        GraphicsStyle.current == .modern ? modern : classic
    }
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

    private static func payoutText(_ schedule: PaySchedule, payline: Int, coinCount: Int) -> String {
        let payout = schedule.payout(ofPayline: payline, coinCount: coinCount)
        if coinCount == 5 && payout == 4000 {
            // TODO: 4000 is hack; schedule.jackpot?
            return String(format: "$%6.2f", Double(payout))
        }
        return "\(payout)"
    }

    /// Widths of the name column, the 1-4 coin columns and the 5 coin column:
    /// the payout columns just fit their widest payout, and names get the rest
    private static func fittedColumnWidths(_ schedule: PaySchedule, style: PayTableStyle, totalWidth: CGFloat)
        -> (name: CGFloat, payout: CGFloat, last: CGFloat)
    {
        func widest(_ coinCounts: ClosedRange<Int>) -> CGFloat {
            coinCounts.flatMap { coinCount in
                (0..<schedule.paylineCount).map { payline in
                    size(of: payoutText(schedule, payline: payline, coinCount: coinCount) as NSString, font: style.font).width
                }
            }.max() ?? 0
        }
        let padding = textSpacing + 1.0 + style.payoutRightPadding + 1.0 // + left space, text shadow
        let payout = widest(1...4) + padding
        let last = widest(5...5) + padding
        return (totalWidth - 4 * payout - last, payout, last)
    }

    override func awakeFromNib() {
        super.awakeFromNib()
        NotificationCenter.default.addObserver(self, selector: #selector(graphicsStyleDidChange),
                                               name: GraphicsStyle.didChangeNotification, object: nil)
    }

    @objc private func graphicsStyleDidChange() {
        setNeedsDisplay()
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

        let style = PayTableStyle.current

        // Blue, outlined in yellow
        context.setStrokeColor(style.text.cgColor)
        context.setFillColor(style.background.cgColor)
        context.fill(rect)
        context.stroke(rect, width: borderWidth)

        // Column dividers, and the bet column highlighted in red
        let bet = delegate.bet
        let schedule = delegate.paySchedule
        let fitted = style.fitsColumnsToPayouts
            ? PayScheduleView.fittedColumnWidths(schedule, style: style, totalWidth: rect.width) : nil
        var columns: [(offset: CGFloat, width: CGFloat)] = []
        var x = rect.origin.x
        for columnIndex in 0..<columnCount {
            let columnWidth: CGFloat
            if columnIndex == 0 {
                columnWidth = fitted?.name ?? 4.125 * unitWidth
            } else if columnIndex < columnCount - 1 {
                columnWidth = fitted?.payout ?? unitWidth
            } else {
                columnWidth = rect.width - x
            }
            columns.append((x, columnWidth))
            x += columnWidth

            context.stroke(CGRect(x: x, y: rect.origin.y, width: 0.0, height: rect.height), width: borderWidth)

            if bet > 0 && bet == columnIndex {
                context.setFillColor(style.betColumn.cgColor)
                context.fill(CGRect(x: x - columnWidth, y: rect.origin.y, width: columnWidth, height: rect.height).insetBy(dx: 1, dy: 1))
            }
        }

        let font = style.font
        context.setFillColor(style.text.cgColor)
        var nameAttributes: [NSAttributedString.Key: Any] = [.foregroundColor: style.text]
        var payoutAttributes: [NSAttributedString.Key: Any] = [.font: font]
        if let payoutColor = style.payoutColor {
            payoutAttributes[.foregroundColor] = payoutColor
        }
        if style.payoutAlignment != .left {
            let paragraph = NSMutableParagraphStyle()
            paragraph.alignment = style.payoutAlignment
            payoutAttributes[.paragraphStyle] = paragraph
        }
        if let shadow = style.textShadow {
            nameAttributes[.shadow] = shadow
            payoutAttributes[.shadow] = shadow
        }

        var point = CGPoint(x: columns[0].offset + borderWidth + textSpacing, y: borderWidth)
        let maxWidth = columns[0].width - borderWidth * 2.0 - textSpacing * 2.0

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
            nameAttributes[.font] = nameFont
            text.draw(at: namePoint, withAttributes: nameAttributes)

            // Draw dots after the name to fill in the column
            var dotOffset = CGPoint(x: textSize.width + 1.0, y: lineHeight / 2.0 + 1)
            while dotOffset.x <= maxWidth {
                context.fill(CGRect(x: point.x + dotOffset.x, y: point.y + dotOffset.y, width: 1.0, height: 1.0))
                dotOffset.x += 2
            }

            for columnIndex in 1..<columnCount {
                let column = columns[columnIndex]
                let payoutRect = CGRect(x: column.offset + textSpacing, y: point.y,
                                        width: column.width - textSpacing - style.payoutRightPadding, height: lineHeight)
                let payoutText = PayScheduleView.payoutText(schedule, payline: payline, coinCount: columnIndex)
                (payoutText as NSString).draw(in: payoutRect, withAttributes: payoutAttributes)
            }

            point.y += lineHeight
        }
    }
}
