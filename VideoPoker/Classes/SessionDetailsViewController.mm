//
//  SessionDetailsViewController.mm
//  VideoPoker
//
//  Created by Mike McQueen on 11/18/10.
//  Copyright 2010 Mike McQueen. All rights reserved.
//

#include "Platform.h"
#import "SessionDetailsViewController.h"
#import "GameModel.h"
#import "SessionMO.h"
#import "HandMO.h"
#import "HandDataMO.h"
//#include "PayScheduleId.hpp"
//#include "PaySchedule.hpp"

@implementation SessionDetailsViewController

@synthesize session     = _session;
@synthesize tableView   = _tableView;
@synthesize tableCell   = _tableCell;
@synthesize game        = _game;

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)
 initWithNibName:(NSString *)nibNameOrNil
 bundle:(NSBundle *)nibBundleOrNil
 {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}
*/

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)
viewDidLoad
{
//    NSLog(@"SessionDetailsView: viewDidLoad, session=%d,context=%d", _session.retainCount, _game.context.retainCount);
    [super viewDidLoad];
    
    [self initFetchedResultsController];
}

- (void)
didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

- (void)
viewDidUnload
{
    [_tableView release];
    
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
    
//    NSLog(@"SessionDetailsView::viewDidUnload: session=%d, context=%d", _session.retainCount, _game.context.retainCount);
}

- (void)
dealloc
{
    [_tableView release]; _tableView = nil;
    
//    NSLog(@"SessionDetailsView::dealloc1: session=%d, context=%d", _session.retainCount, _game.context.retainCount);
    
    [_fetchedResults release]; _fetchedResults = nil;
    self.session = nil;

//    NSLog(@"SessionDetailsView::dealloc2: session=%d, context=%d", _session.retainCount, _game.context.retainCount);
    
    [super dealloc];
}

- (void)
viewWillAppear:(BOOL) animated
{
    NSLog(@"SessionDetailsView: viewWillAppear");
    
    [super viewWillAppear:animated];
    
    NSError* error;
    BOOL success = [_fetchedResults performFetch:&error];
    if (!success)
    {
        NSLog(@"SessionDetailsView::viewWillAppear(): Unresolved error %@, %@", error, [error userInfo]);
        throw runtime_error("SessionDetailsView::viewWillAppear(): !success");
    }
    [_tableView reloadData];
}

- (void)
initFetchedResultsController
{
    NSFetchRequest* fetchRequest = [[NSFetchRequest alloc] init];
    [fetchRequest setEntity:[NSEntityDescription entityForName:[HandMO entityName] inManagedObjectContext:_game.context]];
    [fetchRequest setPredicate:[NSPredicate predicateWithFormat:@"session == %@", _session]];
    
    NSSortDescriptor* sortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"startDate" ascending:NO];
    NSArray* sortDescriptorArray = [[NSArray alloc] initWithObjects:sortDescriptor, nil];
    [fetchRequest setSortDescriptors:sortDescriptorArray];
    [sortDescriptorArray release];
    [sortDescriptor release];
    
    _fetchedResults = [[NSFetchedResultsController alloc]
                       initWithFetchRequest:fetchRequest
                       managedObjectContext:_game.context
                       sectionNameKeyPath:nil
                       cacheName:nil]; // TODO: cache?
    
    [fetchRequest release];
}

- (NSInteger)
numberofSectionsInTableView:(UITableView*)tableView
{
    return [[_fetchedResults sections] count];
}

- (NSInteger)
tableView: (UITableView*) table
numberOfRowsInSection:(NSInteger)section
{
    id <NSFetchedResultsSectionInfo> sectionInfo = [[_fetchedResults sections] objectAtIndex:section];
    return [sectionInfo numberOfObjects];
}

- (void)
tableView:(UITableView*)tableView
didSelectRowAtIndexPath:(NSIndexPath*)indexPath
{
    [tableView deselectRowAtIndexPath:indexPath animated:NO];
    /*
    BATTrailsViewController *trailsController = [[BATTrailsViewController alloc] initWithStyle:UITableViewStylePlain];
    trailsController.selectedRegion = [regions objectAtIndex:indexPath.row];
    [[self navigationController] pushViewController:trailsController animated:YES];
    [trailsController release];
     */
}

- (UITableViewCell*)
tableView: (UITableView*) tableView
cellForRowAtIndexPath: (NSIndexPath*) indexPath
{
    static NSString* cellIdentifier = @"HandTableCell";
    UITableViewCell* cell = [tableView dequeueReusableCellWithIdentifier:cellIdentifier];
    if (cell == nil)
    {
        [[NSBundle mainBundle] loadNibNamed:@"HandHistoryTable" owner:self options:nil];
        cell = self.tableCell;
        self.tableCell = nil;
    }
    
/*
    HandMO* hand = (HandMO*)[_fetchedResults objectAtIndexPath:indexPath];
    
    UILabel* label;
    
    PaySchedule::Game::Id_t gameId = (PaySchedule::Game::Id_t)session.gameId.intValue; 
    
    label = (UILabel*)[cell viewWithTag:1];
    const string& gameName = PaySchedule_t::getGameName(gameId);
    label.text = gameName.empty() ? @"Error" : [NSString stringWithUTF8String:gameName.c_str()];
    
#if 0
    label = (UILabel*)[cell viewWithTag:2];
    label.text = [NSString stringWithFormat:@"%d", session.handCount.intValue];
#endif
    label = (UILabel*)[cell viewWithTag:2];
    NSDateFormatter* formatter = [[NSDateFormatter alloc] init];
//    [formatter setDateFormat:@"mm/dd/yyyy"];
    [formatter setDateStyle:NSDateFormatterMediumStyle];
    [formatter setTimeStyle:NSDateFormatterShortStyle];
    label.text = [formatter stringFromDate:session.startDate];
    [formatter release];
    
    label = (UILabel*)[cell viewWithTag:3];
    label.text = [NSString stringWithFormat:@"%d", session.perfectPlayCountValue];

    label = (UILabel*)[cell viewWithTag:4];
    label.text = [NSString stringWithFormat:@"%d", session.handCountValue];
 */

    // TODO: configer cell with data from MO
    return cell;
}


@end
