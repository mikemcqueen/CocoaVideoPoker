#import "CardTableViewController.h"

@class CardView;
class CardVector_t;

@interface CardTableView :
    UIView
{
    CardTableViewController* cardTable;
    
	CardView* cardView1;
	CardView* cardView2;
	CardView* cardView3;
	CardView* cardView4;
	CardView* cardView5;
}

- (CardView*) getCardView: (NSInteger) cardIndex;
- (void) redrawSubviews;

@end
