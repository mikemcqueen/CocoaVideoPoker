//
//  Card.cpp
//
//  Copyright (C) 2009 Mike McQueen. All rights reserved.
//

#include "stdafx.h"
#include "Card.hpp"
//#include "Deck.hpp"
#include "Poker.h"

static char const *rgpValueNames[] =
{
"[invalid-value]",
"Ace",
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
"Ace"
};

static char const *rgpSuitNames[] =
{
"Clubs",
"Diamonds",
"Hearts",
"Spades"
};

/* static */
size_t
Card_t::
getUniqueValue(
    const CardVector_t& cards)
{
    return getUniqueValue(&cards[0], cards.size());
}

/* static */
size_t
Card_t::
getUniqueValue(
    const Card_t* cards,
          size_t  cardCount)
{
    // TODO: supports 6 cards, 5 bits each (max card value of 63)
    if (cardCount > 5)
    {
        throw std::invalid_argument("Card_t::getUniqueValue() only works on 5 card hands or less");
    }
    size_t total = 0;
    for (size_t index = 0; index < cardCount; ++index)
    {
        total <<= 6;
        total += cards[index].getValue();
    }
    return total;
}
            
//namespace Card
//{
//////////////////////////////////////////////////////////////////////////////


bool operator >(const Card_t c1, const Card_t c2)
{
    return c1.getValue() > c2.getValue();
}


const char*
GetValueString( int iValue )
{
	return rgpValueNames[iValue];
} /* GetValueString */

//////////////////////////////////////////////////////////////////////////////

const char*
GetSuitString( int iSuit )
{
	return rgpSuitNames[iSuit];
} /* GetSuitString */

////////////////////////////////////////////////////////////////////////////////

const char*
GetShortDisplayString(
    size_t value)
{
    static char disp[3] = { 0 };
    static const char bigCards[] = "TJQKA";
    static const char suits[] = "cdhs";

    if (DRAW == value)
    {
        strcpy_s(disp, _countof(disp), "__");
    }
    else
    {
        if (value < 52) // Card_t::isStandard
        {
            size_t num = value / 4 + 2;
            sprintf_s(disp, _countof(disp), "%c%c", (int)(num < 10 ? '0' + num : bigCards[num - 10]), suits[value % 4]);
        }
        else // Card_t::isJoker
        {
            strcpy_s(disp, _countof(disp), "Jo");
        }
    }
    return disp;
}

////////////////////////////////////////////////////////////////////////////////

const char*
GetShortDisplayString(
                      const Card_t card)
{
    return GetShortDisplayString(card.getValue());
}

////////////////////////////////////////////////////////////////////////////////

bool
operator<(const Card_t c1, const Card_t c2)
{
    return  c1.getValue() < c2.getValue();
}

////////////////////////////////////////////////////////////////////////////////

bool operator==(const Card_t card, size_t value)
{
    return card.getValue() == value;
}

////////////////////////////////////////////////////////////////////////////////

bool operator==(const Card_t c1, const Card_t c2)
{
    return c1.getValue() == c2.getValue();
}

////////////////////////////////////////////////////////////////////////////////
