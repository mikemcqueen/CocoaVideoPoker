//
//  UIStrokeLabel.m
//  VideoPoker
//
//  Created by Mike McQueen on 10/9/10.
//  Copyright 2010 Mike McQueen. All rights reserved.
//

#import "UIStrokeLabel.h"


@implementation UIStrokeLabel

@synthesize strokeColor = _strokeColor;
@synthesize strokeWidth = _strokeWidth;

- (id)
init{
    return self;
}

- (id)
initWithFrame: (CGRect) frame
{
    if ((self = [super initWithFrame:frame]))
    {
        _strokeWidth = [NSNumber numberWithInt: 1];
        _strokeColor = [UIColor whiteColor];
    }
    return self;
}

- (void)
dealloc
{
//    [_strokeColor dealloc];
    [super dealloc];
}

- (void)
drawTextInRect: (CGRect) rect
{   
    CGSize shadowOffset = self.shadowOffset;
//    UIColor *textColor = self.textColor;
    
    CGContextRef ctx = UIGraphicsGetCurrentContext();
    CGContextSetCharacterSpacing(ctx, 1.5);
    CGContextSetLineWidth(ctx, [self.strokeWidth intValue]);
    CGContextSetTextDrawingMode(ctx, kCGTextStroke);
    
    CGContextSetFillColorWithColor(ctx, self.textColor.CGColor);
//    CGContextSetFillColorWithColor(ctx, [UIColor redColor].CGColor);
    CGContextSetStrokeColorWithColor(ctx, self.strokeColor.CGColor);

//    rect.origin.x += 2;

#if 1

    // TODO: text alignment, fudge factor
    
    //    CGContextSetTextMatrix(ctx, CGAffineTransformMakeScale(1.0, -1.0));
    CGContextTranslateCTM(ctx, 0, self.bounds.size.height);
    CGContextScaleCTM(ctx, 1, -1);
    
    CGContextSetFont(ctx, CGFontCreateWithFontName((CFStringRef)self.font.fontName));
    CGContextSetFontSize(ctx, self.font.pointSize);
    CGContextSelectFont(ctx, "Helvetica", 14, kCGEncodingMacRoman);
    CGContextShowTextAtPoint(ctx, 1, 5, [self.text UTF8String], self.text.length);
    CGContextSetTextDrawingMode(ctx, kCGTextFill);
    CGContextShowTextAtPoint(ctx, 1, 5, [self.text UTF8String], self.text.length);
#else
    [self.text drawInRect: rect withFont: self.font lineBreakMode: self.lineBreakMode alignment: self.textAlignment];
#endif
    CGContextSetTextDrawingMode(ctx, kCGTextFill);
    //    [self.text drawInRect: rect withFont: self.font lineBreakMode: self.lineBreakMode alignment: self.textAlignment];
    
#if 0
    self.textColor = self.strokeColor;
    [super drawTextInRect: rect];    
   
    CGContextSetCharacterSpacing(ctx,55.0);
    CGContextSetTextDrawingMode(ctx, kCGTextFill);
    self.textColor = textColor;
    self.shadowOffset = CGSizeMake(0, 0);
    [super drawTextInRect: rect];
#endif
    self.shadowOffset = shadowOffset;
}

@end

