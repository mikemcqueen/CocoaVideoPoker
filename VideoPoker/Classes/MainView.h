//
//  MainView.h
//  VideoPoker
//
//  Created by Mike McQueen on 8/1/09.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

@class CardTableView;
@class UIStrokeLabel;

@interface MainView :
    UIView
{
    UIView*         _payScheduleView;
    UIView*         _returnView;
    UIView*         _cardTableView;
    
    UIButton*       _dealButton;
    UIButton*       _holdBestButton;
    UIButton*       _betOneButton;
    UIButton*       _betMaxButton;
//    UIButton*       _denominationButton;
	UILabel*        _gameOverLabel;
	UILabel*        _handValueLabel;
	UILabel*        _payScheduleLabel;
    UILabel*        _betLabel;
    UIStrokeLabel*  _winLabel;
    UIStrokeLabel*  _creditsLabel;
    
}

@property(nonatomic, /*readonly,*/ retain) IBOutlet UIView*    payScheduleView;
@property(nonatomic, /*readonly,*/ retain) IBOutlet UIView*    returnView;
@property(nonatomic, /*readonly,*/ retain) IBOutlet UIView*    cardTableView;

@property(nonatomic, readonly, retain) IBOutlet UIButton*      dealButton;
@property(nonatomic, readonly, retain) IBOutlet UIButton*      holdBestButton;
@property(nonatomic, readonly, retain) IBOutlet UIButton*      betOneButton;
@property(nonatomic, readonly, retain) IBOutlet UIButton*      betMaxButton;
//@property(readonly, retain) IBOutlet UIButton*      denominationButton;
@property(nonatomic, readonly, retain) IBOutlet UILabel*       gameOverLabel;
@property(nonatomic, readonly, retain) IBOutlet UILabel*       handValueLabel;
@property(nonatomic, readonly, retain) IBOutlet UILabel*       payScheduleLabel;
@property(nonatomic, readonly, retain) IBOutlet UILabel*       betLabel;
@property(nonatomic, readonly, retain) IBOutlet UIStrokeLabel* winLabel;
@property(nonatomic, readonly, retain) IBOutlet UIStrokeLabel* creditsLabel;

- (void) setDealUI;
- (void) setDrawUI;
- (void) showWin: (uint32_t) payout;
- (void) showCash: (double) cash;

@end
