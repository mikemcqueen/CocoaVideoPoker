//
//  HandDetailsViewController.m
//  VideoPoker
//
//  Created by Mike McQueen on 7/19/10.
//  Copyright 2010 Amazon.com. All rights reserved.
//

#import "Platform.h"
#import "HandDetailsViewController.h"
#import "CardTableViewController.h"
#import "UIKit/UIView.h"
#import "GameModel.h"
#import "CardView.h"

#include "Hand.hpp"
#include "Combinations.h"
#include "PaySchedule.hpp"

#import "Deck.h"

@implementation HandDetailsViewController

@synthesize game;
@synthesize cardTable;
@synthesize detailTableView;
@synthesize tableCell;
@synthesize tableHeader;

#pragma mark -
#pragma mark View lifecycle

- (void)
viewDidLoad
{
    [super viewDidLoad];
    
    self.hidesBottomBarWhenPushed = YES;
    
    [self.view addSubview: cardTable.view];
    
    NSArray* titleArray = [NSArray arrayWithObjects: @"Your", @"Best", @"Custom", nil];
    UISegmentedControl* title = [[UISegmentedControl alloc] initWithItems: titleArray];
    title.segmentedControlStyle = UISegmentedControlStyleBar;
    [title addTarget: self action: @selector(selectHand) forControlEvents: UIControlEventValueChanged];
    title.selectedSegmentIndex = 0;
    [self navigationItem].titleView = title;
    [title release];
    
    detailTableView.rowHeight = 25.0;
    
    NSLog(@"HandDetailsVC::viewDidLoad()");
    //    [self.view setNeedsDisplay];
    // Uncomment the following line to display an Edit button in the navigation bar for this view controller.
    // self.navigationItem.rightBarButtonItem = self.editButtonItem;
}

- (void)
viewWillAppear: (BOOL) animated
{
    NSLog(@"HandDetailsVC::viewDidLoad()");

    [super viewWillAppear:animated];
    
    [detailTableView reloadData];
}

/*
- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
}
*/
/*
- (void)viewWillDisappear:(BOOL)animated {
    [super viewWillDisappear:animated];
}
*/
/*
- (void)viewDidDisappear:(BOOL)animated {
    [super viewDidDisappear:animated];
}
*/
/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

#pragma mark -
#pragma mark Table view data source

- (NSInteger)
numberOfSectionsInTableView: (UITableView *)tableView
{
    return 1;
}

- (NSInteger)
tableView: (UITableView *) tableView
numberOfRowsInSection: (NSInteger) section
{
    return game.schedule->getPaylineCount();
}

    // Customize the appearance of table view cells.
- (UITableViewCell *)
tableView: (UITableView *) tableView
cellForRowAtIndexPath: (NSIndexPath *) indexPath
{
    static NSString* cellIdentifier = @"HandDetailsTableCell";
    UITableViewCell* cell = [tableView dequeueReusableCellWithIdentifier:cellIdentifier];
    if (cell == nil)
    {
        [[NSBundle mainBundle] loadNibNamed:@"HandDetailsTableCell" owner:self options:nil];
        cell = self.tableCell;
        self.tableCell = nil;
    }

    UILabel* label;
      
    /* Label #1 */
    size_t coinCount = 5;
    int payLine = (int)[indexPath indexAtPosition: 1];
    Hand::Value_t handValue = game.schedule->getPaylineHandValue(payLine); //GetColumn(coinCount)->_payLines[payLine]._handValue;
    label = (UILabel*)[cell viewWithTag:1];
    label.text = [NSString stringWithUTF8String: Hand::getName(handValue).c_str()];

    // TODO: initialize "results" once on view load, and if/when cards change.
    // NOTE: this +1 because payline 0 = "nothing"
    
    /* Label #2 */
    const CardVector_t& heldCards = [self getHeldCards];
    uint32_t heldCardBits = game.hand->getCardBits(heldCards);
    size_t payoutCount = ((Hand::Solver::Default::Results_t&)game.solver->getResults(heldCardBits)).payouts[payLine + 1];
    label = (UILabel*)[cell viewWithTag:2];
    label.text = [NSString stringWithFormat: @"%d", (int)payoutCount]; //@"13";
	
    const size_t cardsPerHand = 5;
    size_t remainingCards = cardsPerHand - heldCards.size();
    size_t combinations = remainingCards > 0 ? Combinations::Calculate(47, remainingCards) : 1;
/*
    float handFrequency = (float)payoutCount / (float)combinations;
    label = (UILabel*)[cell viewWithTag:3];
    label.text = [NSString stringWithFormat: @"%3.1f%%", handFrequency * 100.0];
  */  

    /* Label #3 */
    size_t payout = (size_t)game.schedule->getPayout((unsigned)payLine, (unsigned)coinCount); // GetColumn(coinCount)->_payLines[payLine]._payout;
    float expectedReturn = 100.0 * (float) (uint32_t)(payout * payoutCount) / (float)(uint32_t)combinations;
    expectedReturn /= 5.0; // # coins bet
    label = (UILabel*)[cell viewWithTag:3];
    label.text = [NSString stringWithFormat: @"%6.2f%%", expectedReturn];
    
    
    return cell;
}

#pragma mark -
#pragma mark Table view delegate

- (void)
tableView:(UITableView *)tableView
didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    // Navigation logic may go here. Create and push another view controller.
	/*
	 <#DetailViewController#> *detailViewController = [[<#DetailViewController#> alloc] initWithNibName:@"<#Nib name#>" bundle:nil];
     // ...
     // Pass the selected object to the new view controller.
	 [self.navigationController pushViewController:detailViewController animated:YES];
	 [detailViewController release];
	 */
}

- (CGFloat)
tableView: (UITableView*) tableView
heightForHeaderInSection: (NSInteger) section
{
    return 50.0;
}

- (UIView*)
tableView: (UITableView*) tableView
viewForHeaderInSection: (NSInteger) section
{
    [[NSBundle mainBundle] loadNibNamed:@"HandDetailsTableCell" owner:self options:nil];
    UIView* header = self.tableHeader;
    self.tableHeader = nil;
    
    using namespace Hand::Solver;
    const Default::Results_t& results = (Default::Results_t&)game.solver->getResults(game.hand->getCardBits([self getHeldCards]));
    UILabel* label;
    
    /* Label #2 */
    const size_t totalPayouts = results.totalPayouts();
    stringstream ssPayouts;
    ssPayouts << totalPayouts;
    
    label = (UILabel*)[header viewWithTag:2];
    label.text = [NSString stringWithUTF8String: ssPayouts.str().c_str()]; // [NSString stringWithFormat: @"%d", payoutCount]; //@"13";
    
    /* Label #3 */
    // TODO: coin count
    double ev = (results.ev / 5.0);
    stringstream ss;
    ss.precision(2);
    ss << fixed << ev << "%";
    
    label = (UILabel*)[header viewWithTag:3];
    label.text = [NSString stringWithUTF8String: ss.str().c_str()];
        //    label.text = @"123.45%"; //[NSString stringWithFormat: @"%6.2f%%", expectedReturn];
        
    return header;
}

#pragma mark -
#pragma mark Custom behavior

- (UISegmentedControl*)
getTitle
{
    return (UISegmentedControl*)[self navigationItem].titleView;
}

- (void)
selectHand
{
    for (CardView* cardView in cardTable.view.subviews)
    {
        cardView.enabled = NO;
    }
    [cardTable redrawCards];
    [detailTableView reloadData];
    [detailTableView setNeedsDisplay];
}

- (void)
cardViewWasPressed: (CardView*) cardView
{
}

- (void)
cardsDidChange
{
}

- (Card_t)
getCard: (NSInteger) index
{
    return game.hand->getCard((int)index);
}

- (BOOL)
isHeldCard: (Card_t) card
{
    const CardVector_t& cards = [self getHeldCards];
    return cards.end() != find(cards.begin(), cards.end(), card);
}

- (CGImageRef)
getCardImage: (Card_t) card
{
    return [game.deck getCardImage: card];
}

- (CGImageRef)
getBackImage: (NSInteger) index
{
    return [Deck getBackImage];
}

- (const CardVector_t&)
getHeldCards
{
    switch([self getTitle].selectedSegmentIndex)
    {
        case 0:
        case 2:
            return game.hand->getHeldCards();
        case 1:
            return game.hand->getBestCards();
                //        default:
                //       return nil; // TODO: throw exception
    }
    throw runtime_error("getHeldCards()");
}

#pragma mark -
#pragma mark Memory management

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Relinquish ownership any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload
{
    // Relinquish ownership of anything that can be recreated in viewDidLoad or on demand.
    // For example: self.myOutlet = nil;
}


- (void)dealloc
{
    [super dealloc];
}

@end

