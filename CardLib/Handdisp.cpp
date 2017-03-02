//////////////////////////////////////////////////////////////////////////////
//
// HANDDISP.CPP
//
//////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Card.hpp"
#include "HandAnalyzer_t.hpp"

#ifdef WIN32

#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#pragma warning(push)
#pragma warning(disable:4996)
#include <boost/algorithm/string.hpp>
#pragma warning(pop)
#include <boost/algorithm/string/predicate.hpp>

using namespace boost;
#endif

//////////////////////////////////////////////////////////////////////////////

namespace Hand
{

    using namespace Value;
    
#define	HAND_VALUE_MASK		0xF0000000
    
        // hand name flags
    
#define	HNF_VALUE			1
#define	HNF_VALUEVALUE		2
#define	HNF_VALUESUIT 		3
#define	HNF_SUIT			4
    
    static const struct
    {
        Hand::Value_t   dwHandValue;
        char const *	pHandNameFmt;
        int				iType;
    } rgHandNames[] =
    {
            //    HighCard,			"Nothing, %s-high",					HNF_VALUE,
        OnePair,			"a pair of %ss",					HNF_VALUE,
        TwoPair,			"Two Pair, %ss and %ss",			HNF_VALUEVALUE,
        ThreeKind,			"Trip %ss",							HNF_VALUE,
        Straight,			"a %s-high Straight",				HNF_VALUE,
        Flush,				"a %s-high Flush",					HNF_VALUE,
        FullHouse,			"a Full House, %ss full of %ss",	HNF_VALUEVALUE,
        FourOfAKind,		"Quad %ss",							HNF_VALUE,
        StraightFlush,		"a %s-high Straight Flush",			HNF_VALUE,
        FiveOfAKind,	    "Five %ss",							HNF_VALUE,
        RoyalFlushWithDeuce,	"a wild Royal Flush",				HNF_SUIT,
        RoyalFlushWithJoker,	"a wild Royal Flush",				HNF_SUIT,
        RoyalFlush,			"a Royal Flush",					HNF_SUIT,
//        FourWild,			"Four Wild %ss",					HNF_VALUE,
            //        StraightDraw,		"a %s-high Straight draw",			HNF_VALUE,
            //        FlushDraw,			"a %s-high Flush draw",				HNF_VALUE,
            //        StraightFlushDraw,	"a %s-high Straight Flush draw",	HNF_VALUE,
    };
    
        //////////////////////////////////////////////////////////////////////////////
    
    const char*
    Analyzer_t::GetValueString(Value_t dwHandValue)
    {
        static char szHandValue[255];
        Value_t dwValue = (dwHandValue>>28)&0x0000000f;
        ASSERT( dwValue<HAND_NUMVALUES );
        ASSERT( rgHandNames[dwValue].dwHandValue==dwValue );
        
        dwHandValue &= ~HAND_VALUE_MASK;
        
        char const *pNameFmt = rgHandNames[dwValue].pHandNameFmt;
        
        switch( rgHandNames[dwValue].iType )
        {
            case HNF_VALUE:
                sprintf_s( szHandValue, _countof(szHandValue), pNameFmt, ::GetValueString(GetFirstKicker((int)dwHandValue)) );
                break;
                
            case HNF_VALUEVALUE:
                sprintf_s( szHandValue, _countof(szHandValue), pNameFmt, ::GetValueString(GetFirstKicker((int)dwHandValue)),
                         ::GetValueString(GetSecondKicker((int)dwHandValue)) );
                break;
                
#if 0
            case HNF_VALUESUIT:
                printf( pNameFmt, ::GetValueString(LOWORD(dwHandValue)),
                       ::GetSuitString(HIWORD(dwHandValue)) );
                break;
#endif
                
            case HNF_SUIT:
                strcpy_s( szHandValue, _countof(szHandValue), pNameFmt );
                break;
                
            default:
                sprintf_s( szHandValue, _countof(szHandValue), "GetHandValueString Error: Value is %d, dwHandValue = %08X", (int)dwValue, (int)dwHandValue );
                ASSERT( 0 );
                break;
        }
        return szHandValue;
    } /* GetValueString */
        

//////////////////////////////////////////////////////////////////////////////

void
Analyzer_t::
DisplayValue(
    size_t dwHandValue)
{
	puts( GetValueString(dwHandValue) );
} /* DisplayValue */

//////////////////////////////////////////////////////////////////////////////

void
Analyzer_t::
Display( void )
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
const char*
Analyzer_t::
GetString(
    const CardVector_t& cardVector)
{
// todo: GetString(&cardVector[0], cardVector.size());
	static char szHand[255];
	szHand[0] = 0;
    CardVector_t::const_iterator iter = cardVector.begin();
    for (; iter != cardVector.end(); ++iter)
	{
		if (szHand[0])
		{
			strcat(szHand, " ");
		}
		strcat(szHand, GetShortDisplayString(*iter));
	}
	return szHand;
}

//////////////////////////////////////////////////////////////////////////////

const char*
Analyzer_t::
GetString(
    const size_t* cards,
          size_t  cardCount)
{
	static char szHand[255];
	szHand[0] = 0;
    for (size_t card = 0; card < cardCount; ++card )
	{
		if (szHand[0])
		{
			strcat(szHand, " ");
		}
		strcat(szHand, GetShortDisplayString(cards[card]));
	}
	return szHand;
}

//////////////////////////////////////////////////////////////////////////////

#if 0
char*
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
#endif

//////////////////////////////////////////////////////////////////////////////

#ifdef WIN32

/*static*/
size_t
Analyzer_t::
AddCards(
          CardVector_t& hand,
    const std::string&  handString)
{
    std::string src(handString);
    transform(src.begin(), src.end(), src.begin(), (int(*)(int))tolower);
    using namespace boost;
    size_t count = 0;
    char_separator<wchar_t> sep(L" ");
    tokenizer<char_separator<wchar_t>, std::string::const_iterator, std::string> tokens(src, sep);
    BOOST_FOREACH(std::string str, tokens)
    {
        if (AddCard(hand, str))
        {
            ++count;
        }
        else
        {
            printf("Ivalid card: %s\n", str.c_str());
        }
    }
    return count;
}

//////////////////////////////////////////////////////////////////////////////

/*static*/
bool
Analyzer_t::
AddCard(
          CardVector_t& hand,
    const std::string&  cardString)
{
    size_t num = 0;
    if (isdigit(cardString[0]))
    {
        num = atoi(&cardString[0]);
    }
    else
    {
        static const char bigCards[] = "tjqka";
        const char* card = std::find(&bigCards[0], &bigCards[5], cardString[0]);
        if (card < &bigCards[5])
        {
            num = 10 + (card - &bigCards[0]);
        }
    }
    if (2 <= num && num <= Ace)
    {
        static const char suits[] = "cdhs";
        const char* suit = std::find(&suits[0], &suits[4], cardString[1]);
        if (suit < &suits[4])
        {
            hand.push_back(Card_t((suit - &suits[0]) + 4 * (num - 2)));
            return true;
        }
    }
    return false;
}

#endif

//////////////////////////////////////////////////////////////////////////////

} // Hand

//////////////////////////////////////////////////////////////////////////////
