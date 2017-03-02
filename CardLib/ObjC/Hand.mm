//////////////////////////////////////////////////////////////////////////////
//
// HAND.MM
//
//////////////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
//#import <vector>
#import "Hand.h"
#import "Card.h"
//#include "util.h"

#define ASSERT(x) x

namespace Hand
{

//////////////////////////////////////////////////////////////////////////////


Analyzer_t::Analyzer_t(const CardVector_t& cardVector) :
	_cardVector(cardVector),
	_heldCardCount(0),
	m_cKickers(0)
{
}


//////////////////////////////////////////////////////////////////////////////

Analyzer_t::~Analyzer_t( void )
{
}

//////////////////////////////////////////////////////////////////////////////

bool
Analyzer_t::IsValid( void ) const
{
#if 0
	for( int iCard=0; iCard<GetHeldCardCount(); ++iCard )
	{
		if( !GetHeldCard(iCard).IsValid() )
			return false;
	}

#endif
	return true;
}

//////////////////////////////////////////////////////////////////////////////

void
Analyzer_t::Invalidate( void )
{

#if 0
	for( int iCard=0; iCard<GetHeldCardCount(); ++iCard )
		GetHeldCard(iCard).Invalidate();
#endif
}

//////////////////////////////////////////////////////////////////////////////

Card_t&
Analyzer_t::GetHeldCard( int iCard )
{
	return *_heldCards[iCard];
}

//////////////////////////////////////////////////////////////////////////////

const Card_t&
Analyzer_t::GetHeldCard( int iCard ) const
{
	return *_heldCards[iCard];
}

//////////////////////////////////////////////////////////////////////////////

void
Analyzer_t::HoldCard(Card_t& card)
{
	_heldCards[_heldCardCount++] = &card;
}

/////////////////////////////////////////////////////////////////////////////


#if 0
	const Card&
Analyzer_t::GetCommunityCard( int iCard )
	{
	ASSERT( 0<=iCard && iCard<s_cCommunityCards );
	return s_aCommunityCards[iCard];
}

#endif
	
//////////////////////////////////////////////////////////////////////////////

#if 0
void
Analyzer_t::SetCommunityCard( int iCard, const Card& card )
{
	ASSERT( 0<=iCard && iCard<s_cCommunityCards );
	s_aCommunityCards[iCard] = card;
}
#endif	

//////////////////////////////////////////////////////////////////////////////

/*
void
Analyzer_t::SetStructure(
int		cHeldCards,
int		cCommunityCards)
{
	ASSERT( cHeldCards+cCommunityCards );
	ASSERT( cCommunityCards<=COMMUNITY_CARDS_MAX );

//	erase( m_vpHeldCards.begin(), m_vpHeldCards.end() );
//	if( cHeldCards )
	{
		m_vHeldCards.resize( cHeldCards );
	}

	erase( begin(), end() );
	reserve( cHeldCards+cCommunityCards );
	int iCard;
	for( iCard=0; iCard<cHeldCards; ++iCard )
	{
		push_back( &m_vHeldCards[iCard] );
	}
	for( iCard=0; iCard<cCommunityCards; ++iCard )
	{
		push_back( &s_aCommunityCards[iCard] );
	}
	s_cCommunityCards = cCommunityCards;

	ASSERT( size()==cHeldCards+cCommunityCards );
}
*/

//////////////////////////////////////////////////////////////////////////////

void Analyzer_t::InitValues( void )
{
	memset( m_rgiValues, 0, sizeof(m_rgiValues) );
	for( CardVector_t::const_iterator iter = _cardVector.begin(); iter != _cardVector.end(); ++iter )
	{
		m_rgiValues[(*iter)->getNumber()]++;
	}
	m_rgiValues[AceLow] = m_rgiValues[Ace];
}

//////////////////////////////////////////////////////////////////////////////

void Analyzer_t::InitSuits( void )
{
	memset( m_rgiSuits, 0, sizeof(m_rgiSuits) );
	for( CardVector_t::const_iterator iter = _cardVector.begin(); iter != _cardVector.end(); ++iter )
	{
		m_rgiSuits[(*iter)->getSuit()]++;
	}
}

//////////////////////////////////////////////////////////////////////////////

DWORD Analyzer_t::MakeKicker( void )
{
	DWORD dwKicker = 0;

	for( int i=0; i<6; i++ )
	{
		if( i<m_cKickers )
			dwKicker |= m_rgdwKickers[i];
		dwKicker <<= 4;
	}
#ifdef _DEBUG
//	printf( "\nMakeKicker = %08X\n", uiKicker );
#endif
	return dwKicker;
}

//////////////////////////////////////////////////////////////////////////////

#if 0
const Card&
Analyzer_t::GetCard( int iCard )
{
	ASSERT( 0<=iCard && iCard<size() );
	return at( iCard );
}
#endif

//////////////////////////////////////////////////////////////////////////////

int
Analyzer_t::HoldCardsByValue(
	int			iValue,
	int			iHoldMax /*=5*/)
{
	int cHeld = 0;
	if( AceLow==iValue )
		iValue = Ace;
	for( CardVector_t::const_iterator iter = _cardVector.begin(); iter != _cardVector.end(); ++iter )
	{
		if( (*iter)->getNumber()==iValue )
		{
			HoldCard(*(*iter));
			if( ++cHeld==iHoldMax )
				break;
		}
	}
	return cHeld;
}

//////////////////////////////////////////////////////////////////////////////

int
Analyzer_t::HoldCardsBySuit(
	int		iSuit,
	int		iValueMin,	/*=Deuce*/
	int		iValueMax	/*=Ace*/)
{
	ASSERT( AceLow<=iValueMin && iValueMin<=Ace );
	ASSERT( AceLow<=iValueMax && iValueMax<=Ace );
	ASSERT( iValueMin<=iValueMax );

	bool bAceLow = AceLow==iValueMin;
	int cHeld = 0;
	for( CardVector_t::const_iterator iter = _cardVector.begin(); iter != _cardVector.end(); ++iter )
	{
		int iValue = (*iter)->getNumber();
		if( (*iter)->getSuit()==iSuit )
		{
			if( (iValueMin<=iValue && iValue<=iValueMax) || (bAceLow && Ace==iValue) )
			{
				HoldCard(*(*iter));
				cHeld++;
			}
		}
	}
	return cHeld;
}

//////////////////////////////////////////////////////////////////////////////

void
Analyzer_t::HoldAllCards( void )
{
	for( CardVector_t::const_iterator iter = _cardVector.begin(); iter != _cardVector.end(); ++iter )
	{
		HoldCard(*(*iter));
	}
}

//////////////////////////////////////////////////////////////////////////////

int
Analyzer_t::CountCardsBySuit(
int		iSuit,
int		iValueMin,	/*=Deuce*/
int		iValueMax	/*=Ace*/)
{
	ASSERT( AceLow<=iValueMin && iValueMin<=Ace );
	ASSERT( AceLow<=iValueMax && iValueMax<=Ace );
	ASSERT( iValueMin<=iValueMax );

	int cFound = 0;
	for( CardVector_t::const_iterator iter = _cardVector.begin(); iter != _cardVector.end(); ++iter )
	{
		int iValue = (*iter)->getNumber();
		if( (*iter)->getSuit()==iSuit && iValueMin<=iValue && iValue<=iValueMax )
		{
			cFound++;
		}
	}
	return cFound;
}

//////////////////////////////////////////////////////////////////////////////

int
Analyzer_t::CountUniqueValuesInRange(
	int		*piValues,
	int		iValueMin,
	int		iValueMax)
{
	ASSERT( piValues );
	ASSERT( AceLow<=iValueMin && iValueMin<=Ace );
	ASSERT( AceLow<=iValueMax && iValueMax<=Ace );
	ASSERT( iValueMin<=iValueMax );

	int cFound = 0;
	for( int iValue=iValueMin; iValue<=iValueMax; ++iValue )
	{
		if( piValues[iValue] )
		{
			cFound++;
		}
	}
	return cFound;
}

//////////////////////////////////////////////////////////////////////////////

DWORD
Analyzer_t::MakeHighHand( int *piValues )
{
	m_cKickers = 0;
	for( int iValue=Ace; Deuce<=iValue; --iValue )
	{
		if( piValues[iValue] )
		{
			m_rgdwKickers[m_cKickers++] = iValue;
			if( 5==m_cKickers )
				break;
		}
	}
	return MakeKicker();
}

//////////////////////////////////////////////////////////////////////////////

DWORD
Analyzer_t::GetHighCard( void )
{
	return MakeHighHand( m_rgiValues );
}

//////////////////////////////////////////////////////////////////////////////

int
Analyzer_t::CheckPair( void )
{
	int iPair = 0;
	for( int iValue=Ace; Deuce<=iValue; --iValue )
	{
		if( 2==m_rgiValues[iValue] )
		{
			iPair = iValue;
			break;
		}
	}
	return iPair;
}

//////////////////////////////////////////////////////////////////////////////

DWORD
Analyzer_t::CheckOnePair( void )
{
	int iPair = CheckPair();
	if( !iPair )
		return 0;
	
	m_rgdwKickers[0] = iPair;
	m_cKickers = 1;
	
	for( int iValue=Ace; Deuce<=iValue; --iValue )
	{
		if( iValue==iPair )
			continue;
		
		if( m_rgiValues[iValue] )
		{
			m_rgdwKickers[m_cKickers++] = iValue;
			if( 4==m_cKickers )
				break;
		}
	}
	ASSERT( 4==m_cKickers );

	return MakeKicker();
}

//////////////////////////////////////////////////////////////////////////////

DWORD Analyzer_t::GetOnePair( void )
{
	DWORD dwValue = CheckOnePair();
	if( dwValue )
	{
		DWORD dwKicker = GetFirstKicker( dwValue );
		int cHeld = HoldCardsByValue( dwKicker );
		ASSERT( 2==cHeld );
	}
	return dwValue;
}

//////////////////////////////////////////////////////////////////////////////

DWORD Analyzer_t::GetTwoPair( void )
{
	int iHighPair = 0;
	int iLowPair = 0;
	int iKicker = 0;

	for( int iValue=Ace; Deuce<=iValue; --iValue )
	{
		if( 2==m_rgiValues[iValue] )
		{
			if( !iHighPair )
			{
				iHighPair = iValue;
				continue;
			}
			else if( !iLowPair )
			{
				iLowPair = iValue;
				continue;
			}
		}
		if( m_rgiValues[iValue] && !iKicker )
		{
			iKicker = iValue;
		}
	}

	if( !iHighPair || !iLowPair )
		return 0;
	ASSERT( iKicker );

	int cHeld;
	cHeld = HoldCardsByValue( iHighPair, 2 );
	cHeld += HoldCardsByValue( iLowPair, 2 );
	ASSERT( 4==cHeld );

	m_rgdwKickers[0] = iHighPair;
	m_rgdwKickers[1] = iLowPair;
	m_rgdwKickers[2] = iKicker;
	m_cKickers = 3;

	return MakeKicker();
} /* GetTwoPair */

//////////////////////////////////////////////////////////////////////////////

int
Analyzer_t::CheckThreeKind( void )
{
	int iTrips = 0;
	for( int iValue=Ace; Deuce<=iValue; --iValue )
	{
		if( 3==m_rgiValues[iValue] )
		{
			iTrips = iValue;
			break;
		}
	}
	return iTrips;
} /* CheckThreeKind */

//////////////////////////////////////////////////////////////////////////////

DWORD
Analyzer_t::GetThreeKind( void )
{
	int iTrips = CheckThreeKind();
	if( !iTrips )
		return 0;

	m_rgdwKickers[0] = iTrips;
	m_cKickers = 1;
	
	for( int iValue=Ace; Deuce<=iValue; --iValue )
	{
		if( iValue==iTrips )
			continue;
		
		if( m_rgiValues[iValue] )
		{
			m_rgdwKickers[m_cKickers++] = iValue;
			if( m_cKickers==3 )
				break;
		}
	}
	HoldCardsByValue( iTrips, 3 );
	ASSERT( m_cKickers==3 );
	return MakeKicker();
} /* GetThreeKind */

//////////////////////////////////////////////////////////////////////////////

DWORD
Analyzer_t::CheckStraight( int *piValues, int CardsRequired/*=5*/, int iGapMax/*=0*/ )
{
	int CardsFound = 0;
	int iGapSize = 0;
	int iGapStart = 0;
	int iHighCard = 0;

	size_t iValue = Ace;
	for( ; AceLow<=iValue; --iValue )
	{
		if( 0<piValues[iValue] )
		{
			if( !iHighCard )
				iHighCard = iValue;
			if( ++CardsFound==CardsRequired )
				break;
		}
		else
		{
			if( CardsFound && (iGapSize<iGapMax) )
			{
				iGapSize++;
				if( !iGapStart )
					iGapStart = iValue;
			}
			else
			{
				CardsFound = 0;
				iGapSize = 0;
				iHighCard = 0;
				if( iGapStart )
				{
					iValue = iGapStart; // tricky tricky!  no loop optimizations here
					iGapStart = 0;
				}
			}
		}
	}
	if( CardsFound<CardsRequired )
		return 0;

	DWORD highestStraight = iValue + CardsPerHand - 1;
	if (highestStraight > Ace)
		highestStraight = Ace;
	m_rgdwKickers[0] = highestStraight;
	m_rgdwKickers[1] = (DWORD)iHighCard;								// highest actual card value in hand
	m_cKickers = 2;
	return MakeKicker();
} /* CheckStraight */

//////////////////////////////////////////////////////////////////////////////

DWORD
Analyzer_t::GetStraight( void )
{
	DWORD dwValue = CheckStraight( m_rgiValues );
	if( dwValue )
	{
		int iValue = GetFirstKicker( dwValue );
		for( int iCard=0; iCard<5; ++iCard )
		{
			HoldCardsByValue( iValue-iCard, 1 );
		}
	}
	return dwValue;
}

//////////////////////////////////////////////////////////////////////////////

DWORD
Analyzer_t::CheckFlush( int cRequired )
{
	int iSuit = 0;
	for( ; iSuit< MaxSuits; ++iSuit )
	{
		if( cRequired<=m_rgiSuits[iSuit] )
			goto check;
	}
	return 0;

check:;
	int rgiValues[MaxValues];
	memset( rgiValues, 0, sizeof(rgiValues) );
			
	// initialize value array for cards of given suit
	for( CardVector_t::const_iterator iter = _cardVector.begin(); iter != _cardVector.end(); ++iter )
	{
		 if( (*iter)->getSuit()==iSuit )
		 {
			rgiValues[(*iter)->getNumber()]++;
		 }
	}

	// BUG: doesn't hold the "highest" flush in a 6+ card game.  we'd need to
	// keep a counter, above, of how many unique rgiValues elements are incremented,
	// then loop through rgiValues something like this:
	//
	// for( int iValue=deuce; 5<counter && ivalue<=Ace; ++iValue )
	// { if( rgiValues[iValues] ) -- counter; }
	//
	// then we can HoldCardsBySuit( iSuit, iValue+1, Ace );

	HoldCardsBySuit( iSuit );
	return iSuit | MakeHighHand( rgiValues );
}

//////////////////////////////////////////////////////////////////////////////

DWORD
Analyzer_t::GetFlush( void )
{
	return CheckFlush();
} 

//////////////////////////////////////////////////////////////////////////////

DWORD
Analyzer_t::GetFullHouse( void )
{
	int iTrips = 0;
	int iPair = 0;
	
	iTrips = CheckThreeKind();
	if( iTrips )
	{
		for( int iValue=Ace; Deuce<=iValue; --iValue )
		{
			if( iValue==iTrips )
				continue;
			
			if( 2<=m_rgiValues[iValue] )
			{
				iPair = iValue;
				break;
			}
		}
	}
	
	if( !iTrips || !iPair )
		return 0;

	int cHeld;
	cHeld = HoldCardsByValue( iTrips, 3 );
	cHeld += HoldCardsByValue( iPair, 2 );
	ASSERT( 5==cHeld );

	m_rgdwKickers[0] = iTrips;
	m_rgdwKickers[1] = iPair;
	m_cKickers = 2;
	return MakeKicker();
} /* GetFullHouse */

//////////////////////////////////////////////////////////////////////////////

DWORD
Analyzer_t::GetFourKind( void )
{
	int iQuads = 0;
	int iKicker = 0;
	for( int iValue=Ace; Deuce<=iValue; --iValue )
	{
		if( m_rgiValues[iValue]==4 && !iQuads )
		{
			iQuads = iValue;
			continue;
		}
		if( m_rgiValues[iValue] && !iKicker )
		{
			iKicker = iValue;
		}
	}
	if( !iQuads )
		return 0;

	int cHeld = HoldCardsByValue( iQuads, 4 );
	ASSERT( 4==cHeld );

	m_rgdwKickers[0] = iQuads;
	m_rgdwKickers[1] = iKicker;
	m_cKickers = 2;
	return MakeKicker();
} /* GetFourKind */

//////////////////////////////////////////////////////////////////////////////

DWORD
Analyzer_t::CheckStraightFlush(
int			cRequired,
int			iGapMax /*=-1*/)
{
	DWORD dwHighStraight = 0;
	DWORD dwHighSuit = 0;

	if( -1==iGapMax )
		iGapMax = 5-cRequired;

	int iSuit = size_t(Suit::Spades);
	for( ; int(Suit::Clubs)<=iSuit; --iSuit )
	{
		// for any suit for which there are required # or more cards
		if( cRequired<=m_rgiSuits[iSuit] )
		{
			// initialize a value array for cards of given suit
			int rgiValues[MaxValues];
			memset( rgiValues, 0, sizeof(rgiValues) );
			for( CardVector_t::const_iterator iter = _cardVector.begin(); iter != _cardVector.end(); ++iter )
			{
				if( (*iter)->getSuit()==iSuit )
					rgiValues[(*iter)->getNumber()]++;
			}
			// determine if values for cards of this suit make a straight
			DWORD dwStraight = CheckStraight( rgiValues, cRequired, iGapMax );
			if( dwStraight>dwHighStraight )
			{
				// a straight was made; save it if its the highest one so far
				dwHighStraight = dwStraight;
				dwHighSuit = iSuit;
			}
		}
	}
	return dwHighSuit | dwHighStraight;
}

//////////////////////////////////////////////////////////////////////////////

DWORD
Analyzer_t::GetStraightFlush( void )
{
	DWORD dwValue = CheckStraightFlush( 5 );
	if( dwValue )
	{
		int iMax = (int)GetFirstKicker( dwValue );
		int iMin = iMax-4;
		int cHeld = HoldCardsBySuit( GetSuit(dwValue), iMin, iMax );
		ASSERT( 5==cHeld );
	}
	return dwValue;
} /* GetStraightFlush */

//////////////////////////////////////////////////////////////////////////////

DWORD
Analyzer_t::GetFiveKind( void )
{
	return 0;
} /* GetFiveKind */

//////////////////////////////////////////////////////////////////////////////

DWORD
Analyzer_t::GetWildRoyalFlush( void )
{
	return 0;
} /* GetWildRoyalFlush */

//////////////////////////////////////////////////////////////////////////////

DWORD
Analyzer_t::CheckRoyalFlush( int cRequired/*=5*/ )
{
	DWORD dwValue = CheckStraightFlush( cRequired );
	if( dwValue )
	{
		if( Ace==GetFirstKicker(dwValue) )
		{
			int cHeld = HoldCardsBySuit( GetSuit(dwValue), 10, Ace );
			ASSERT( cRequired==cHeld );
			return dwValue;
		}
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////////

DWORD
Analyzer_t::GetRoyalFlush( void )
{
	return CheckRoyalFlush();
}

} // namespace Hand
