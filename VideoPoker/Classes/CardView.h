#include "Card.hpp"

@class CardTableViewController;

@interface CardView : UIControl
{
	IBOutlet CardTableViewController* cardTable;
}

@property (nonatomic, readonly, assign) CardTableViewController* cardTable;

- (void) drawRect:(CGRect) rect;
- (Card_t) getCard;
- (NSInteger) getIndex;

+ (CGImageRef) getHeldImage;

@end
