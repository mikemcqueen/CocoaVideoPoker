//
//  ScaleToFitView.swift
//  VideoPoker
//
//  Scales a fixed-size (320pt wide) legacy layout uniformly so it fills the
//  width of the screen, centered and clear of the navigation and tab bars.
//

import UIKit

final class ScaleToFitView: UIView {
    /// Holds the nib's subviews at the size the nib was laid out for
    private(set) var designView = UIView()
    private var designSize = CGSize.zero

    /// Moves all current subviews of rootView into a design view of designSize
    /// (the size the nib was laid out for), and scales that to fit rootView.
    /// Call at the end of viewDidLoad, after all subviews have been added.
    @discardableResult
    static func install(in rootView: UIView, designSize: CGSize) -> ScaleToFitView {
        // rootView may already have been resized to the screen; shrink it back to
        // the nib size so the children's autoresizing puts them where designed
        let rootFrame = rootView.frame
        rootView.frame = CGRect(origin: rootFrame.origin, size: designSize)

        let designView = UIView(frame: CGRect(origin: .zero, size: designSize))
        designView.backgroundColor = .clear
        for child in rootView.subviews {
            designView.addSubview(child)
        }

        let view = ScaleToFitView(frame: rootView.bounds)
        view.designView = designView
        view.designSize = designSize
        view.backgroundColor = .clear
        view.autoresizingMask = [.flexibleWidth, .flexibleHeight]
        view.addSubview(designView)
        rootView.addSubview(view)

        rootView.frame = rootFrame
        rootView.autoresizesSubviews = true     // legacy nibs often have this off
        view.frame = rootView.bounds
        return view
    }

    override func safeAreaInsetsDidChange() {
        super.safeAreaInsetsDidChange()
        setNeedsLayout()
    }

    override func layoutSubviews() {
        super.layoutSubviews()

        let available = bounds.inset(by: safeAreaInsets)
        guard designSize.width > 0.0, designSize.height > 0.0, !available.isEmpty else {
            return
        }

        // content can grow past the nib height (e.g. a taller pay table)
        let contentHeight = designView.subviews.filter { !$0.isHidden }.map { $0.frame.maxY }
            .reduce(designSize.height, max)

        let scale = min(available.width / designSize.width, available.height / contentHeight)

        designView.transform = .identity
        designView.bounds = CGRect(origin: .zero, size: designSize)
        designView.transform = CGAffineTransform(scaleX: scale, y: scale)
        designView.center = CGPoint(x: available.midX, y: available.minY + designSize.height * scale / 2.0)

        let screenScale = window?.screen.scale ?? traitCollection.displayScale
        ScaleToFitView.setContentScale(of: designView, to: screenScale * scale)
    }

    /// Re-renders text and custom-drawn views at the scaled resolution so they
    /// stay sharp instead of being bitmap-stretched by the transform
    private static func setContentScale(of view: UIView, to contentScale: CGFloat) {
        let drawSelector = #selector(UIView.draw(_:))
        let drawsItself = class_getMethodImplementation(type(of: view), drawSelector)
            != class_getMethodImplementation(UIView.self, drawSelector)
        if (view is UILabel || drawsItself) && view.contentScaleFactor != contentScale {
            view.contentScaleFactor = contentScale
            view.setNeedsDisplay()
        }
        for child in view.subviews {
            setContentScale(of: child, to: contentScale)
        }
    }
}
