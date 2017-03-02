//
//  MainViewController.mm
//  VideoPoker
//
//  Created by Mike McQueen on 8/1/09.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import "Platform.h"

#import "Foundation/Foundation.h"
#import "QuartzCore/QuartzCore.h"
#import "CoreGraphics/CoreGraphics.h"

#import "MainViewController.h"
#import "ChooseGameViewController.h"
#import "PayScheduleViewController.h"
#import "VideoPokerAppDelegate.h"
#import "MainView.h"
#import "HandDetailsViewController.h"
#import "CardView.h"
#import "CardTableView.h"
#import "Deck.h"
#import "HandAnalyzer_t.hpp"
#import "GameModel.h"
#import "UIStrokeLabel.h"

//#import "SessionMO.h"
//#import "HandMO.h"
//#import "HandDataMO.h"


#include "PaySchedule.hpp"

@implementation MainViewController

@synthesize game                    = _game;
@synthesize mainView                = _mainView;
@synthesize payScheduleController   = _payScheduleController;
@synthesize cardTable               = _cardTable;
@synthesize returnViewController    = _returnViewController;

////////////////////////////////////////////////////////////////////////////////

- (id)
initWithNibName: (NSString *) nibName
bundle:          (NSBundle *) bundle
{
    if (self = [super initWithNibName:nibName bundle:bundle])
	{
    }
    return self;
}

////////////////////////////////////////////////////////////////////////////////

-(void)
viewDidLoad
{
	[super viewDidLoad];
    
    /* Initialize model */
    self->_hand.reset();

    const UINavigationBar* navBar = self.navigationController.navigationBar;
    navBar.tintColor = UIColorFromRGB(0.0, 0.0, 128.0);
    navBar.barStyle = UIBarStyleDefault;
    navBar.translucent = NO;

    /* Create and add Choose Game right nav bar button */
    UIBarButtonItem* item = [[UIBarButtonItem alloc] initWithTitle:@"Choose Game" style:UIBarButtonItemStyleDone
                                                            target:self action:@selector(chooseGame)];
    self.navigationItem.rightBarButtonItem = item;
    [item release];
    
    /* Create and add 'info' left nav bar button */
    UIButton* btn = [UIButton buttonWithType: UIButtonTypeInfoLight];
    [btn addTarget:self action:@selector(showInfo) forControlEvents:UIControlEventTouchUpInside];
    UIBarButtonItem* leftItem = [[UIBarButtonItem alloc] initWithCustomView:btn];
    self.navigationItem.leftBarButtonItem = leftItem;
    [leftItem release];
    
    /* Add pay schedule view */
    [_mainView addSubview: _payScheduleController.view];
    _mainView.payScheduleView = _payScheduleController.view;
    
    /* Add return view */
    [_mainView addSubview: _returnViewController.view];
    _mainView.returnView = _returnViewController.view;   

    /* Add card table view */
    [_mainView addSubview: _cardTable.view];
    _mainView.cardTableView = _cardTable.view;
    
    // TODO: cardTable.disableAll();
	for (CardView* cardView in _cardTable.view.subviews)
	{
		cardView.enabled = NO;
	}
/*
     NSString* text = [[NSString stringWithUTF8String: [self getPaySchedule]->getGameName().c_str()]
                      stringByAppendingFormat: @" - %4.2f%%", [self getPaySchedule]->
                      getReturn()];
    _mainView.payScheduleLabel.text = text;
 */   
	self.mainView.handValueLabel.text = nil;
	self.mainView.gameOverLabel.hidden = false;
    
    self.mainView.winLabel.strokeWidth = [NSNumber numberWithInt: 2];
    self.mainView.winLabel.strokeColor = [UIColor yellowColor];
    
    self.mainView.creditsLabel.strokeWidth = [NSNumber numberWithInt: 2];
    self.mainView.creditsLabel.strokeColor = [UIColor yellowColor];
}

- (void)
viewWillAppear:(BOOL)animated
{
    _game.hand = &_hand;

    // not exactly what i want but prevents a crash.
    // if coming back from solve view, and we have a valid hand in play,
    // we need to recalc returns; if no valid hand was in play , returns
    // should reset to zero?
    // TODO: this could happen in game.setHand() setter
    if (_game.hand->isValid())
    {
        [_game setCards: _game.hand->cards()];
    }
}

////////////////////////////////////////////////////////////////////////////////

- (IBAction)
betOne
{
    [self.game betOne];
    [self updateBet];
}

- (IBAction)
betMax
{
    [self.game betMax];
    [self updateBet];
}

- (void)
updateBet
{
    [self.payScheduleController.view setNeedsDisplay];
    
    NSString* text = [NSString stringWithFormat: @"BET %d", self.game.bet];
    self.mainView.betLabel.text = text;
    
    //[self.betLabel setNeedsDisplay];
}

////////////////////////////////////////////////////////////////////////////////

- (IBAction)
dealOrDraw
{
	if (NO == _draw)
	{
		/* Deal */
        
        [_game deal];
        
		// TODO: self.cardTable.invalidateAllCardViews();
        [self deal];
        
        // Update UI
        double dealtEv = ((Hand::Solver::Default::Results_t&)_game.solver->getResults(0)).ev;
        double bestEv = ((Hand::Solver::Default::Results_t&)_game.solver->getBestResults()).ev;
        
        [_returnViewController setYourEv: dealtEv];
        [_returnViewController setBestEv: bestEv];

        [_mainView setDrawUI];

        // can only view hand details after deal
        _returnViewController.detailButton.enabled = YES;
        // can't change game after deal
        self.navigationItem.rightBarButtonItem.enabled = NO;

	
        [_mainView showCash: self.game.cash];
    }
    else
	{
        /* Draw */
        
        uint32_t payout = [_game draw];
     
        // Update UI
        
        [self draw];

        [_mainView setDealUI];

        _returnViewController.detailButton.enabled = NO;
        self.navigationItem.rightBarButtonItem.enabled = YES;
       
        if (payout > 0)
        {
            [_mainView showWin: payout];
            [_mainView showCash: _game.cash];
        }
    }

    if (_game.handValueChanged)
    {
        [_mainView.payScheduleView setNeedsDisplay];
    }
    
	_draw = !_draw;
}

////////////////////////////////////////////////////////////////////////////////

- (void)
updateYourEv
{
    [_returnViewController setYourEv:[_game updateEv]];
}

////////////////////////////////////////////////////////////////////////////////

- (IBAction)
holdBestCards
{
    _game.hand->setHeldCards(_game.hand->getBestCards());
    for (CardView* cardView in _cardTable.view.subviews)
    {
        [cardView setNeedsDisplay];        
    }
    [self updateYourEv];
}
    
////////////////////////////////////////////////////////////////////////////////

- (void)
deal
{
    // TODO: CardTable.handCount
    for (int index = 0; index < 5; ++index)
    {
        CardView* cardView = [(CardTableView*)_cardTable.view getCardView: index];
        cardView.enabled = YES;
        [cardView setNeedsDisplay];
    }
}

////////////////////////////////////////////////////////////////////////////////

- (void)
draw
{
    for (int index = 0; index < 5; ++index)
    {
        CardView* cardView = [(CardTableView*)_cardTable.view getCardView: index];
        [cardView setNeedsDisplay];
		cardView.enabled = NO;
	}
}

////////////////////////////////////////////////////////////////////////////////

- (IBAction)
showInfo
{    
	FlipsideViewController *controller = [[FlipsideViewController alloc] initWithNibName:@"OptionsView" bundle:nil];
	controller.delegate = self;

    UINavigationController* navController = [[UINavigationController alloc] initWithRootViewController:controller];
	navController.modalTransitionStyle = UIModalTransitionStyleFlipHorizontal;
	[self presentModalViewController:navController animated:YES];
	
	[controller release];
    [navController release];
}

////////////////////////////////////////////////////////////////////////////////

- (void)
flipsideViewControllerDidFinish: (FlipsideViewController *) controller
{
	[self dismissModalViewControllerAnimated:YES];
}

////////////////////////////////////////////////////////////////////////////////

- (IBAction)
chooseGame
{
#if 0
    ChooseGameViewController* chooseController = [[ChooseGameViewController alloc] initWithNibName: @"ChooseGameView" bundle: nil];
#else
    ChooseGameViewController* chooseController = [[ChooseGameViewController alloc] initWithPaySchedule: _game.schedule];
#endif
    //    chooseController.game = self.game;
    chooseController.delegate = self;
    chooseController.title = @"Choose Game";
    
    UINavigationController* navController = [[UINavigationController alloc] initWithRootViewController: chooseController];
    navController.navigationBar.tintColor = UIColorFromRGB(0.0, 0.0, 128.0);
   
    UIBarButtonItem* done = [[UIBarButtonItem alloc] initWithTitle: @"Done" style: UIBarButtonItemStyleDone
                                                            target: chooseController action: @selector(done)];
    chooseController.navigationItem.rightBarButtonItem = done;
    [done release];
    
    UIBarButtonItem* cancel = [[UIBarButtonItem alloc] initWithTitle: @"Cancel" style: UIBarButtonItemStyleDone
                                                            target: chooseController action: @selector(cancel)];
    chooseController.navigationItem.leftBarButtonItem = cancel;
    [cancel release];
    
    [self presentModalViewController: navController animated: YES];
}

/* ChooseGameViewControllerDelegate */
#if 0
- (const PaySchedule_t*)
getPaySchedule
{
    return _game.schedule;
}
#endif

- (void)
chooseGameDone: (const PaySchedule::Lookup::Data_t&) schedule
{
    if ([_game switchToPaySchedule: schedule])
    {
        [self.payScheduleController payScheduleDidChange];
    }
    [self dismissModalViewControllerAnimated: YES];
}

- (void)
chooseGameCancel
{
    [self dismissModalViewControllerAnimated: YES];
}

////////////////////////////////////////////////////////////////////////////////

- (void)
showHandDetails
{
	HandDetailsViewController* handDetailsController = [[HandDetailsViewController alloc] initWithNibName: @"HandDetailsView" bundle: nil];
    handDetailsController.game = _game;
    handDetailsController.hidesBottomBarWhenPushed = YES;
    [self.navigationController pushViewController: handDetailsController animated: YES];
}

////////////////////////////////////////////////////////////////////////////////

- (void)
didReceiveMemoryWarning
{
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

////////////////////////////////////////////////////////////////////////////////

- (void)
viewDidUnload
{
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}

////////////////////////////////////////////////////////////////////////////////

- (void)
dealloc
{
        //    [game release];
    [super dealloc];
}

- (void)
cardViewWasPressed: (CardView*) cardView
{
    _game.hand->toggleHeldCard([cardView getCard]);
    [cardView setNeedsDisplay];
    [self updateYourEv];
}

////////////////////////////////////////////////////////////////////////////////

- (Card_t)
getCard: (NSInteger) index
{
    return _game.hand->getCard((int)index);
}

- (BOOL)
isHeldCard: (Card_t) card
{
    return _game.hand->isHeldCard(card);
}

- (CGImageRef)
getCardImage: (Card_t) card
{
    return [_game.deck getCardImage: card];
}

- (CGImageRef)
getBackImage: (NSInteger) index
{
    return [Deck getBackImage];
}

////////////////////////////////////////////////////////////////////////////////

- (void)
returnView: (UIView*) view
detailButtonWasPressed: (UIButton*) button
{
    [self showHandDetails];
}

////////////////////////////////////////////////////////////////////////////////

// GameModelContainer

- (GameModel*)
getGameModel
{
    return _game;
}

////////////////////////////////////////////////////////////////////////////////

@end

////////////////////////////////////////////////////////////////////////////////

