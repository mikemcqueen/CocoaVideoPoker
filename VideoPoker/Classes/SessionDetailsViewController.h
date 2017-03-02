//
//  SessionDetailsViewController.h
//  VideoPoker
//
//  Created by Mike McQueen on 11/18/10.
//  Copyright 2010 Mike McQueen. All rights reserved.
//

#import <UIKit/UIKit.h>

@class GameModel;
@class SessionMO;

@interface SessionDetailsViewController :
    UIViewController<UITableViewDelegate,
                    UITableViewDataSource>
{
    SessionMO*                  _session;
    UITableView*                _tableView;
    UITableViewCell*            _tableCell;
    NSFetchedResultsController* _fetchedResults;
    GameModel*                  _game;
}

@property (nonatomic, retain)                    SessionMO*         session;
@property (nonatomic, readonly, retain) IBOutlet UITableView*       tableView;
@property (nonatomic, retain)           IBOutlet UITableViewCell*   tableCell;
@property (nonatomic, retain)           IBOutlet GameModel*         game;

- (void) initFetchedResultsController;

@end
