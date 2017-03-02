//
//  PayScheduleViewController.mm
//  VideoPoker
//
//  Created by Mike McQueen on 8/26/10.
//  Copyright 2010 Mike McQueen. All rights reserved.
//


#include "Platform.h"

#import "PayScheduleViewController.h"

#include "PaySchedule.hpp"
#include "PaySchedule.hpp"

@implementation PayScheduleViewController

@synthesize payScheduleView = _payScheduleView;
@synthesize gameContainer   = _gameContainer;

- (void)
dealloc
{
    [super dealloc];
}

// init for when embedded in trainerView
- (id)
initWithNibName: (NSString *) nibName
bundle:          (NSBundle *) bundle
{
    if (self = [super initWithNibName:nibName bundle:bundle])
	{
        _paySchedule = NULL;
    }
    return self;
}

// init for when presented modally in chooseGameView
- (id)
initWithPaySchedule: (const PaySchedule::Lookup::Data_t*) paySchedule
{
    if (self = [super initWithNibName:@"PayScheduleView" bundle:nil])
    {
        _paySchedule = paySchedule;
        [self payScheduleDidChange];
    }
    return self;
}

- (void)
payScheduleDidChange
{
    const PaySchedule::Lookup::Data_t& schedule = [self getPaySchedule];
    CGRect frame = self.payScheduleView.frame;
    frame.size.height = [PayScheduleView getHeightForSchedule: schedule];
    self.payScheduleView.frame = frame;
    [self.payScheduleView setNeedsDisplay];
}

- (void)
viewDidLoad
{
    [super viewDidLoad];
}

- (void)
viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

////////////////////////////////////////////////////////////////////////////////

- (const PaySchedule::Lookup::Data_t&)
getPaySchedule
{
    const PaySchedule::Lookup::Data_t* paySchedule = _paySchedule;
    
    if (NULL == paySchedule)
    {
        paySchedule = self.game.schedule;
    }
    
    return *paySchedule;
}

- (Hand::Value_t)
getHandValue
{
    // No hand value highlight in modal select-schedule
    if (NULL != _paySchedule)
    {
        return 0;
    }
    return self.game.handValue;
}

- (uint32_t)
getBet
{
    // No bet highlight in modal select-schedule
    if (NULL != _paySchedule)
    {
        return 0;
    }
    return self.game.bet;
}

////////////////////////////////////////////////////////////////////////////////

- (GameModel*)
game
{
    return [self getGameModel];
}

- (GameModel*)
getGameModel
{
    return [_gameContainer getGameModel];
}

////////////////////////////////////////////////////////////////////////////////
    
@end
