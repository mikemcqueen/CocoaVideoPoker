//
//  Deck.h
//  FirstApp
//
//  Created by Mike McQueen on 7/27/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreGraphics/CGImage.h>

#include "Deck.hpp"
#include "Card.hpp"

////////////////////////////////////////////////////////////////////////////////
//
// Deck
//
////////////////////////////////////////////////////////////////////////////////

@interface Deck : NSObject
{
    Deck_t*     _deck;
	CGImageRef* _cardImages; // array of card images
}

- (id)         initWithSize: (int) deckSize;
- (id)         initNormal: (int) deckSize;
- (void)	   shuffle;
- (Card_t)     deal;

- (CGImageRef) getCardImage: (Card_t) card;

+ (CGImageRef) getBackImage;
+ (CGImageRef) loadImage: (NSString*)imageFilename;

@end
