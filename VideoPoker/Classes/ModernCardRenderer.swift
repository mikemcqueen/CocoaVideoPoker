//
//  ModernCardRenderer.swift
//  VideoPoker
//
//  Draws card faces the way video poker machines do: a big number and small
//  suit in the corner, one large suit, and a portrait logo on face cards and aces.
//

import UIKit

enum ModernCardRenderer {
    private static let red = UIColor(red: 0.86, green: 0.0, blue: 0.0, alpha: 1.0)

    private static let suitSymbols: [Suit: String] = [
        .clubs: "suit.club.fill",
        .diamonds: "suit.diamond.fill",
        .hearts: "suit.heart.fill",
        .spades: "suit.spade.fill",
    ]

    private static let logos: [Int: String] = [
        CardNumber.jack: "💂",
        CardNumber.queen: "👸",
        CardNumber.king: "🤴",
        CardNumber.ace: "🛡️",
    ]

    static func numberString(_ number: Int) -> String {
        switch number {
        case CardNumber.jack:   return "J"
        case CardNumber.queen:  return "Q"
        case CardNumber.king:   return "K"
        case CardNumber.ace:    return "A"
        default:                return "\(number)"
        }
    }

    /// Draws the card face filling `rect` in the current UIKit context
    static func draw(_ card: Card, in rect: CGRect) {
        let width = rect.width
        let height = rect.height
        let color = (card.suit == .hearts || card.suit == .diamonds) ? red : .black

        // Card
        let cardPath = UIBezierPath(roundedRect: rect.insetBy(dx: 0.5, dy: 0.5), cornerRadius: width * 0.08)
        UIColor.white.setFill()
        cardPath.fill()
        UIColor(white: 0.6, alpha: 1.0).setStroke()
        cardPath.lineWidth = 0.5
        cardPath.stroke()

        // Number in the top left corner, small suit below it
        let font = UIFont(name: "HelveticaNeue-CondensedBold", size: height * 0.3) ?? .boldSystemFont(ofSize: height * 0.3)
        let number = numberString(card.number) as NSString
        number.draw(at: CGPoint(x: rect.minX + width * 0.07, y: rect.minY + height * 0.01),
                    withAttributes: [.font: font, .foregroundColor: color])

        drawSuit(card.suit, color: color,
                 in: CGRect(x: rect.minX + width * 0.07, y: rect.minY + height * 0.33, width: width * 0.3, height: height * 0.22))

        // Portrait logo in the top right corner of face cards and aces
        if let logo = logos[card.number] {
            let logoFont = UIFont.systemFont(ofSize: width * 0.44)
            let text = logo as NSString
            let size = text.size(withAttributes: [.font: logoFont])
            let logoRect = CGRect(x: rect.minX + width * 0.42, y: rect.minY + height * 0.04, width: width * 0.54, height: height * 0.42)
            text.draw(at: CGPoint(x: logoRect.midX - size.width / 2, y: logoRect.midY - size.height / 2),
                      withAttributes: [.font: logoFont])
        }

        // Large suit
        drawSuit(card.suit, color: color,
                 in: CGRect(x: rect.minX + width * 0.3, y: rect.minY + height * 0.52, width: width * 0.6, height: height * 0.4))
    }

    /// Draws a suit symbol as large as fits, centered in `rect`
    private static func drawSuit(_ suit: Suit, color: UIColor, in rect: CGRect) {
        let configuration = UIImage.SymbolConfiguration(pointSize: rect.height, weight: .regular)
        guard let symbol = UIImage(systemName: suitSymbols[suit]!, withConfiguration: configuration)?
            .withTintColor(color, renderingMode: .alwaysOriginal) else {
            return
        }
        let scale = min(rect.width / symbol.size.width, rect.height / symbol.size.height)
        let size = CGSize(width: symbol.size.width * scale, height: symbol.size.height * scale)
        symbol.draw(in: CGRect(x: rect.midX - size.width / 2, y: rect.midY - size.height / 2,
                               width: size.width, height: size.height))
    }
}
