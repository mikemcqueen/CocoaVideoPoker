//
//  HistoryViewController.mm
//  VideoPoker
//
//  Created by Mike McQueen on 10/27/10.
//  Copyright 2010 Mike McQueen. All rights reserved.
//

#include "Platform.h"
#import "HistoryViewController.h"
#import "SessionDetailsViewController.h"
#import "GameModel.h"
#import "SessionMO.h"

#include "PaySchedule.hpp"

@implementation HistoryViewController

@synthesize tableView   = _tableView;
@synthesize tableCell   = _tableCell;
@synthesize game        = _game;

- (void)
viewDidLoad
{
    NSLog(@"HistoryView: viewDidLoad");
    [super viewDidLoad];

    // TODO: TEMPORARY
    /* Create and add Delete All right nav bar button */
    UIBarButtonItem* item = [[UIBarButtonItem alloc] initWithTitle:@"Delete all" style:UIBarButtonItemStyleDone
                                                            target:self action:@selector(askDeleteAll)];
    self.navigationItem.rightBarButtonItem = item;
    [item release];
                
    [self initFetchedResultsController];
}

- (void)
initFetchedResultsController
{
    NSFetchRequest* fetchRequest = [[NSFetchRequest alloc] init];
    NSEntityDescription* entityDesc = [NSEntityDescription entityForName:[SessionMO entityName] inManagedObjectContext:_game.context];
    [fetchRequest setEntity:entityDesc];
    [entityDesc release];

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

- (void)
viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
    [_fetchedResults release]; _fetchedResults = nil;
}

- (void)
didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

- (void)
dealloc
{
    _fetchedResults = nil;
    [super dealloc];
}

- (void)
viewWillAppear:(BOOL)animated
{
    NSLog(@"HistoryView: viewWillAppear");
    [super viewWillAppear:animated];

    [self fetchAndRedraw];
}
    
- (void)
fetchAndRedraw
{
    NSError* error;
    BOOL success = [_fetchedResults performFetch:&error];
    if (!success)
    {
        NSLog(@"HistoryView::viewDidLoad(): Unresolved error %@, %@", error, [error userInfo]);
        throw runtime_error("HistoryView::viewDidLoad(): !success");
    }
    [_tableView reloadData];
}

- (NSInteger)
numberofSectionsInTableView: (UITableView*) tableView
{
    return [[_fetchedResults sections] count];
}

- (NSInteger)
tableView: (UITableView*) table
numberOfRowsInSection: (NSInteger) section
{
    id <NSFetchedResultsSectionInfo> sectionInfo = [[_fetchedResults sections] objectAtIndex:section];
    return [sectionInfo numberOfObjects];
}

- (UITableViewCell*)
tableView: (UITableView*) tableView
cellForRowAtIndexPath: (NSIndexPath*) indexPath
{
    static NSString* cellIdentifier = @"HistoryTableCell";
    UITableViewCell* cell = [tableView dequeueReusableCellWithIdentifier:cellIdentifier];
    if (cell == nil)
    {
        [[NSBundle mainBundle] loadNibNamed:@"SessionTableCell" owner:self options:nil];
        cell = self.tableCell;
        self.tableCell = nil;
    }
    
    SessionMO* session = (SessionMO*)[_fetchedResults objectAtIndexPath:indexPath];
    
    UILabel* label;
    
    PaySchedule::Game::Id_t gameId = (PaySchedule::Game::Id_t)session.gameId.intValue; 
    
    label = (UILabel*)[cell viewWithTag:1];
    const string& gameName = PaySchedule::Manager_t::getGameName(gameId);
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

    // TODO: configer cell with data from MO
    return cell;
}

- (void)
tableView:(UITableView *)tableView
didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    [tableView deselectRowAtIndexPath:indexPath animated:NO];
    SessionDetailsViewController *detailsController = [[SessionDetailsViewController alloc] initWithNibName:@"SessionDetailsView" bundle:nil];
//    detailsController.selectedRegion = [regions objectAtIndex:indexPath.row];
    detailsController.game = _game;
    SessionMO* _session = (SessionMO*)[_fetchedResults objectAtIndexPath:indexPath];

    NSLog(@"didSelect: _session=%luu",(unsigned long) _session.retainCount);
    detailsController.session = _session;
    NSLog(@"didSelect: _session=%lu, details.session=%lu", _session.retainCount, detailsController.session.retainCount);
    
    [[self navigationController] pushViewController:detailsController animated:YES];
    [detailsController release];
}

- (IBAction)
askDeleteAll
{
    [_game askDeleteStores:self];
}

- (void)
alertView:(UIAlertView*)alertView
clickedButtonAtIndex:(NSInteger)index
{
    if (index > 0)
    {
        // TODO: doesn't work, need to invalidate/re-create current session & hand
        // [self deleteAll];
    }
}
        
- (void)
deleteAll
{
    [_game deleteStores];
    [_fetchedResults release];
    _fetchedResults = nil;
    [self initFetchedResultsController];
    [self fetchAndRedraw];
}

@end
