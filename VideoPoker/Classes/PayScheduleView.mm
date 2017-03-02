//
//  PayScheduleView.mm
//  VideoPoker
//
//  Created by Mike McQueen on 9/4/10.
//  Copyright 2010 Mike McQueen. All rights reserved.
//

#include "Platform.h"

#import "PayScheduleView.h"

#include "PaySchedule.hpp"
#include "Hand.hpp"

static const CGFloat borderWidth = 2.0;
static const CGFloat textSpacing = 2.0;
static const CGFloat lineHeight  = 10.0; // TODO: font.height

@implementation PayScheduleView

@synthesize delegate = _delegate;

+ (CGFloat)
getHeightForSchedule: (const PaySchedule::Lookup::Data_t&) schedule
{
    return schedule.getPaylineCount() * lineHeight + borderWidth * 2.0 + 3.0; // textSpacing * 2.0;
}

- (id)
initWithFrame: (CGRect) frame
{
    if ((self = [super initWithFrame:frame])) 
    {
        // Initialization code
    }
    return self;
}

- (void)
dealloc
{
    [super dealloc];
}

 - (void)
drawRect: (CGRect) rect
{
	CGContextRef ctx = UIGraphicsGetCurrentContext();
    
    rect = self.bounds;

    const int divisor = 10;
    const int columnCount = 6;
    int columnExtra = 0;

    CGFloat unitWidth = CGRectGetWidth(rect) / divisor;
    int extra = CGRectGetWidth(rect) - (divisor * unitWidth);

    if (extra >= columnCount)
    {
        columnExtra = extra / columnCount;
        extra %= columnCount;
    }

    
    // Yellow / Dark blue
    CGContextSetRGBStrokeColor(ctx, 1.0, 1.0, 0.0, 1.0);
    CGContextSetRGBFillColor(ctx, 0.0, 0.0, 0.5, 1.0);
    
    // Fill/Stroke outer rect
    CGContextFillRect(ctx, rect);
    CGContextStrokeRectWithWidth(ctx, rect, borderWidth); // TODO: borderWidth

    typedef pair<CGFloat, CGFloat> OffsetWidthPair_t;
    
    vector<OffsetWidthPair_t> columnOffsetWidths;
    columnOffsetWidths.reserve(columnCount);
    
    const size_t bet = [self.delegate getBet];
    
    // Stroke divider lines
    CGFloat x = rect.origin.x;
    for (int columnIndex = 0; columnIndex < columnCount; ++columnIndex)
    {
        CGFloat columnWidth;
        switch (columnIndex)
        {
            case 0:
                columnWidth = 4.125 * unitWidth + columnExtra + extra / 2.0;
                break;
                
            default: 
                if (columnIndex < columnCount - 1)
                {
                    columnWidth = unitWidth + columnExtra;                    
                }
                else
                {
                    columnWidth = CGRectGetWidth(rect) - x;
                }
                break;
        }
        columnOffsetWidths.push_back(make_pair(x, columnWidth));
        x += columnWidth;
        
        CGContextStrokeRectWithWidth(ctx, CGRectMake(x, rect.origin.y, 0.0, CGRectGetHeight(rect)), borderWidth);
        
        if ((bet > 0) && (bet == columnIndex))
        {
//            CGContextSetRGBFillColor(ctx, 1.0, 0.0, 0.0, 1.0);
            CGContextSetRGBFillColor(ctx, 0.7, 0.0, 0.0, 1.0);
            CGContextFillRect(ctx, CGRectInset(CGRectMake(x - columnWidth, rect.origin.y, columnWidth, CGRectGetHeight(rect)), 1, 1));
        }
    }
    
    UIFont* font = [UIFont fontWithName: @"Helvetica-Bold" size: 10.0];
    UIFont* smallFont = [UIFont fontWithName: @"Helvetica-Bold" size: 9.0];
    
    ///CGFloat height = font.capHeight;

    CGContextSetRGBFillColor(ctx, 1.0, 1.0, 0.0, 1.0);

    CGPoint point = CGPointMake(columnOffsetWidths[0].first + borderWidth + textSpacing, borderWidth );//+ 1.0);
    CGFloat maxWidth = columnOffsetWidths[0].second - borderWidth * 2.0 - textSpacing * 2.0;

    const PaySchedule::Lookup::Data_t& schedule = [self.delegate getPaySchedule];
    
    for (int payLineIndex = 0; payLineIndex < schedule.getPaylineCount(); ++payLineIndex)
    {
        const string& handName = Hand::getName(schedule.getPaylineHandValue(payLineIndex));
        const NSString* text = [NSString stringWithUTF8String: handName.c_str()];

        CGSize textSize = [text sizeWithFont: font];
        bool small = false;
        if (textSize.width > maxWidth)
        {
            textSize = [text sizeWithFont: smallFont];
            small = true;
        }
        // Draw the hand name
        [text drawAtPoint: point withFont: small ? smallFont : font];

        // Draw dots after name to fill in the column
        CGPoint dotOffset = CGPointMake(textSize.width + 1.0, lineHeight / 2.0 + 1);
        while (dotOffset.x <= maxWidth)
        {
            CGContextFillRect(ctx, CGRectMake(point.x + dotOffset.x, point.y + dotOffset.y, 1.0, 1.0));
            dotOffset.x += 2;
        }
        
        for (int columnIndex = 1; columnIndex < columnCount; ++columnIndex)
        {
            OffsetWidthPair_t& offsetWidth = columnOffsetWidths[columnIndex];
            CGRect payoutRect = CGRectMake(offsetWidth.first, point.y, offsetWidth.second, lineHeight);
            payoutRect = CGRectInset(payoutRect, textSpacing, 0.0);
            size_t payout = schedule.getPayout(payLineIndex, columnIndex);
            NSString* payoutText;
            if (columnIndex == columnCount - 1 && 4000 == payout)
            {
                // TODO: 4000 is hack; schedule.jackpot?
                payoutText = [NSString stringWithFormat: @"$%6.2f", double(payout)];
            }
            else
            {
                payoutText = [NSString stringWithFormat: @"%lu", payout];
            }
            NSDictionary* attr = @{
                NSFontAttributeName : font
                 /*lineBreakMode: UILineBreakModeClip*/
//                 alignment: UITextAlignmentRight
            };
            [payoutText drawInRect: payoutRect withAttributes: attr];
        }
        
        point.y += lineHeight;
    }
}

@end
