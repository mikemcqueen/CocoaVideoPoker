//////////////////////////////////////////////////////////////////////////////
//
// HANDDISP.MM
//
//////////////////////////////////////////////////////////////////////////////


#include "Hand.h"
#include "Card.h"
//#include "dim.h"

#define ASSERT(x) x



namespace Hand
{



//////////////////////////////////////////////////////////////////////////////

#define	HAND_VALUE_MASK		0xF0000000

// hand name flags

#define	HNF_VALUE			1
#define	HNF_VALUEVALUE		2
#define	HNF_VALUESUIT 		3
#define	HNF_SUIT			4

static struct
{
	DWORD			dwHandValue;
	char const *	pHandNameFmt;
	int				iType;
} rgHandNames[] =
{
	HighCard,			"Nothing, %s-high",					HNF_VALUE,
	OnePair,			"One Pair of %s%s",					HNF_VALUE,
	TwoPair,			"Two Pair, %s%s and %s%s",			HNF_VALUEVALUE,
	ThreeKind,			"Three of a Kind, %s%s",				HNF_VALUE,
	Straight,			"%s-high Straight",				    HNF_VALUE,
	Flush,				"%s-high Flush",					HNF_VALUE,
	FullHouse,			"Full House, %s%s full of %s%s",	    HNF_VALUEVALUE,
	FourKind,			"Four of a Kind, %s%s",				HNF_VALUE,
	StraightFlush,		"a %s-high Straight Flush",			HNF_VALUE,
	FiveKind,			"Five of a Kind, %s%s",   			HNF_VALUE,
	WildRoyalFlush,		"Wild Royal Flush",				HNF_SUIT,
	RoyalFlush,			"Royal Flush",					HNF_SUIT,
	FourWild,			"Four Wild %s%s",					HNF_VALUE,
	StraightDraw,		"%s-high Straight draw",			HNF_VALUE,
	FlushDraw,			"%s-high Flush draw",				HNF_VALUE,
	StraightFlushDraw,	"%s-high Straight Flush draw",	HNF_VALUE,
};

//////////////////////////////////////////////////////////////////////////////

LPCTSTR
Analyzer_t::GetValueString( DWORD dwHandValue )
{
	static char szHandValue[255];
	DWORD dwValue = (dwHandValue>>28)&0x0000000f;
	ASSERT( dwValue<HAND_NUMVALUES );
	ASSERT( rgHandNames[dwValue].dwHandValue==dwValue );

	dwHandValue &= ~HAND_VALUE_MASK;
	
	char const *pNameFmt = rgHandNames[dwValue].pHandNameFmt;

	switch( rgHandNames[dwValue].iType )
	{
	case HNF_VALUE:
		sprintf( szHandValue, pNameFmt,
				::GetValueString(GetFirstKicker(dwHandValue)),
				GetFirstKicker(dwHandValue) == 6 ? "es" : "s");
		break;

	case HNF_VALUEVALUE:
		sprintf( szHandValue, pNameFmt, 
			::GetValueString(GetFirstKicker(dwHandValue)),GetFirstKicker(dwHandValue) == 6 ? "es" : "s",
			::GetValueString(GetSecondKicker(dwHandValue)),GetSecondKicker(dwHandValue) == 6 ? "es" : "s" );
		break;

#if 0
	case HNF_VALUESUIT:
		printf( pNameFmt, ::GetValueString(LOWORD(dwHandValue)),
			::GetSuitString(HIWORD(dwHandValue)) );
		break;
#endif

	case HNF_SUIT:
		strcpy( szHandValue, pNameFmt );
		break;

	default:
		sprintf( szHandValue, "GetHandValueString Error: Value is %d, dwHandValue = %08X", dwValue, dwHandValue );
		ASSERT( 0 );
		break;
	}
	return szHandValue;
} /* GetValueString */

//////////////////////////////////////////////////////////////////////////////

void
Analyzer_t::DisplayValue( DWORD dwHandValue )
{
	puts( GetValueString(dwHandValue) );
} /* DisplayValue */

//////////////////////////////////////////////////////////////////////////////

void
Analyzer_t::Display( void )
{
#if 0
	for( CardVector_t::const_iterator iter= _cardVector.begin(); iter!=_cardVector.end(); ++iter )
	{	
		puts( (*iter)->GetDisplayString() );
	}
	putchar( '\n' );
#endif
	//	DisplayValue( GetValue() );
}

//////////////////////////////////////////////////////////////////////////////

/* static */
LPCTSTR
Analyzer_t::GetString( const CardVector_t& cardVector )
{
	static char szHand[255];
	szHand[0] = 0;

	for( CardVector_t::const_iterator iter=cardVector.begin(); iter!=cardVector.end(); ++iter )
	{
		if( szHand[0] )
		{
			strcat( szHand, "<not supported>" );
		}
//		strcat( szHand, iter->GetShortDisplayString() );
	}
	return szHand;
}

//////////////////////////////////////////////////////////////////////////////

LPCTSTR
Analyzer_t::GetString( bool bHeldOnly/*=false*/ ) const
{
	static char szHand[255];
	szHand[0] = 0;

	if( bHeldOnly )
	{
		return GetString( _heldCards );

#if 0
		for( CardVector_t::const_iterator iter=_heldCards.begin(); iter!=m_vHeldCards.end(); ++iter )
		{
			if( szHand[0] )
			{
				strcat( szHand, " " );
			}
			strcat( szHand, (*iter).GetShortDisplayString() );
		}

#endif
	}
	else
	{
		for( CardVector_t::const_iterator iter=_cardVector.begin(); iter!=_cardVector.end(); ++iter )
		{
			if( szHand[0] )
			{
				strcat( szHand, "<not supported>" );
			}
//			strcat( szHand, (*iter)->GetShortDisplayString() );
		}
	}
	return szHand;
}

//////////////////////////////////////////////////////////////////////////////

} // Hand