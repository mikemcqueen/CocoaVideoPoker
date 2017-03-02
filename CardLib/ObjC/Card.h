//
//  Card.h
//  FirstApp
//
//  Created by Mike McQueen on 7/27/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#ifndef Include_CARD_H_
#define Include_CARD_H_

#import <Foundation/Foundation.h>
#import <CoreGraphics/CGImage.h>

#include "Card.hpp"

@class Deck;

#if 0
LPCSTR	GetValueString( int iValue );
LPCSTR	GetSuitString( int iSuit );
#endif

#if 0
namespace Suit
{
	enum Type
	{
		Clubs,
		Diamonds,
		Hearts,
		Spades
	};
}
typedef Suit::Type Suit_t;

struct Card_t
{
	unsigned _value;

	int getNumber() 
	{
		unsigned num = _value / 4;
		return num + 2; // convert ace (12) to 1; 0 is a deuce
	}

	Suit_t 
	getSuit()
	{
		return (Suit_t)((int)_value % 4);
	}
};
#endif

@interface Card : NSObject
{
	Card_t* _pCard;
	Deck*   _deck;
}

@property(readwrite,assign) Card_t*    card;
@property(readwrite,assign) Deck*      deck;
@property(readonly)         NSInteger  value;
@property(readonly)         CGImageRef image;
@property(readonly)         Suit_t     suit;
@property(readonly)         NSInteger  number;

+(id) cardWithDeck:(Deck *) aDeck
           andCard:(Card_t*) aCard;

-(NSString*) text;
-(NSString*) description;

-(CGImageRef) image;
-(Suit_t)     suit;
-(NSInteger)  number;

@end // Card

//typedef	std::vector<Card_t*> CardVector_t;

#endif
