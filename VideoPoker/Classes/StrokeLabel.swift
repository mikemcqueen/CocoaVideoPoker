//
//  StrokeLabel.swift
//  VideoPoker
//
//  Created by Mike McQueen on 10/9/10.
//  Copyright 2010 Mike McQueen. All rights reserved.
//

import CoreText
import UIKit

/// A label drawn as text outlined in `strokeColor` and filled with `textColor`
final class StrokeLabel: UILabel {
    var strokeColor = UIColor.white
    var strokeWidth: CGFloat = 1

    override func drawText(in rect: CGRect) {
        guard let context = UIGraphicsGetCurrentContext(), let text = text else {
            return
        }

        // TODO: text alignment; always drawn in 14pt Helvetica, with 1.5pt
        // character spacing and no kerning
        let font = CTFontCreateWithName("Helvetica" as CFString, 14, nil)
        let characters = Array(text.utf16)
        var glyphs = [CGGlyph](repeating: 0, count: characters.count)
        CTFontGetGlyphsForCharacters(font, characters, &glyphs, characters.count)
        var advances = [CGSize](repeating: .zero, count: glyphs.count)
        CTFontGetAdvancesForGlyphs(font, .horizontal, glyphs, &advances, glyphs.count)
        let characterSpacing: CGFloat = 1.5
        var x: CGFloat = 1
        let positions = advances.map { advance -> CGPoint in
            defer { x += advance.width + characterSpacing }
            return CGPoint(x: x, y: 5)
        }

        context.saveGState()
        context.translateBy(x: 0, y: bounds.size.height)
        context.scaleBy(x: 1, y: -1)
        context.textMatrix = .identity
        context.setFont(CTFontCopyGraphicsFont(font, nil))
        context.setFontSize(CTFontGetSize(font))
        context.setLineWidth(strokeWidth)
        context.setFillColor(textColor.cgColor)
        context.setStrokeColor(strokeColor.cgColor)

        for mode in [CGTextDrawingMode.stroke, .fill] {
            context.setTextDrawingMode(mode)
            context.showGlyphs(glyphs, at: positions)
        }
        context.restoreGState()
    }
}
