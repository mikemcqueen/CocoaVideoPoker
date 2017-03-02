//
//  MainView.m
//  VideoPoker
//
//  Created by Mike McQueen on 8/1/09.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

// rename to .mm

//#include "Platform.h"

#import "MainView.h"
#import "UIStrokeLabel.h"

@implementation MainView

@synthesize payScheduleView     = _payScheduleView;
@synthesize returnView          = _returnView;
@synthesize cardTableView       = _cardTableView;

@synthesize dealButton          = _dealButton;
@synthesize holdBestButton      = _holdBestButton;
@synthesize betOneButton        = _betOneButton;
@synthesize betMaxButton        = _betMaxButton;
//@synthesize denominationButton      = _denominationButton;
@synthesize handValueLabel      = _handValueLabel;
@synthesize gameOverLabel       = _gameOverLabel;
@synthesize payScheduleLabel    = _payScheduleLabel;
@synthesize betLabel            = _betLabel;
@synthesize winLabel            = _winLabel;
@synthesize creditsLabel        = _creditsLabel;

- (void)
dealloc
{
    [super dealloc];
}

- (void)
layoutSubviews
{
    /* payScheduleLabel is placed correctly automatically */
    
    /* payScheduleView is placed immediately below payScheduleLabel */
    _payScheduleView.frame = CGRectOffset(_payScheduleView.bounds, 0.0, _payScheduleLabel.frame.origin.y + _payScheduleLabel.frame.size.height + 1);
	
    /* returnView is placed immediately below payScheduleView */
//    CGRect framePSV = _payScheduleView.frame;
//    CGRect boundsRV = _returnView.bounds;
    _returnView.frame = CGRectOffset(_returnView.bounds, 0.0, _payScheduleView.frame.origin.y + _payScheduleView.frame.size.height + 1);
    
    /* cardTableView is placed immediately below returnView */
//    CGRect frameRV = _returnView.frame;
    _cardTableView.frame = CGRectOffset(_cardTableView.bounds, 0.0, _returnView.frame.origin.y + _returnView.frame.size.height + 1);
    
    /* TODO: button layout here! */
}

- (void)
setDealUI
{
    [self.dealButton setTitle:@"DEAL" forState: UIControlStateNormal];
    self.gameOverLabel.hidden = false;
    self.holdBestButton.hidden = true;
    self.betOneButton.hidden = false;
    self.betMaxButton.hidden = false;
}

- (void)
setDrawUI
{
    [self.dealButton setTitle:@"DRAW" forState: UIControlStateNormal];
//    self.handValueLabel.text = nil;
    self.gameOverLabel.hidden = true;
    self.holdBestButton.hidden = false;
    self.betOneButton.hidden = true;
    self.betMaxButton.hidden = true;
    self.winLabel.hidden = true;
}

- (void)
showWin: (uint32_t) payout
{
    self.winLabel.text = [NSString stringWithFormat: @"WIN %d", payout];
    self.winLabel.hidden = false;
}

- (void)
showCash: (double) cash
{
    self.creditsLabel.text = [NSString stringWithFormat: @"CASH $%.2f", cash];
    self.creditsLabel.hidden = false;
}

/*
- (id)
initWithFrame: (CGRect) frame
{
    if (self = [super initWithFrame:frame]) 
	{
        // Initialization code
    }
    return self;
}

- (void)
drawRect: (CGRect) rect
{
    // Drawing code
}
*/

@end
