/* 
 * CardView.mm
 *
 *
 */

#include "Platform.h"
#import "CardView.h"
#import "Deck.h"
#import "CardTableViewController.h"
//#import "GameModel.h"
//#import "VideoPokerAppDelegate.h"

@implementation CardView

@synthesize cardTable;

- (void) drawRect:(CGRect) rect
{
//    CGImageRef cardImage = [[VideoPokerAppDelegate app].game.deck getCardImage:[self getCard]];
    CGImageRef cardImage = NULL;
    
    Card_t card = [self getCard];
    bool held = false;
    if (card.getValue() == Card_t::Value::Undefined)
    {
        cardImage = [self.cardTable getBackImage:[self getIndex]];
    }
    else
    {
        cardImage = [self.cardTable getCardImage:card];
        held = [self.cardTable isHeldCard:card];
    }
    
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextTranslateCTM(context, 0.0, rect.size.height);
    CGContextScaleCTM(context, 1.0, -1.0);
    CGContextDrawImage(context, rect, cardImage);
    if (held)
    {
        CGImageRef heldImage = [CardView getHeldImage];
//        NSInteger border = 10;
        NSInteger x = (rect.size.width - CGImageGetWidth(heldImage)) / 2;
        CGRect heldRect = CGRectMake(x,
                                     10,
                                     x + CGImageGetWidth(heldImage),
                                     10 + CGImageGetHeight(heldImage));
                                     
        CGContextDrawImage(context, heldRect, heldImage);
    }
}

- (Card_t) getCard
{
    return [self.cardTable getCard:[self getIndex]];
}

- (NSInteger) getIndex
{
    return self.tag;
}

+ (CGImageRef) getHeldImage
{
    static CGImageRef heldImage = NULL;
    
	if (NULL == heldImage)
	{
		heldImage = [Deck loadImage:@"held.png"];
	}
    
	return heldImage;
}


@end