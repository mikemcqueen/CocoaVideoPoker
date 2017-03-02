
#import "Platform.h"
#import "CardTableView.h"
#import "CardView.h"

@implementation CardTableView

- (CardView*)
getCardView: (NSInteger) cardIndex
{
        // return [self viewWithTag: [NSString stringWithFormat: @"%d", index]
	switch (cardIndex)
	{
		case 0: return cardView1;
		case 1: return cardView2;
		case 2: return cardView3;
		case 3: return cardView4;
		case 4: return cardView5;
		default:
			NSLog(@"CardTableView::getCardView: Invalid cardIndex");
			return NULL;
	}
}

- (void)
redrawSubviews
{
    for (UIView* subView in [self subviews])
    {
        [subView setNeedsDisplay];
    }
}

@end
