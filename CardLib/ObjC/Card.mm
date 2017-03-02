//
//  Card.mm
//  FirstApp
//
//  Created by Mike McQueen on 7/27/09.
//  Copyright 2009 Mike McQueen. All rights reserved.
//

#import "Platform.h"

#import "Card.h"
#import "Deck.h"

@implementation Card;

@synthesize card = _pCard;
@synthesize deck = _deck;

//@dynamic value;
//@synthesize value;
//@synthesize deck;
//@synthesize image;
//@synthesize suit;
//@synthesize number;

#if 0

static char const *rgpValueNames[] =
{
"<Error1>",
"<Error2>",
"Deuce",
"Three",
"Four",
"Five",
"Six",
"Seven",
"Eight",
"Nine",
"Ten",
"Jack",
"Queen",
"King",
"Ace",
"<Error3>",
};

static char const *rgpSuitNames[] =
{
"Clubs",
"Diamonds",
"Hearts",
"Spades",
"<Error>",
};

//namespace Card
//{
//////////////////////////////////////////////////////////////////////////////

LPCSTR
GetValueString( int iValue )
{
	return rgpValueNames[iValue];
} /* GetValueString */

//////////////////////////////////////////////////////////////////////////////

LPCSTR
GetSuitString( int iSuit )
{
	return rgpSuitNames[iSuit];
} /* GetSuitString */

//}
#endif

////////////////////////////////////////////////////////////////////////////////

+(id)
cardWithDeck:(Deck *) aDeck
     andCard:(Card_t*) pCard
{
	// TODO: initFromDeck: deck];, make deck readonly
	Card* newCard = [[Card alloc] init];
	newCard.card = pCard;
	newCard.deck = aDeck;
	return newCard;
}

////////////////////////////////////////////////////////////////////////////////

-(NSInteger)
value
{
	return self.card->getValue();
}

////////////////////////////////////////////////////////////////////////////////
#if 0
-(void)
setValue:(NSInteger) aValue
{
	self.card->setValue(aValue);
}
#endif

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

-(CGImageRef)
image
{
	return [self.deck getCardImage:self.value];
}

////////////////////////////////////////////////////////////////////////////////

-(NSInteger)
number
{
	return self.card->getNumber();
	
	NSInteger num = self.value / 4;
	return (num == 12) ? 1 : num + 2; // convert ace (13) to 1; 0 is a deuce
}

////////////////////////////////////////////////////////////////////////////////

-(Suit_t)
suit
{
	return self.card->getSuit();
	
	return (Suit_t)((int)self.value % 4);
}

////////////////////////////////////////////////////////////////////////////////

- (NSString*)
text
{
	if (self.value < 52)
	{
		int num = self.number;
		if (1 == num)
			num += 13;
//		int suit = 
		static NSString* suits[] = { @"c", @"d", @"h", @"s" };
		if (10 < num)
		{
			static NSString* acesFaces[] = { @"J", @"Q", @"K", @"A" };
			return [NSString stringWithFormat:@"%@%@", acesFaces[num - 11], suits[self.suit]];
		}
		else
		{
			return [NSString stringWithFormat:@"%d%@", num, suits[self.suit]];
		}
	}
	return [NSString stringWithFormat:@"%d", self.value];
}

////////////////////////////////////////////////////////////////////////////////

- (NSString*)
description
{
	return [self text];
}

////////////////////////////////////////////////////////////////////////////////

@end

////////////////////////////////////////////////////////////////////////////////
