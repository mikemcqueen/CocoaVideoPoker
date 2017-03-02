//
//  HistoryViewController.h
//  VideoPoker
//
//  Created by Mike McQueen on 10/27/10.
//  Copyright 2010 Mike McQueen. All rights reserved.
//

#import <UIKit/UIKit.h>

@class GameModel;

@interface HistoryViewController :
    UIViewController<UITableViewDelegate,
                    UITableViewDataSource,
                    UIAlertViewDelegate>
{
    UITableView*                _tableView;
    UITableViewCell*            _tableCell;
    NSFetchedResultsController* _fetchedResults;
    GameModel*                  _game;
}

@property (nonatomic, readonly, retain) IBOutlet UITableView*       tableView;
@property (nonatomic, retain)           IBOutlet UITableViewCell*   tableCell;
@property (nonatomic, retain)           IBOutlet GameModel*         game;

- (void) initFetchedResultsController;
- (void) fetchAndRedraw;

- (IBAction) askDeleteAll;
- (void) deleteAll;

@end
