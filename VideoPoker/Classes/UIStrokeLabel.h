//
//  UIStrokeLabel.h
//  VideoPoker
//
//  Created by Mike McQueen on 10/9/10.
//  Copyright 2010 Mike McQueen. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface UIStrokeLabel :
    UILabel
{
    UIColor*  _strokeColor;
    NSNumber* _strokeWidth;
}

@property (nonatomic, retain) IBOutlet UIColor*  strokeColor;
@property (nonatomic, retain) IBOutlet NSNumber* strokeWidth;

- (void) drawTextInRect: (CGRect) rect;

@end
