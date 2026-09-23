//
//  VideoPokerUITests.swift
//  VideoPokerUITests
//

import XCTest

final class VideoPokerUITests: XCTestCase {
    private var app: XCUIApplication!

    override func setUp() {
        continueAfterFailure = false
        app = XCUIApplication()
        app.launch()
    }

    /// Saves a screenshot as a test attachment (and to $SCREENSHOT_DIR if set)
    private func screenshot(_ name: String) {
        let shot = XCUIScreen.main.screenshot()
        let attachment = XCTAttachment(screenshot: shot)
        attachment.name = name
        attachment.lifetime = .keepAlways
        add(attachment)
        if let dir = ProcessInfo.processInfo.environment["SCREENSHOT_DIR"] {
            try? shot.pngRepresentation.write(to: URL(fileURLWithPath: dir).appendingPathComponent("\(name).png"))
        }
    }

    private func cashLabel() -> XCUIElement {
        app.staticTexts.matching(NSPredicate(format: "label BEGINSWITH 'CASH'")).firstMatch
    }

    func testDealHoldDraw() {
        let deal = app.buttons["DEAL"]
        XCTAssertTrue(deal.waitForExistence(timeout: 10))
        screenshot("01-train")

        // Bet one cycles 5 -> 1
        app.buttons["BET 1"].tap()
        XCTAssertTrue(app.staticTexts["BET 1"].exists)
        for bet in 2...5 {
            app.buttons["BET 1"].tap()
            XCTAssertTrue(app.staticTexts["BET \(bet)"].exists)
        }

        deal.tap()
        let draw = app.buttons["DRAW"]
        XCTAssertTrue(draw.waitForExistence(timeout: 5))
        XCTAssertTrue(app.buttons["HOLD BEST"].exists || app.buttons.matching(NSPredicate(format: "label CONTAINS[c] 'best'")).count > 0)
        XCTAssertTrue(cashLabel().exists)
        screenshot("02-dealt")

        app.otherElements["card0"].tap()
        app.otherElements["card2"].tap()
        screenshot("03-held-two")

        app.buttons.matching(NSPredicate(format: "label CONTAINS[c] 'best'")).firstMatch.tap()
        screenshot("04-hold-best")

        // Hand details
        app.buttons["handDetails"].tap()
        XCTAssertTrue(app.segmentedControls.buttons["Best"].waitForExistence(timeout: 5))
        screenshot("05-hand-details-your")
        app.segmentedControls.buttons["Best"].tap()
        screenshot("06-hand-details-best")
        app.navigationBars.buttons.element(boundBy: 0).tap()

        XCTAssertTrue(draw.waitForExistence(timeout: 5))
        draw.tap()
        XCTAssertTrue(deal.waitForExistence(timeout: 5))
        screenshot("07-drawn")

        // Play a few more hands
        for _ in 0..<5 {
            deal.tap()
            XCTAssertTrue(draw.waitForExistence(timeout: 5))
            app.buttons.matching(NSPredicate(format: "label CONTAINS[c] 'best'")).firstMatch.tap()
            draw.tap()
            XCTAssertTrue(deal.waitForExistence(timeout: 5))
        }
        screenshot("08-after-hands")
    }

    func testChooseGameAndOptions() {
        XCTAssertTrue(app.buttons["DEAL"].waitForExistence(timeout: 10))

        // A session can't be saved until it has a hand (model validation), and
        // switching games saves the session, so play a hand first
        app.buttons["DEAL"].tap()
        app.buttons["DRAW"].tap()
        XCTAssertTrue(app.buttons["DEAL"].waitForExistence(timeout: 5))

        app.buttons["Choose Game"].tap()
        XCTAssertTrue(app.pickers.firstMatch.waitForExistence(timeout: 5))
        screenshot("10-choose-game")

        app.pickers.pickerWheels.firstMatch.adjust(toPickerWheelValue: "Deuces Wild")
        screenshot("11-choose-deuces")

        app.pickers.pickerWheels.firstMatch.adjust(toPickerWheelValue: "Jacks or Better")
        let page2 = app.staticTexts["Jacks or Better - 99.54%"]
        app.scrollViews.firstMatch.swipeLeft()
        if !page2.waitForExistence(timeout: 3) {
            // tapping right of the current page dot goes to the next page
            app.pageIndicators.firstMatch.coordinate(withNormalizedOffset: CGVector(dx: 0.9, dy: 0.5)).tap()
        }
        XCTAssertTrue(page2.waitForExistence(timeout: 5))
        screenshot("12-choose-jacks-page2")
        app.navigationBars.buttons["Done"].tap()
        XCTAssertTrue(app.buttons["DEAL"].waitForExistence(timeout: 5))
        screenshot("13-switched-schedule")

        app.navigationBars.buttons.element(boundBy: 0).tap()
        XCTAssertTrue(app.navigationBars["Options"].waitForExistence(timeout: 5))
        screenshot("14-options")

        // Switch to the classic graphics and back
        let classic = app.switches["classicGraphics"]
        XCTAssertEqual(classic.value as? String, "0")
        classic.tap()
        app.navigationBars["Options"].buttons["Done"].tap()
        XCTAssertTrue(app.buttons["DEAL"].waitForExistence(timeout: 5))
        screenshot("15-classic-graphics")

        app.navigationBars.buttons.element(boundBy: 0).tap()
        XCTAssertEqual(classic.value as? String, "1")
        classic.tap()
        app.navigationBars["Options"].buttons["Done"].tap()
        XCTAssertTrue(app.buttons["DEAL"].waitForExistence(timeout: 5))
    }

    func testSolveAndHistory() {
        XCTAssertTrue(app.buttons["DEAL"].waitForExistence(timeout: 10))

        app.tabBars.buttons["Solve"].tap()
        XCTAssertTrue(app.pickers.firstMatch.waitForExistence(timeout: 5))
        screenshot("20-solve")

        // Change the first card (10 of spades) to a 3 of hearts
        app.otherElements["card0"].tap()
        app.pickers.pickerWheels.element(boundBy: 0).adjust(toPickerWheelValue: "3")
        screenshot("21-solve-changed")

        // A duplicate card is not a valid hand
        app.otherElements["card1"].tap()
        app.pickers.pickerWheels.element(boundBy: 0).adjust(toPickerWheelValue: "3")
        XCTAssertTrue(app.staticTexts["That is not a valid hand"].waitForExistence(timeout: 5))
        screenshot("22-solve-invalid")

        app.tabBars.buttons["History"].tap()
        XCTAssertTrue(app.tables.firstMatch.waitForExistence(timeout: 5))
        screenshot("30-history")
        if app.tables.cells.count > 0 {
            app.tables.cells.element(boundBy: 0).tap()
            screenshot("31-session-details")
        }

        app.tabBars.buttons["Train"].tap()
        XCTAssertTrue(app.buttons["DEAL"].waitForExistence(timeout: 5))
    }
}
