//////////////////////////////////////////////////////////////////////////////

#include "Hand.h"
#include "Card.h"
//#include "util.h"

#define ASSERT(x) x
namespace Hand
{

//////////////////////////////////////////////////////////////////////////////

DWORD
Analyzer_t::GetFourWild( void )
{
	return 0;
}

//////////////////////////////////////////////////////////////////////////////

DWORD
Analyzer_t::GetValue( void )
{
static struct
{
	int		iHandValue;
	BOOL	bWild;
	DWORD	(Analyzer_t::*fn)( void );
} rgHandValues[] =
{
	FourWild,			TRUE,		&Analyzer_t::GetFourWild,			
	RoyalFlush,			FALSE,		&Analyzer_t::GetRoyalFlush,
	WildRoyalFlush,		TRUE,		&Analyzer_t::GetWildRoyalFlush,
	FiveKind,			TRUE,		&Analyzer_t::GetFiveKind,
	StraightFlush,		FALSE,		&Analyzer_t::GetStraightFlush,
	FourKind,			FALSE,		&Analyzer_t::GetFourKind,
	FullHouse,			FALSE,		&Analyzer_t::GetFullHouse,
	Flush,				FALSE,		&Analyzer_t::GetFlush,
	Straight,			FALSE,		&Analyzer_t::GetStraight,
	ThreeKind,			FALSE,		&Analyzer_t::GetThreeKind,
	TwoPair,			FALSE,		&Analyzer_t::GetTwoPair,
	OnePair,			FALSE,		&Analyzer_t::GetOnePair,
	HighCard,			FALSE,		&Analyzer_t::GetHighCard,
	-1,					FALSE,		NULL,
};

	InitValues();
	InitSuits();
	for( int iHand=0; rgHandValues[iHand].iHandValue!=-1; ++iHand )
	{
		if( rgHandValues[iHand].bWild )
			continue;
		
		DWORD uiCardValue = (this->*rgHandValues[iHand].fn)();
		if( uiCardValue )
		{
			return MAKEHANDVALUE( rgHandValues[iHand].iHandValue, uiCardValue );
		}
	}
	ASSERT( 0 );
	return 0;
} /* GetValue */

//////////////////////////////////////////////////////////////////////////////
// Open = Open ended (outside draw)

DWORD
Analyzer_t::GetFourOpenStraightFlush( void )
{
	const int cRequired = 4;
	DWORD dwValue = CheckStraightFlush( cRequired, 0 );
	if( dwValue )
	{
		int iHighCard = GetSecondKicker( dwValue );

		// if the highest actual card in a zero-gap 4 card straight flush
		// is an Ace or Four, it's effectively an "insider", and will not
		// count in this function.

		// of course, iHighCard should *never* be Ace, because that would be
		// an inside Royal, and that should have been found & held already.

		if( 4<iHighCard && iHighCard<Ace )
		{
			int iMax = GetFirstKicker( dwValue );
			int cHeld = HoldCardsBySuit( GetSuit(dwValue), iMax-4, iMax );
			ASSERT( cRequired==cHeld );
			return dwValue;
		}
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////////

DWORD
Analyzer_t::GetFourStraightFlush( void )
{
	const int cRequired = 4;
	DWORD dwValue = CheckStraightFlush( cRequired );
	if( dwValue )
	{
		int iMax = GetFirstKicker( dwValue );
		int cHeld = HoldCardsBySuit( GetSuit(dwValue), iMax-4, iMax );
		ASSERT( cRequired==cHeld );
	}
	return dwValue;
}

//////////////////////////////////////////////////////////////////////////////

DWORD
Analyzer_t::GetThreeOpenStraightFlush( void )
{
	return 0;
}

//////////////////////////////////////////////////////////////////////////////

DWORD
Analyzer_t::GetJacksOrBetter( void )
{
	DWORD dwValue = CheckOnePair();
	if( dwValue )
	{
		int iPair = GetFirstKicker( dwValue );
		if( Jack<=iPair )
		{
			int cHeld = HoldCardsByValue( iPair, 2 );
			ASSERT( 2==cHeld );
			return dwValue;
		}
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////////

DWORD
Analyzer_t::GetFourFlush( void )
{
	return CheckFlush( 4 );
}

//////////////////////////////////////////////////////////////////////////////

DWORD
Analyzer_t::GetThreeFlush( void )
{
	return CheckFlush( 3 );
}

//////////////////////////////////////////////////////////////////////////////
// For Jacks or Better, this is the same as saying "a four card open ended
// straight with three high cards (3 cards which are Jack or better)." 
// TJQK is the only 4-open case where the 3 high card condition is met.
// For this to work in, for example, 10s or better, something has to change.
//

DWORD
Analyzer_t::GetFourOpenStraightKingHigh( void )
{
	const int cRequired = 4;
	DWORD dwValue = CheckStraight( m_rgiValues, cRequired );
	if( dwValue )
	{
		DWORD iHighCard = GetSecondKicker( dwValue );
		if( King==iHighCard )
		{
			int cHeld = 0;
			for( int iCard=0; iCard<cRequired; ++iCard )
			{
				cHeld += HoldCardsByValue( iHighCard--, 1 );
			}
			ASSERT( cRequired==cHeld );
			return dwValue;
		}
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////////

DWORD
Analyzer_t::GetFourOpenStraightThreeHighCards( void )
{
	return GetFourOpenStraightKingHigh();
}

//////////////////////////////////////////////////////////////////////////////

DWORD
Analyzer_t::GetFourOpenStraight( void )
{
	const int cRequired = 4;
	DWORD dwValue = CheckStraight( m_rgiValues, cRequired );
	if( dwValue )
	{
		int iHighCard = GetSecondKicker( dwValue );

		// if the highest actual card in a zero-gap 4 card straight 
		// is an Ace or Four, it's effectively an "insider", and will not
		// count in this function.

		if( 4<iHighCard && iHighCard<Ace )
		{
			int cHeld = 0;
			for( int iCard=0; iCard<4; ++iCard )
			{
				cHeld += HoldCardsByValue( iHighCard--, true );
			}
			ASSERT( cRequired==cHeld );
			return dwValue;
		}
	}
	return 0;
} /* GetFourOpenStraight */

//////////////////////////////////////////////////////////////////////////////

DWORD
Analyzer_t::GetFourStraight( void )
{
	const int cRequired = 4;
	DWORD dwValue = CheckStraight( m_rgiValues, cRequired, 1 );
	if( dwValue )
	{
		int iHighest = GetFirstKicker( dwValue );
		int cHeld = 0;
		for( int iCard=0; iCard<CardsPerHand; ++iCard )
		{
			cHeld += HoldCardsByValue( iHighest--, 1 );
		}
		ASSERT( cRequired==cHeld );
	}
	return dwValue;
} /* GetFourStraight */

//////////////////////////////////////////////////////////////////////////////

DWORD
Analyzer_t::GetThreeStraightFlush( void )
{
	const int cRequired = 3;
	DWORD dwValue = CheckStraightFlush( cRequired );
	if( dwValue )
	{
		int iHighest = GetFirstKicker( dwValue );
		int cHeld = HoldCardsBySuit( GetSuit(dwValue), iHighest-4, iHighest );
		ASSERT( cRequired==cHeld ); // cHeld could be 4 or 5 here
	}
	return dwValue;
}

//////////////////////////////////////////////////////////////////////////////

DWORD
Analyzer_t::GetFourRoyalFlush( void )
{
	return CheckRoyalFlush( 4 );
}

//////////////////////////////////////////////////////////////////////////////

DWORD
Analyzer_t::GetThreeRoyalFlush( void )
{
	return CheckRoyalFlush( 3 );
}

//////////////////////////////////////////////////////////////////////////////

DWORD
Analyzer_t::GetTwoRoyalFlush( void )
{
	return CheckRoyalFlush( 2 );
}

//////////////////////////////////////////////////////////////////////////////

DWORD
Analyzer_t::GetFourStraightThreeHighCards( void )
{
	const int cRequired = 4;
	DWORD dwValue = CheckStraight( m_rgiValues, cRequired, 1 );
	if( dwValue )
	{
		int iHighest = GetFirstKicker( dwValue );

		// if it's at least a king high straight draw, and there are at least three
		// cards which are jack or better
		if( King<=iHighest && 3<CountUniqueValuesInRange(m_rgiValues, Jack, iHighest) )
		{
			int cHeld = 0;
			for( int iCard=0; iCard<CardsPerHand; ++iCard )
			{
				cHeld += HoldCardsByValue( iHighest--, 1 );
			}
			ASSERT( cRequired==cHeld );
			return dwValue;
		}
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////////

DWORD
Analyzer_t::GetHighCardsJackOrBetter( void )
{
	DWORD dwValue = GetHighCard();
	int iKicker = GetFirstKicker( dwValue );
	if( Jack<=iKicker )
	{
		HoldCardsByValue( iKicker );

		iKicker = GetSecondKicker( dwValue );
		if( Jack<=iKicker )
			HoldCardsByValue( iKicker );

		iKicker = GetThirdKicker( dwValue );
		if( Jack<=iKicker )
			HoldCardsByValue( iKicker );
	}
	return dwValue;
}

//////////////////////////////////////////////////////////////////////////////

DWORD
Analyzer_t::HoldCards( void )
{
static struct
{
	int		iHandValue;
	DWORD	(Analyzer_t::*fn)( void );
} rgHold[] =
{
	StraightFlush,				&Analyzer_t::GetStraightFlush,
	FourKind,					&Analyzer_t::GetFourKind,
	FullHouse,					&Analyzer_t::GetFullHouse,
	StraightFlushDraw,			&Analyzer_t::GetFourRoyalFlush,
	Flush,						&Analyzer_t::GetFlush,
	Straight,					&Analyzer_t::GetStraight,
	StraightFlushDraw,			&Analyzer_t::GetFourOpenStraightFlush,
	ThreeKind,					&Analyzer_t::GetThreeKind,
	StraightFlushDraw,			&Analyzer_t::GetFourStraightFlush,
	TwoPair,					&Analyzer_t::GetTwoPair,
	FlushDraw,					&Analyzer_t::GetFourFlush,
	StraightFlushDraw,			&Analyzer_t::GetThreeRoyalFlush,
	StraightDraw,				&Analyzer_t::GetFourOpenStraightThreeHighCards,
	StraightDraw,				&Analyzer_t::GetFourOpenStraight,
	OnePair,					&Analyzer_t::GetJacksOrBetter,
	StraightDraw,				&Analyzer_t::GetFourStraightThreeHighCards,
	OnePair,					&Analyzer_t::GetOnePair,
	StraightFlushDraw,			&Analyzer_t::GetTwoRoyalFlush,
	StraightFlushDraw,			&Analyzer_t::GetThreeStraightFlush,
	HighCard,					&Analyzer_t::GetHighCardsJackOrBetter,
//	FlushDraw,					GetThreeFlush,
	-1,							NULL,
};

	DWORD dwHandValue = 0;

	InitValues();
	InitSuits();

	for( int iHand=0; rgHold[iHand].iHandValue!=-1; ++iHand )
	{
		DWORD dwCardValue = (this->*rgHold[iHand].fn)();
		if( dwCardValue )
		{
			dwHandValue = MAKEHANDVALUE( rgHold[iHand].iHandValue, dwCardValue );
			break;
		}
	}
	return dwHandValue;
} /* HoldCards */
}
//////////////////////////////////////////////////////////////////////////////
