//
//  HandDetailsViewController.h
//  VideoPoker
//
//  Created by Mike McQueen on 7/19/10.
//  Copyright 2010 Amazon.com. All rights reserved.
//

#import "CardTableViewController.h"

@class GameModel;
    //@class HandDetailsTableViewController;
@class CardTableViewController;
    //@class UITableViewCell

@interface HandDetailsViewController :
    UIViewController<
        UITableViewDelegate,
        UITableViewDataSource,
        CardTableViewControllerDelegate,
        CardTableViewControllerDataSource>
{
    CardTableViewController* cardTable;
    UITableView*             detailTableView;
    UITableViewCell*         tableCell;
    UIView*                  tableHeader;
    GameModel*               game;
}

@property (nonatomic, retain) IBOutlet CardTableViewController* cardTable;
@property (nonatomic, retain) IBOutlet UITableView*             detailTableView;
@property (nonatomic, retain) IBOutlet UITableViewCell*         tableCell;
@property (nonatomic, retain) IBOutlet UIView*                  tableHeader;
@property (nonatomic, retain) IBOutlet GameModel*               game;

- (void) selectHand;
- (UISegmentedControl*) getTitle;
- (const CardVector_t&) getHeldCards;

@end
