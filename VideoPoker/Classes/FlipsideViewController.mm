//
//  FlipsideViewController.m
//  VideoPoker
//
//  Created by Mike McQueen on 8/1/09.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#include "Platform.h"

#import "FlipsideViewController.h"


@implementation FlipsideViewController

@synthesize trainMode       = _trainMode;
@synthesize showReturn      = _showReturn;
@synthesize showHoldBest    = _showHoldBest;
@synthesize gameSpeed       = _gameSpeed;
@synthesize autoAddFunds    = _autoAddFunds;

@synthesize delegate;


- (void)viewDidLoad
{
    [super viewDidLoad];
    //self.view.backgroundColor = [UIColor viewFlipsideBackgroundColor];     
    
    /* Create and add Choose Game right nav bar button */
    UIBarButtonItem* doneItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone
                                                        target:self action:@selector(done)];
    self.navigationItem.rightBarButtonItem = doneItem;
    self.navigationItem.title = @"Options";
    [doneItem release];
}


- (IBAction)
done
{
	[self.delegate flipsideViewControllerDidFinish:self];	
}

- (IBAction) trainModeChanged
{
    
}
- (IBAction) showReturnChanged
{
    
}
- (IBAction) showHoldBestChanged
{
    
}
- (IBAction) gameSpeedChanged
{
    
}
- (IBAction) autoAddFundsChanged
{
    
}

/*
 // Override to allow orientations other than the default portrait orientation.
 - (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
 // Return YES for supported orientations
 return (interfaceOrientation == UIInterfaceOrientationPortrait);
 }
 */

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [super dealloc];
}

#pragma mark -
#pragma mark Table view data source and delegate

- (NSInteger)
numberOfSectionsInTableView: (UITableView *) tableView
{
    return 2;
}

- (NSInteger)
tableView: (UITableView *) tableView
numberOfRowsInSection: (NSInteger) section
{
    switch (section)
    {
        case 0: return 3;
        case 1: return 2;
        // case 2:
        default:
            throw invalid_argument("tableView: numberOfRowsInSection: bad section");
    }
}

- (UITableViewCell *)
tableView: (UITableView *) tableView
cellForRowAtIndexPath: (NSIndexPath *) indexPath
{
    switch (indexPath.section)
    {
    case 0:
        switch (indexPath.row)
        {
        case 0: return _trainMode;
        case 1: return _showReturn;
        case 2: return _showHoldBest;
        default:
            throw invalid_argument("tableView: cellForRowAtIndexPath: bad row section 0");
        }
    case 1:
        switch (indexPath.row)
        {
        case 0: return _gameSpeed;
        case 1: return _autoAddFunds;
        default:
            throw invalid_argument("tableView: cellForRowAtIndexPath: bad row section 1");
        }
    default:
        throw invalid_argument("tableView: cellForRowAtIndexPath: bad section");
    }
}

- (NSString*)
tableView: (UITableView*) tableView
titleForHeaderInSection: (NSInteger) section
{
    switch (section)
    {
        case 0: return @"Train";
        case 1: return @"Game";
        default:
            throw invalid_argument("tableView: titleForFooterInSection: bad section");
    }
}

- (void)
tableView: (UITableView*) tableView
didSelectRowAtIndexPath: (NSIndexPath*) indexPath
{
    NSLog(@"DidSelect Row %ld", (long)(long)indexPath.row);
}

@end
