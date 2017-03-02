//
//  CardTableViewController.h
//  VideoPoker
//
//  Created by Mike McQueen on 7/27/10.
//  Copyright 2010 Amazon.com. All rights reserved.
//

#include "Card.hpp"

@class CardView;

@protocol CardTableViewControllerDelegate;
@protocol CardTableViewControllerDataSource;

////////////////////////////////////////////////////////////////////////////////

@protocol CardTableViewControllerDataSource

- (Card_t) getCard: (NSInteger) index;
- (BOOL) isHeldCard: (Card_t) card;
- (CGImageRef) getCardImage: (Card_t) card;
- (CGImageRef) getBackImage: (NSInteger) index;

@end

////////////////////////////////////////////////////////////////////////////////

@interface CardTableViewController :
    UIViewController<CardTableViewControllerDataSource>
{
    id<CardTableViewControllerDataSource> dataSource;
    id<CardTableViewControllerDelegate>   delegate;
}

@property (nonatomic, readonly, assign) IBOutlet id<CardTableViewControllerDataSource> dataSource;
@property (nonatomic, readonly, assign) IBOutlet id<CardTableViewControllerDelegate>   delegate;

- (IBAction) tapCardView: (id) sender;
- (void) redrawCards;

@end

////////////////////////////////////////////////////////////////////////////////

@protocol CardTableViewControllerDelegate

- (void) cardViewWasPressed: (CardView*) cardView;

@end

////////////////////////////////////////////////////////////////////////////////

