//
//  SolveViewController.h
//  VideoPoker
//
//  Created by Mike McQueen on 8/2/10.
//  Copyright 2010 Amazon.com. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "CardTableViewController.h"
#import "ReturnViewController.h"
#import "GameModel.h"

#include "Card.hpp"

@interface SolveViewController :
    UIViewController<UIPickerViewDataSource,
                     UIPickerViewDelegate,
                     CardTableViewControllerDataSource,
                     CardTableViewControllerDelegate,
                     ReturnViewControllerDelegate>
{
    GameModel*               game;
    CardTableViewController* cardTable;
    ReturnViewController*    returnViewController;
    UIPickerView*            cardPicker;   
    UIButton*                holdBestButton;
    
    CurrentHand     _hand;
    CardView*       selectedView;
    NSMutableArray* suitViewArray;
    NSMutableArray* numberViewArray;
    UIImageView*    clubView;
    UIImageView*    diamondView;
    UIImageView*    heartView;
    UIImageView*    spadeView;
}

@property (nonatomic, retain) IBOutlet GameModel*               game;
@property (nonatomic, retain) IBOutlet CardTableViewController* cardTable;
@property (nonatomic, retain) IBOutlet ReturnViewController*    returnViewController;
@property (nonatomic, retain) IBOutlet UIPickerView*            cardPicker;
@property (nonatomic, retain) IBOutlet UIButton*                holdBestButton;
@property (nonatomic, retain) IBOutlet NSMutableArray*          suitViewArray;
@property (nonatomic, retain) IBOutlet NSMutableArray*          numberViewArray;
    //bad. have this in CardTable (optionally)
@property (nonatomic, retain) IBOutlet CardView* selectedView;

- (void)
setPicker: (UIPickerView*) pickerView
toCard: (Card_t) card;

- (void)
buildSuitViews;

- (void)
buildNumberViews;

- (NSString*)
getCardNumberString: (size_t) number;

- (void)
cardsDidChange;

- (void)
showHandDetails;

@end
