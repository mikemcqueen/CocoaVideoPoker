//
//  FlipsideViewController.h
//  VideoPoker
//
//  Created by Mike McQueen on 8/1/09.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

@protocol FlipsideViewControllerDelegate;


@interface FlipsideViewController :
    UITableViewController<UITableViewDelegate,
                          UITableViewDataSource>
{
    UITableViewCell* _trainMode;
    UITableViewCell* _showReturn;
    UITableViewCell* _showHoldBest;
    UITableViewCell* _gameSpeed;
    UITableViewCell* _autoAddFunds;
    
	id <FlipsideViewControllerDelegate> delegate;
}

@property (nonatomic, assign) id <FlipsideViewControllerDelegate> delegate;
@property (nonatomic, readonly, retain) IBOutlet UITableViewCell* trainMode;
@property (nonatomic, readonly, retain) IBOutlet UITableViewCell* showReturn;
@property (nonatomic, readonly, retain) IBOutlet UITableViewCell* showHoldBest;
@property (nonatomic, readonly, retain) IBOutlet UITableViewCell* gameSpeed;
@property (nonatomic, readonly, retain) IBOutlet UITableViewCell* autoAddFunds;

- (IBAction) done;
- (IBAction) trainModeChanged;
- (IBAction) showReturnChanged;
- (IBAction) showHoldBestChanged;
- (IBAction) gameSpeedChanged;
- (IBAction) autoAddFundsChanged;

@end


@protocol FlipsideViewControllerDelegate

- (void) flipsideViewControllerDidFinish: (FlipsideViewController *) controller;

@end

