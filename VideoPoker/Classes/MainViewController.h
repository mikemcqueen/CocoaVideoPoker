////////////////////////////////////////////////////////////////////////////////
//
//  MainViewController.h
//  VideoPoker
//
//  Created by Mike McQueen on 8/1/09.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#import "FlipsideViewController.h"
#import "CardTableViewController.h"
#import "ReturnViewController.h"
#import "ChooseGameViewController.h"
#import "GameModel.h"

#include "CurrentHand.hpp"
#include "Card.hpp"

@class MainView;
@class CardTableView;
@class CardView;

////////////////////////////////////////////////////////////////////////////////

@interface MainViewController :
    UIViewController<
        CardTableViewControllerDelegate,
        CardTableViewControllerDataSource,
        ReturnViewControllerDelegate,
        FlipsideViewControllerDelegate,
        ChooseGameViewControllerDelegate,
        GameModelContainer>
{
@private
    
    CurrentHand   _hand;
	BOOL          _draw;
    
    MainView*                  _mainView;
    PayScheduleViewController* _payScheduleController;
    CardTableViewController*   _cardTable;
    ReturnViewController*      _returnViewController;
    GameModel*                 _game;
}

@property (nonatomic, retain)                    GameModel*                 game;
@property (nonatomic, readonly, retain) IBOutlet MainView*                  mainView;
@property (nonatomic, readonly, retain) IBOutlet PayScheduleViewController* payScheduleController;
@property (nonatomic, readonly, retain) IBOutlet CardTableViewController*   cardTable;
@property (nonatomic, readonly, retain) IBOutlet ReturnViewController*      returnViewController;

- (IBAction) showInfo;
- (IBAction) dealOrDraw;
- (IBAction) holdBestCards;
- (IBAction) chooseGame;
- (IBAction) betOne;
- (IBAction) betMax;

- (void) showHandDetails;
- (void) deal;
- (void) draw;
//- (void) setDealUI;
//- (void) setDrawUI;
//- (void) showHandValue;
- (void) updateYourEv;
- (void) updateBet;

////////////////////////////////////////////////////////////////////////////////

@end

////////////////////////////////////////////////////////////////////////////////
