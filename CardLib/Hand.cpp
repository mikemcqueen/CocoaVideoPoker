//////////////////////////////////////////////////////////////////////////////
//
// HAND.CPP
//
//////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HandAnalyzer_t.hpp"
#include "Card.hpp"
#include "PaySchedule.hpp" // hack kinda
#include <assert.h>

#undef ASSERT
#define ASSERT assert

namespace Hand
{

    
    const std::string&
    getName(Value_t value)
    {
        const static string strUnknown("*** UNKNOWN ***");
        static map<Value_t, string> *valueNameMap = new map<Value_t, string>();
        
        if (valueNameMap->empty())
        {
            using namespace Value;
            
            valueNameMap->insert(make_pair(FourAces_With_234, string("4 ACES WITH ANY 2,3,4")));
            valueNameMap->insert(make_pair(Four2s3s4s_With_Ace234, string("4 2s, 3s, 4s W/ACE,2,3,4")));
            valueNameMap->insert(make_pair(Four2s3s4s, string("4 2s, 3s, 4s")));
            valueNameMap->insert(make_pair(Four5sThruKings, string("4 5s THRU KINGS")));

            valueNameMap->insert(make_pair(RoyalFlushNoDeuces, string("ROYAL FLUSH NO DEUCES")));
            valueNameMap->insert(make_pair(RoyalFlushWithDeuce, string("ROYAL FLUSH WITH DEUCE")));
            
            valueNameMap->insert(make_pair(RoyalFlushNoJokers, string("NATURAL ROYAL FLUSH")));
            valueNameMap->insert(make_pair(RoyalFlushWithJoker, string("WILD ROYAL FLUSH")));

            valueNameMap->insert(make_pair(FiveOfAKind, string("5 OF A KIND"))); 
            valueNameMap->insert(make_pair(RoyalFlush, string("ROYAL FLUSH")));
            valueNameMap->insert(make_pair(StraightFlush, string("STRAIGHT FLUSH")));

            valueNameMap->insert(make_pair(FourOfAKind, string("4 OF A KIND")));
            valueNameMap->insert(make_pair(FourAces, string("4 ACES")));
            valueNameMap->insert(make_pair(FourDeuces, string("4 DEUCES")));
            valueNameMap->insert(make_pair(FullHouse, string("FULL HOUSE")));
            valueNameMap->insert(make_pair(Flush, string("FLUSH")));
            valueNameMap->insert(make_pair(Straight, string("STRAIGHT")));
            valueNameMap->insert(make_pair(ThreeKind, string("3 OF A KIND")));
            valueNameMap->insert(make_pair(TwoPair, string("TWO PAIR")));
            valueNameMap->insert(make_pair(KingsOrBetter, string("KINGS OR BETTER")));
            valueNameMap->insert(make_pair(JacksOrBetter, string("JACKS OR BETTER")));
        }
        map<Value_t, string>::const_iterator it = valueNameMap->find(value);
        if (it != valueNameMap->end())
        {
            return it->second;
        }
        return strUnknown;
    }
    
    Value_t
    getValue(const CardVector_t& cards, const PaySchedule::Lookup::Data_t& schedule)
    {
        Analyzer_t analyzer(cards, schedule.getWildCard());
        return analyzer.GetValue();
    }
   
//////////////////////////////////////////////////////////////////////////////

Analyzer_t::
Analyzer_t(
    const Hand_t&   cardVector,
    Card_t::Value_t wildCardNumber)
    :
	_cardVector(cardVector),
	_heldCardCount(0),
    _wildCardNumber(wildCardNumber),
    _wildCardCount(0),
	m_cKickers(0)
{
}

//////////////////////////////////////////////////////////////////////////////

Analyzer_t::~Analyzer_t()
{
}

//////////////////////////////////////////////////////////////////////////////

bool
Analyzer_t::IsValid() const
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
Analyzer_t::Invalidate()
{

#if 0
	for( int iCard=0; iCard<GetHeldCardCount(); ++iCard )
		GetHeldCard(iCard).Invalidate();
#endif
}

//////////////////////////////////////////////////////////////////////////////

const Card_t&
Analyzer_t::GetHeldCard( int iCard ) const
{
	return *_heldCards[iCard];
}

//////////////////////////////////////////////////////////////////////////////

void
Analyzer_t::HoldCard(const Card_t& card)
{
	_heldCards[_heldCardCount++] = &card;
}

/////////////////////////////////////////////////////////////////////////////

void
Analyzer_t::
InitSuitsValues(
    size_t& maxSuitCount,
    size_t& maxNumberCount,
    size_t& mostPopularNumber,
    size_t& uniqueNumberCount)
{
	memset(m_rgiValues, 0, sizeof(m_rgiValues));
	memset(m_rgiSuits, 0, sizeof(m_rgiSuits));
    maxSuitCount = 0;
    maxNumberCount = 0;
    uniqueNumberCount = 0;
    CardVector_t::const_iterator iter = _cardVector.begin();
	for (;iter != _cardVector.end(); ++iter)
	{
        if (DRAW == iter->getValue())
        {
            continue;
        }
        ASSERT(iter->getNumber() < _countof(m_rgiValues));
        if (iter->getNumber() == _wildCardNumber)
        {
            ++_wildCardCount;
        }
        else
        {
            const size_t numberCount = ++m_rgiValues[iter->getNumber()];
            if (numberCount > maxNumberCount)
            {
                maxNumberCount = numberCount;
                mostPopularNumber = iter->getNumber();
            }
            if (numberCount == 1)
            {
                ++uniqueNumberCount;
            }

            ASSERT(iter->getSuit() < _countof(m_rgiSuits));
            const size_t suitCount = ++m_rgiSuits[iter->getSuit()];
            if (suitCount > maxSuitCount)
            {
                maxSuitCount = suitCount;
            }
        }
	}
	m_rgiValues[AceLow] = m_rgiValues[Ace];
}

//////////////////////////////////////////////////////////////////////////////

size_t
Analyzer_t::
MakeKicker(bool wild /* = false */)
{
    if (0 == m_cKickers)
    {
        return 0;
    }
	size_t dwKicker = 0;
	for( int i=0; i<6; i++ )
	{
		if( i<m_cKickers )
			dwKicker |= m_rgdwKickers[i];
		dwKicker <<= 4;
	}
    size_t flags = (wild) ? Value::Flag::Wild : Value::Flag::None;
	return dwKicker | flags;
}

//////////////////////////////////////////////////////////////////////////////

int
Analyzer_t::
HoldCardsBySuit(
	int iSuit,
	int iValueMin,	/*=Deuce*/
	int iValueMax	/*=Ace*/)
{
	ASSERT( AceLow<=iValueMin && iValueMin<=Ace );
	ASSERT( AceLow<=iValueMax && iValueMax<=Ace );
	ASSERT( iValueMin<=iValueMax );

	bool bAceLow = AceLow==iValueMin;
	int cHeld = 0;
	for( CardVector_t::const_iterator iter = _cardVector.begin(); iter != _cardVector.end(); ++iter )
	{
		int iValue = (int)iter->getNumber();
		if( iter->getSuit()==iSuit )
		{
			if( (iValueMin<=iValue && iValue<=iValueMax) || (bAceLow && Ace==iValue) )
			{
				HoldCard(*iter);
				cHeld++;
			}
		}
	}
	return cHeld;
}

//////////////////////////////////////////////////////////////////////////////

void
Analyzer_t::
HoldAllCards()
{
	for( CardVector_t::const_iterator iter = _cardVector.begin(); iter != _cardVector.end(); ++iter )
	{
		HoldCard(*iter);
	}
}

//////////////////////////////////////////////////////////////////////////////

int
Analyzer_t::
CountCardsBySuit(
    int iSuit,
    int iValueMin,	/*=Deuce*/
    int iValueMax	/*=Ace*/)
{
	ASSERT( AceLow<=iValueMin && iValueMin<=Ace );
	ASSERT( AceLow<=iValueMax && iValueMax<=Ace );
	ASSERT( iValueMin<=iValueMax );

	int cFound = 0;
	for( CardVector_t::const_iterator iter = _cardVector.begin(); iter != _cardVector.end(); ++iter )
	{
		int iValue = (int)iter->getNumber();
		if( iter->getSuit()==iSuit && iValueMin<=iValue && iValue<=iValueMax )
		{
			cFound++;
		}
	}
	return cFound;
}

//////////////////////////////////////////////////////////////////////////////

int
Analyzer_t::CountUniqueValuesInRange(
    int* piValues,
    int  iValueMin,
    int  iValueMax)
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

size_t
Analyzer_t::MakeHighHand( int *piValues )
{
	m_cKickers = 0;
	for( int iValue=Ace; Deuce<=iValue; --iValue )
	{
		if( piValues[iValue] )
		{
			m_rgdwKickers[m_cKickers++] = iValue;
//			if( 5==m_cKickers )
				break;
		}
	}
	return MakeKicker();
}

//////////////////////////////////////////////////////////////////////////////

size_t
Analyzer_t::GetHighCard()
{
	return MakeHighHand( m_rgiValues );
}

//////////////////////////////////////////////////////////////////////////////

int
Analyzer_t::
CheckPair(
    int ignoreValue /* = 0 */)
{
	int iPair = 0;
	for( int iValue = Ace; Deuce <= iValue; --iValue )
	{
        if (iValue == ignoreValue)
        {
            continue;
        }
		if(m_rgiValues[iValue] + _wildCardCount >= 2)
		{
			iPair = iValue;

            int wildCardsUsed = 2 - m_rgiValues[iValue];
            if (wildCardsUsed > 0)
            {
                _wildCardCount -= wildCardsUsed;
            }
            break;
		}
	}
	return iPair;
}

//////////////////////////////////////////////////////////////////////////////

size_t
Analyzer_t::
CheckOnePair()
{
	int iPair = CheckPair();
	if( !iPair )
    {
		return 0;
    }
	
	m_rgdwKickers[0] = iPair;
	m_cKickers = 1;
	
#if 0
	for (int iValue = Ace; Deuce <= iValue; --iValue)
	{
        if ((iValue != iPair) && (0 < m_rgiValues[iValue]))
        {
            m_rgdwKickers[m_cKickers++] = iValue;
            if( 4==m_cKickers )
                break;
        }
	}
    //	ASSERT( 4==m_cKickers ); // NO because wild cards aren't included
#endif

	return MakeKicker();
}

//////////////////////////////////////////////////////////////////////////////

size_t
Analyzer_t::
GetOnePair()
{
	const size_t dwValue = CheckOnePair();
#if HOLDCARDS
    if( dwValue )
	{
        size_t dwKicker = GetFirstKicker( dwValue );
        HoldCardsByValue( dwKicker, 2 );
	}
#endif
	return dwValue;
}

//////////////////////////////////////////////////////////////////////////////

size_t
Analyzer_t::
GetTwoPair()
{
    m_cKickers = 0;
#if 0
	int iHighPair = 0;
	int iLowPair = 0;
	int iKicker = 0;

	for (int iValue = Ace; Deuce <= iValue; --iValue)
	{
		if (m_rgiValues[iValue])
		{
			if (!iHighPair)
			{
				iHighPair = iValue;
			}
			else if (!iLowPair)
			{
				iLowPair = iValue;
			}
		}
        else if ((1 == m_rgiValues[iValue]) && (0 == iKicker))
		{
			iKicker = iValue;
		}
	}

	if (!iHighPair || !iLowPair)
		return 0;
	ASSERT( iKicker );
#endif

    int highPair = CheckPair();
    if (0 != highPair)
    {
        int lowPair = CheckPair(highPair);
        if (0 != lowPair)
        {
            m_rgdwKickers[0] = highPair;
            m_rgdwKickers[1] = lowPair;
            m_cKickers = 2;
        }
    }

#if 0
	int cHeld;
	cHeld = HoldCardsByValue( iHighPair, 2 );
	cHeld += HoldCardsByValue( iLowPair, 2 );
//	ASSERT( 4==cHeld );

	m_rgdwKickers[0] = iHighPair;
	m_rgdwKickers[1] = iLowPair;
	m_rgdwKickers[2] = iKicker;
	m_cKickers = 3;
#endif

	return MakeKicker();
} /* GetTwoPair */

//////////////////////////////////////////////////////////////////////////////

int
Analyzer_t::
CheckThreeKind()
{
	int iTrips = 0;
	for (int iValue = Ace; Deuce <= iValue; --iValue)
	{
        if (m_rgiValues[iValue] + _wildCardCount >= 3)
		{
			iTrips = iValue;

            int wildCardsUsed = 3 - m_rgiValues[iValue];
            if (wildCardsUsed > 0)
            {
                _wildCardCount -= wildCardsUsed;
            }
			break;
		}
	}
	return iTrips;
} /* CheckThreeKind */

//////////////////////////////////////////////////////////////////////////////

size_t
Analyzer_t::
GetThreeKind()
{
#if 1
	int iTrips = CheckThreeKind();
	if (0 == iTrips)
    {
		return 0;
    }
#else
    size_t iTrips = _mostPopularNumber;
#endif
	m_rgdwKickers[0] = iTrips;
	m_cKickers = 1;
#if 0
    for( unsigned iValue=Ace; Deuce<=iValue; --iValue )
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
#endif
	return MakeKicker();
} /* GetThreeKind */

//////////////////////////////////////////////////////////////////////////////

size_t
Analyzer_t::
CheckStraight(
    const int* piValues)
{
    int cardsNeeded = CardsPerHand - _wildCardCount;

    int cardsFound = 0;
    size_t highCard = Ace;
    for(; 5 <= highCard; --highCard, cardsFound = 0)
	{
        for (size_t card = 0; card < CardsPerHand; ++card)
        {
            if (piValues[highCard - card] > 0)
            {
                ++cardsFound;
            }
        }
        if (cardsFound >= cardsNeeded)
        {
            break;
        }
	}
    if (cardsFound >= cardsNeeded)
    {
        m_rgdwKickers[0] = highCard;
        m_cKickers = 1;
        bool wild = CardsPerHand > cardsFound;
        return MakeKicker(wild);
    }
    return 0;
} /* CheckStraight */

//////////////////////////////////////////////////////////////////////////////

size_t
Analyzer_t::
GetStraight()
{
	size_t dwValue = CheckStraight( m_rgiValues );
#if HOLDCARDS
	if( dwValue )
	{
		int iValue = (int)GetFirstKicker( dwValue );
		for( int iCard=0; iCard<5; ++iCard )
		{
			HoldCardsByValue( iValue-iCard, 1 );
		}
	}
#endif
	return dwValue;
}

//////////////////////////////////////////////////////////////////////////////

size_t
Analyzer_t::
MakeFlush(int iSuit)
{
    int rgiValues[MaxValues] = { 0 };
			
    // TODO BUGBUG: doesn't take into account wild cards

	// initialize value array for cards of given suit
	for( CardVector_t::const_iterator iter = _cardVector.begin(); iter != _cardVector.end(); ++iter )
	{
		 if( iter->getSuit() == iSuit )
		 {
			rgiValues[iter->getNumber()]++;
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

#if HOLDCARDS
	HoldCardsBySuit( iSuit );
#endif
	return iSuit | MakeHighHand( rgiValues );
}

//////////////////////////////////////////////////////////////////////////////

size_t
Analyzer_t::
CheckFlush( int cRequired )
{
	int iSuit = 0;
    for( ; iSuit < Suit::Count; ++iSuit )
	{
		if (m_rgiSuits[iSuit] + _wildCardCount >= cRequired)
        {
            return MakeFlush(iSuit);
        }
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////////

size_t
Analyzer_t::
GetFlush()
{
	return CheckFlush();
} 

//////////////////////////////////////////////////////////////////////////////

size_t
Analyzer_t::GetFullHouse()
{
	int iTrips = 0;
	int iPair = 0;
	iTrips = CheckThreeKind();
	if (0 != iTrips)
	{
        iPair = CheckPair(iTrips);
        if (0 != iPair)
        {
            m_rgdwKickers[0] = iTrips;
            m_rgdwKickers[1] = iPair;
            m_cKickers = 2;
        }
    }

#if HOLDCARDS
	int cHeld;
	cHeld = HoldCardsByValue( iTrips, 3 );
	cHeld += HoldCardsByValue( iPair, 2 );
//	ASSERT( 5==cHeld );
#endif

#if 0
	m_rgdwKickers[0] = iTrips;
	m_rgdwKickers[1] = iPair;
	m_cKickers = 2;
#endif
	return MakeKicker();
} /* GetFullHouse */

//////////////////////////////////////////////////////////////////////////////

size_t
Analyzer_t::GetFourKind()
{
	int iQuads = 0;
	int iKicker = 0;
	for( int iValue = Ace; Deuce <= iValue; --iValue )
	{
		if(m_rgiValues[iValue] + _wildCardCount >= 4)
		{
			iQuads = iValue;
		}
        else if ((m_rgiValues[iValue] > 0) && !iKicker)
		{
			iKicker = iValue;
		}
        if (iKicker && iQuads)
        {
            break;
        }
	}
	if( !iQuads )
    {
		return 0;
    }

#if HOLDCARDS
	int cHeld = HoldCardsByValue( iQuads, 4 );
cHeld;
//	ASSERT( 4==cHeld );
#endif

	m_rgdwKickers[0] = iQuads;
	m_rgdwKickers[1] = iKicker;
	m_cKickers = 2;
	return MakeKicker();
} /* GetFourKind */

//////////////////////////////////////////////////////////////////////////////

size_t
Analyzer_t::CheckStraightFlush(
int cRequired,
int unused)
{
    unused = 0;
    
	size_t dwHighStraight = 0;
	size_t dwHighSuit = 0;

	int iSuit = size_t(Suit::Spades);
	for (; int(Suit::Clubs) <= iSuit; --iSuit)
	{
		// for any suit for which there are required # or more cards
		if (m_rgiSuits[iSuit] + _wildCardCount >= cRequired)
		{
			// initialize a value array for cards of given suit
            int rgiValues[MaxValues] = { 0 };
			for (CardVector_t::const_iterator iter = _cardVector.begin(); iter != _cardVector.end(); ++iter)
			{
				if ((iter->getNumber() != _wildCardNumber) && (iter->getSuit() == iSuit))
                {
                    ++rgiValues[iter->getNumber()];
                    if (iter->getNumber() == Ace)
                    {
                        ++rgiValues[AceLow];
                    }
                }
			}
			// determine if values for cards of this suit make a straight
			size_t dwStraight = CheckStraight(rgiValues);
			if (dwStraight > dwHighStraight)
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

size_t
Analyzer_t::GetStraightFlush()
{
	size_t dwValue = CheckStraightFlush( 5 );
#if HOLDCARDS
	if( dwValue )
	{
		int iMax = (int)GetFirstKicker( dwValue );
		int iMin = iMax-4;
		int cHeld = HoldCardsBySuit( (int)GetSuit(dwValue), iMin, iMax );
cHeld;
		ASSERT( 5==cHeld );
	}
#endif
	return dwValue;
} /* GetStraightFlush */

//////////////////////////////////////////////////////////////////////////////

size_t
Analyzer_t::GetRoyalFlush()
{
	return CheckRoyalFlush();
}

//////////////////////////////////////////////////////////////////////////////

size_t
Analyzer_t::CheckRoyalFlush( int cRequired/*=5*/ )
{
	size_t dwValue = CheckStraightFlush( cRequired );
	if( dwValue )
	{
		if (Ace == GetFirstKicker(dwValue))
		{
#if HOLDCARDS
			int cHeld = HoldCardsBySuit( (int)GetSuit(dwValue), 10, Ace );
            cHeld;
			ASSERT( cRequired==cHeld );
#endif
			return dwValue;
		}
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////////

size_t
Analyzer_t::
GetFourDeuces()
{
    m_cKickers = 0;
    if ((_wildCardCount == 4) && (_wildCardNumber == 2))
    {
        m_rgdwKickers[0] = _wildCardNumber;
        m_cKickers = 1;
    }
    return MakeKicker();
} /* GetFourDeuces */

//////////////////////////////////////////////////////////////////////////////

size_t
Analyzer_t::
GetFiveOfAKind()
{
	int fives = 0;
	for( int iValue = Ace; Deuce <= iValue; --iValue )
	{
		if(m_rgiValues[iValue] + _wildCardCount >= 5)
		{
			fives = iValue;
            break;
		}
	}
	if (!fives)
    {
		return 0;
    }

#if HOLDCARDS
	int cHeld = HoldCardsByValue( iQuads, 4 );
cHeld;
//	ASSERT( 4==cHeld );
#endif

	m_rgdwKickers[0] = fives;
	m_cKickers = 1;
	return MakeKicker();
} /* GetFiveOfAKind */

//////////////////////////////////////////////////////////////////////////////

} // namespace Hand
