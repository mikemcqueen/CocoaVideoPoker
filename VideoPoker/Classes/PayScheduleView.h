//
//  PayScheduleView.h
//  VideoPoker
//
//  Created by Mike McQueen on 9/4/10.
//  Copyright 2010 Mike McQueen. All rights reserved.
//

#include "Hand.hpp"
#include "PayScheduleTypes.hpp"
//class PaySchedule_t;

@protocol PayScheduleViewDelegate;

@interface PayScheduleView :
    UIView 
{
    id<PayScheduleViewDelegate> _delegate;
}

@property (nonatomic, assign) IBOutlet id<PayScheduleViewDelegate> delegate;

+ (CGFloat) getHeightForSchedule: (const PaySchedule::Lookup::Data_t&) schedule;

@end

@protocol PayScheduleViewDelegate

- (const PaySchedule::Lookup::Data_t&) getPaySchedule;

@optional

- (uint32_t) getBet;
- (Hand::Value_t) getHandValue;

@end

