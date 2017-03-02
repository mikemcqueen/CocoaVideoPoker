//
//  Deck.mm
//  FirstApp
//
//  Created by Mike McQueen on 7/27/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//


#import <Platform.h>

#import <stdio.h>
#import <stdlib.h>

#import "Deck.h"
#import "Card.h"
#import "Poker.h"

using namespace Poker::Card;

////////////////////////////////////////////////////////////////////////////////

/*
 @implementation ShuffleShim

    //@synthesize card;
@synthesize sortKey;

- (id)
initWithCard: (Card*) aCard
{
	self = [super init];
	if (self)
	{
		card = aCard;
	}
	return self;
}

@end
*/

////////////////////////////////////////////////////////////////////////////////

@implementation Deck

////////////////////////////////////////////////////////////////////////////////

-(id) 
initWithSize: (int) deckSize
{
	return [self initNormal: deckSize];
}

////////////////////////////////////////////////////////////////////////////////

+(CGImageRef)
loadImage: (NSString*)imageFilename
{
	NSString* imagePath = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:imageFilename];
    
	CGDataProviderRef provider = CGDataProviderCreateWithFilename([imagePath UTF8String]);
	CGImageRef image = CGImageCreateWithPNGDataProvider(provider, NULL, true, kCGRenderingIntentDefault);
	CGDataProviderRelease(provider);
    
	return image;
}

////////////////////////////////////////////////////////////////////////////////

-(id)
initNormal: (int) deckSize
{
	self = [super init];
	if (self)
	{
        _deck = new Deck_t(deckSize);
		CGImageRef deckImage = [Deck loadImage:@"classic-playing-cards.png"];
            //	cards = [[NSMutableArray alloc] init];
            //shims = [[NSMutableArray alloc] init];
		_cardImages = (CGImageRef*)malloc(sizeof(CGImageRef) * deckSize);
		for (size_t cardIndex = 0; cardIndex < deckSize; ++cardIndex)
		{

			/*
             Card_t* pCard = new Card_t;
			pCard->setValue(i);
		
			// [cards addObject: [Card initWithValue: i]]
			Card* card = [Card cardWithDeck:self andCard:pCard];
//			[card setValue:i];
			[cards addObject:card];	
			
			// shim
			// TODO: lost reference here?
			[self->shims addObject:[[ShuffleShim alloc] initWithCard:card]];
             */
			
			// cardImage
			CGRect cardRect = { 0, 0, 71, 96 };
            Card_t card(cardIndex);
			int cardNumber = card.getNumber();
			if (Ace == cardNumber)
				cardNumber = AceLow;
			cardRect.origin.x = 1 + (cardRect.size.width + 2) * (cardNumber - 1);
			Suit_t cardSuit = card.getSuit();
			if (cardSuit == Suit::Diamonds)
            {
				cardSuit = Suit::Spades;
            }
			else if (cardSuit == Suit::Spades)
            {
				cardSuit = Suit::Diamonds;
            }
			cardRect.origin.y = 1 + (cardRect.size.height + 2) * cardSuit;

			// TODO: JOKER
            
            _cardImages[cardIndex] = CGImageCreateWithImageInRect(deckImage, cardRect);
		}
		CGImageRelease(deckImage);

	}
	return self;
}
	
////////////////////////////////////////////////////////////////////////////////

- (CGImageRef)
getCardImage: (Card_t) card
{
	return _cardImages[card.getValue()];
}

////////////////////////////////////////////////////////////////////////////////

+(CGImageRef)
getBackImage
{
	static CGImageRef backImage = NULL;
	if (NULL == backImage)
	{
		backImage = [Deck loadImage:@"blueback.png"];
	}
	return backImage;
}

////////////////////////////////////////////////////////////////////////////////
/*
NSInteger
sortShims(id shim1, id shim2, void* context)
{
	if ([shim1 sortKey] < [shim2 sortKey])
		return NSOrderedAscending;
	else if ([shim2 sortKey] < [shim1 sortKey])
		return NSOrderedDescending;
	else
		return NSOrderedSame;
}

////////////////////////////////////////////////////////////////////////////////
*/
- (void)
shuffle
{
/*
 int count = [shims count];
	for (int i = 0; i < count; ++i)
	{
		[[shims objectAtIndex:i] setSortKey:random() % 1000000];
	}
	[shims sortUsingFunction:sortShims context:NULL];
	topShim = 0;
 */
    _deck->Shuffle();
}

////////////////////////////////////////////////////////////////////////////////

- (Card_t)
deal
{
	/*
     ShuffleShim* shim = [shims objectAtIndex:topShim++];
	return [shim card];
     */
    return _deck->Deal();
}

////////////////////////////////////////////////////////////////////////////////

@end
