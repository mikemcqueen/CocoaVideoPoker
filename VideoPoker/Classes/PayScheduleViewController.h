//
//  PayScheduleViewController.h
//  VideoPoker
//
//  Created by Mike McQueen on 8/26/10.
//  Copyright 2010 Mike McQueen. All rights reserved.
//

#import "PayScheduleView.h"
#import "GameModel.h"

#include "PaySchedule.hpp"

class PaySchedule_t;

@interface PayScheduleViewController :
    UIViewController<PayScheduleViewDelegate,
                     GameModelContainer>
{
    id<GameModelContainer>  _gameContainer;
    PayScheduleView*        _payScheduleView;
    
    const PaySchedule::Lookup::Data_t*    _paySchedule;
}

@property (nonatomic, retain)   IBOutlet id<GameModelContainer>   gameContainer;
@property (nonatomic, retain)   IBOutlet PayScheduleView*         payScheduleView;
@property (nonatomic, readonly, retain)  GameModel*               game;

- (id) initWithPaySchedule: (const PaySchedule::Lookup::Data_t*) paySchedule;
- (void) payScheduleDidChange;

@end
