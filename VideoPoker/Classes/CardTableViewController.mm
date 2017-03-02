//
//  CardTableViewController.m
//  VideoPoker
//
//  Created by Mike McQueen on 7/27/10.
//  Copyright 2010 Amazon.com. All rights reserved.
//

#import "Platform.h"
#import "CardTableViewController.h"
#import "CardTableView.h"
#import "CardView.h"

#include "Card.hpp"

@implementation CardTableViewController

@synthesize dataSource;
@synthesize delegate;
    
////////////////////////////////////////////////////////////////////////////////

- (IBAction)
tapCardView: (id) sender
{
    CardView* cardView = (CardView*)sender;
	if (cardView.enabled)
	{
        [delegate cardViewWasPressed: cardView];
	}
    else
    {
        NSLog(@"tapCardView: disabled");
    }
}

- (BOOL)
isHeldCard: (Card_t) card
{
    if (dataSource == nil)
    {
        return false;
    }
    return [dataSource isHeldCard: card];
}

- (Card_t)
getCard: (NSInteger) index
{
    if (dataSource == nil)
    {
        return Card_t(Card_t::Value_t(0));
    }
    return [dataSource getCard: index];
}

- (CGImageRef)
getCardImage: (Card_t) card
{
    if (dataSource == nil)
    {
        return nil;
    }
    return (CGImageRef)[dataSource getCardImage: card];
}

- (CGImageRef)
getBackImage: (NSInteger) index
{
    if (dataSource == nil)
    {
        return nil;
    }
    return [dataSource getBackImage: index];
}

- (void)
redrawCards
{
    [(CardTableView*)[self view] redrawSubviews];
}

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

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView
{
}
*/

/*
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad
{
    [super viewDidLoad];
}
*/

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

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
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


- (void)
dealloc
{
    [super dealloc];
}


@end
