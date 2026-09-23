//
//  SolveViewController.swift
//  VideoPoker
//
//  Created by Mike McQueen on 8/2/10.
//

import UIKit

/// The Solve tab: pick any 5 cards and see the best way to play them
final class SolveViewController: UIViewController {
    @IBOutlet var game: GameModel!
    @IBOutlet var cardTable: CardTableViewController!
    @IBOutlet var returnViewController: ReturnViewController!
    @IBOutlet var cardPicker: UIPickerView!
    @IBOutlet var holdBestButton: UIButton!

    private let hand = CurrentHand()
    private var selectedView: CardView!
    private var suitImages: [UIImage] = []
    private var numberLabels: [UILabel] = []

    private static let pickerItemSize = CGSize(width: 48, height: 53)

    override func viewDidLoad() {
        super.viewDidLoad()

        view.addSubview(cardTable.view)

        let top = cardTable.view.frame.maxY + 3.0
        returnViewController.view.frame = returnViewController.view.bounds.offsetBy(dx: 0.0, dy: top)
        view.addSubview(returnViewController.view)

        // Start with a royal flush in spades
        game.hand = hand
        game.setCards((0..<5).map { Card(number: 10 + $0, suit: .spades) })

        selectedView = cardTable.cardTableView.cardView(at: 0)

        buildSuitImages()
        buildNumberLabels()

        ScaleToFitView.install(in: view, designSize: CGSize(width: 320.0, height: 367.0))
        NSLog("SolveView::viewDidLoad()")
    }

    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        game.hand = hand
    }

    private func buildSuitImages() {
        guard let allSuits = CardImages.load("suits-60.png") else {
            return
        }
        let size = SolveViewController.pickerItemSize
        suitImages = Suit.allCases.compactMap { suit in
            allSuits.cropping(to: CGRect(origin: CGPoint(x: size.width * CGFloat(suit.rawValue), y: 0), size: size))
                .map { UIImage(cgImage: $0) }
        }
    }

    private func buildNumberLabels() {
        numberLabels = (CardNumber.deuce...CardNumber.ace).map { number in
            let label = UILabel(frame: CGRect(origin: .zero, size: SolveViewController.pickerItemSize))
            label.text = SolveViewController.numberString(number)
            label.adjustsFontSizeToFitWidth = false
            label.numberOfLines = 1
            label.font = .boldSystemFont(ofSize: 36.0)
            return label
        }
    }

    private static func numberString(_ number: Int) -> String {
        switch number {
        case 2...10:            return "\(number)"
        case CardNumber.jack:   return "J"
        case CardNumber.queen:  return "Q"
        case CardNumber.king:   return "K"
        case CardNumber.ace:    return "A"
        default:                return "ERR"
        }
    }

    private func setPicker(to card: Card) {
        cardPicker.selectRow(card.number - 2, inComponent: 0, animated: true)
        cardPicker.selectRow(card.suit.rawValue, inComponent: 1, animated: true)
    }

    private func cardsDidChange() {
        let valid = game.hand.isValid
        holdBestButton.isEnabled = valid
        returnViewController.setInvalid(!valid)
        if valid {
            returnViewController.setYourEv(game.ev(forCardBits: 0))
            returnViewController.setBestEv(game.bestEv)
        }
    }

    private func showHandDetails() {
        let controller = HandDetailsViewController(nibName: "HandDetailsView", bundle: nil)
        controller.game = game
        controller.hidesBottomBarWhenPushed = true
        navigationController?.pushViewController(controller, animated: true)
    }
}

extension SolveViewController: UIPickerViewDataSource, UIPickerViewDelegate {
    func numberOfComponents(in pickerView: UIPickerView) -> Int {
        2
    }

    func pickerView(_ pickerView: UIPickerView, numberOfRowsInComponent component: Int) -> Int {
        component == 0 ? numberLabels.count : suitImages.count
    }

    func pickerView(_ pickerView: UIPickerView, widthForComponent component: Int) -> CGFloat {
        54.0
    }

    func pickerView(_ pickerView: UIPickerView, rowHeightForComponent component: Int) -> CGFloat {
        53.0
    }

    func pickerView(_ pickerView: UIPickerView, viewForRow row: Int, forComponent component: Int, reusing view: UIView?) -> UIView {
        // The picker may show a row in more than one place at once (e.g. the
        // selection highlight), so return a distinct view rather than a shared one
        let frame = CGRect(origin: .zero, size: SolveViewController.pickerItemSize)
        if component == 0 {
            let template = numberLabels[row]
            let label = view as? UILabel ?? UILabel(frame: frame)
            label.text = template.text
            label.font = template.font
            label.numberOfLines = 1
            return label
        } else {
            // sized to the image, which may be smaller than the crop rect
            let image = suitImages[row]
            let imageView = view as? UIImageView ?? UIImageView(frame: CGRect(origin: .zero, size: image.size))
            imageView.image = image
            return imageView
        }
    }

    func pickerView(_ pickerView: UIPickerView, didSelectRow row: Int, inComponent component: Int) {
        var card = game.hand.card(at: selectedView.index)
        if component == 0 {
            card.number = row + 2
        } else {
            card.suit = Suit(rawValue: row)!
        }
        selectedView.setNeedsDisplay()
        game.setCard(card, at: selectedView.index)
        cardsDidChange()
    }
}

extension SolveViewController: CardTableDataSource, CardTableDelegate {
    func cardViewWasPressed(_ cardView: CardView) {
        selectedView = cardView
        setPicker(to: cardView.card)
    }

    func card(at index: Int) -> Card {
        game.hand.card(at: index)
    }

    func isHeld(_ card: Card) -> Bool {
        false
    }
}

extension SolveViewController: ReturnViewDelegate {
    func returnViewDetailButtonWasPressed() {
        showHandDetails()
    }
}
