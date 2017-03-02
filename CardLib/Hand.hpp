//////////////////////////////////////////////////////////////////////////////
//
// HAND.HPP
// 
//////////////////////////////////////////////////////////////////////////////

#if _MSC_VER >= 1000
#pragma once
#endif
#ifndef Include_HAND_HPP
#define Include_HAND_HPP

#include "Poker.h"
#include "Card.hpp"

typedef CardVector_t Hand_t;

namespace PaySchedule
{
    namespace Lookup
    {
        class Data_t;
    }
}

//struct PayScheduleColumn_t;

static const size_t MaxCards = 5;
static const size_t	CardsPerHand = 5;

using namespace Poker::Card;

//////////////////////////////////////////////////////////////////////////////

namespace Hand
{

#define COMMUNITY_CARDS_MAX					5

    
    namespace Rank
    {
        enum e
        {
            HighCard       = 0,
            OnePair,       // 1
            TwoPair,       // 2
            ThreeKind,     // 3
            Straight,      // 4
            Flush,         // 5
            FullHouse,     // 6
            FourKind,      // 7
            StraightFlush, // 8
            FiveKind,      // 9
            
            Group          = 15
        };
    }
    
//////////////////////////////////////////////////////////////////////////////
// TODO: move this all into a Hand::Value_t class
    
namespace Value
{
    namespace Flag
    {
        enum e
        {
            None   = 0,
            /* 1 - 7 = suits */
            Exact  = 0x08,
            Suited = 0x10,
            Wild   = 0x20,
        };
    }
    typedef Flag::e Flag_t;
}

    
typedef size_t Value_t;
    
__inline size_t	 GetFirstKicker(size_t dwHandValue)			{ return (dwHandValue >> 24) & 0xF; }
__inline size_t	 GetSecondKicker(size_t dwHandValue)	  	{ return (dwHandValue >> 20) & 0xF; }
__inline size_t	 GetThirdKicker(size_t dwHandValue)			{ return (dwHandValue >> 16) & 0xF; }
__inline size_t  getRank(Value_t value)                     { return (value >> 28) & 0xf; }
__inline size_t	 GetSuit(size_t dwHandValue)			    { return dwHandValue & 0x7;	 }
__inline bool	 isExact(Value_t value)	                    { return 0 != (value & Value::Flag::Exact);  }
__inline bool    isWild(Value_t value)                      { return 0 != (value & Value::Flag::Wild);  }
__inline bool    isSuited(Value_t value)                    { return 0 != (value & Value::Flag::Suited);  }
    
#define MAKEFIRSTKICKER(cardValue)                          (cardValue << 24)
#define MAKESECONDKICKER(cardValue)                         (cardValue << 20)
#define MAKEKICKERS(first, second)                          (MAKEFIRSTKICKER(first) | MAKESECONDKICKER(second))
#define	MAKEVALUE(handValue, cardValues)             	    ((size_t(handValue) << 28) | cardValues)
#define	MAKESUITEDVALUE(handValue, cardValues, suit)	    ((size_t(handValue) << 28) | cardValues | suit | Flag::Suited)
#define	MAKEEXACTVALUE(handValue, cardValues)               ((size_t(handValue) << 28) | cardValues | Flag::Exact)
#define HANDRANK(dwHandValue)                               ((size_t(dwHandValue) >> 28) & 0x0000000f)
#define MAKEGROUPVALUE(groupValue)                          MAKEVALUE(Rank::Group, groupValue)
    

const std::string& getName(Value_t value);
Value_t            getValue(const CardVector_t& cards, const PaySchedule::Lookup::Data_t& schedule);

//////////////////////////////////////////////////////////////////////////////

enum Job // Hand::Job -- get it?
{        
    StraightDraw = 12,
    FlushDraw,
    StraightFlushDraw,
};

namespace Value
{
    static const Value_t FourAces_With_234      = MAKEGROUPVALUE(1);
    static const Value_t Four2s3s4s_With_Ace234 = MAKEGROUPVALUE(2);
    static const Value_t Four2s3s4s             = MAKEGROUPVALUE(3);
    static const Value_t Four5sThruKings        = MAKEGROUPVALUE(4);
    static const Value_t RoyalFlushNoDeuces     = MAKEGROUPVALUE(5);
    static const Value_t RoyalFlushWithDeuce    = MAKEGROUPVALUE(6);
    static const Value_t JacksOrBetter          = MAKEGROUPVALUE(7);
    static const Value_t KingsOrBetter          = MAKEGROUPVALUE(8);
    static const Value_t RoyalFlushNoJokers     = MAKEGROUPVALUE(9);
    static const Value_t RoyalFlushWithJoker    = MAKEGROUPVALUE(10);
    
    static const Value_t FiveOfAKind            = MAKEVALUE(Rank::FiveKind, 0);
    static const Value_t RoyalFlush             = MAKEVALUE(Rank::StraightFlush, MAKEFIRSTKICKER(Ace));
    static const Value_t RoyalFlushWild         = MAKEVALUE(Rank::StraightFlush, MAKEFIRSTKICKER(Ace)) | Flag::Wild;
    static const Value_t RoyalFlush_Clubs       = MAKESUITEDVALUE(Rank::StraightFlush, MAKEFIRSTKICKER(Ace), Suit::Clubs);    
    static const Value_t RoyalFlush_Diamonds    = MAKESUITEDVALUE(Rank::StraightFlush, MAKEFIRSTKICKER(Ace), Suit::Diamonds);
    static const Value_t RoyalFlush_Hearts      = MAKESUITEDVALUE(Rank::StraightFlush, MAKEFIRSTKICKER(Ace), Suit::Hearts);
    static const Value_t RoyalFlush_Spades      = MAKESUITEDVALUE(Rank::StraightFlush, MAKEFIRSTKICKER(Ace), Suit::Spades);
    static const Value_t StraightFlush          = MAKEVALUE(Rank::StraightFlush, 0);
    static const Value_t FourAces_With_Two      = MAKEEXACTVALUE(Rank::FourKind, MAKEKICKERS(Ace, 2));
    static const Value_t FourAces_With_Three    = MAKEEXACTVALUE(Rank::FourKind, MAKEKICKERS(Ace, 3));
    static const Value_t FourAces_With_Four     = MAKEEXACTVALUE(Rank::FourKind, MAKEKICKERS(Ace, 4));
    static const Value_t FourTwos_With_Ace      = MAKEEXACTVALUE(Rank::FourKind, MAKEKICKERS(2, Ace));
    static const Value_t FourTwos_With_Three    = MAKEEXACTVALUE(Rank::FourKind, MAKEKICKERS(2, 3));
    static const Value_t FourTwos_With_Four     = MAKEEXACTVALUE(Rank::FourKind, MAKEKICKERS(2, 4));
    static const Value_t FourThrees_With_Ace    = MAKEEXACTVALUE(Rank::FourKind, MAKEKICKERS(3, Ace));
    static const Value_t FourThrees_With_Two    = MAKEEXACTVALUE(Rank::FourKind, MAKEKICKERS(3, 2));
    static const Value_t FourThrees_With_Four   = MAKEEXACTVALUE(Rank::FourKind, MAKEKICKERS(3, 4));
    static const Value_t FourFours_With_Ace     = MAKEEXACTVALUE(Rank::FourKind, MAKEKICKERS(4, Ace));
    static const Value_t FourFours_With_Two     = MAKEEXACTVALUE(Rank::FourKind, MAKEKICKERS(4, 2));
    static const Value_t FourFours_With_Three   = MAKEEXACTVALUE(Rank::FourKind, MAKEKICKERS(4, 3));
    static const Value_t FourAces               = MAKEEXACTVALUE(Rank::FourKind, MAKEFIRSTKICKER(Ace));
    static const Value_t FourTwos               = MAKEEXACTVALUE(Rank::FourKind, MAKEFIRSTKICKER(2));
    static const Value_t FourDeuces             = FourTwos;
    static const Value_t FourThrees             = MAKEEXACTVALUE(Rank::FourKind, MAKEFIRSTKICKER(3));
    static const Value_t FourFours              = MAKEEXACTVALUE(Rank::FourKind, MAKEFIRSTKICKER(4));
    static const Value_t FourFives              = MAKEEXACTVALUE(Rank::FourKind, MAKEFIRSTKICKER(5));
    static const Value_t FourSixes              = MAKEEXACTVALUE(Rank::FourKind, MAKEFIRSTKICKER(6));
    static const Value_t FourSevens             = MAKEEXACTVALUE(Rank::FourKind, MAKEFIRSTKICKER(7));
    static const Value_t FourEights             = MAKEEXACTVALUE(Rank::FourKind, MAKEFIRSTKICKER(8));
    static const Value_t FourNines              = MAKEEXACTVALUE(Rank::FourKind, MAKEFIRSTKICKER(9));
    static const Value_t FourTens               = MAKEEXACTVALUE(Rank::FourKind, MAKEFIRSTKICKER(10));
    static const Value_t FourJacks              = MAKEEXACTVALUE(Rank::FourKind, MAKEFIRSTKICKER(Jack));
    static const Value_t FourQueens             = MAKEEXACTVALUE(Rank::FourKind, MAKEFIRSTKICKER(Queen));
    static const Value_t FourKings              = MAKEEXACTVALUE(Rank::FourKind, MAKEFIRSTKICKER(King));
    static const Value_t FourOfAKind            = MAKEEXACTVALUE(Rank::FourKind, 0);
    static const Value_t FullHouse              = MAKEVALUE(Rank::FullHouse, 0);
    static const Value_t Flush                  = MAKEVALUE(Rank::Flush, 0);
    static const Value_t Straight               = MAKEVALUE(Rank::Straight, 0);
    static const Value_t ThreeKind              = MAKEVALUE(Rank::ThreeKind, 0);
    static const Value_t TwoPair                = MAKEVALUE(Rank::TwoPair, 0);
    static const Value_t PairOfJacks            = MAKEVALUE(Rank::OnePair, MAKEFIRSTKICKER(Jack));
    static const Value_t PairOfQueens           = MAKEVALUE(Rank::OnePair, MAKEFIRSTKICKER(Queen));
    static const Value_t PairOfKings            = MAKEVALUE(Rank::OnePair, MAKEFIRSTKICKER(King));
    static const Value_t PairOfAces             = MAKEVALUE(Rank::OnePair, MAKEFIRSTKICKER(Ace));
    static const Value_t OnePair                = MAKEVALUE(Rank::OnePair, 0);
        //    static const Value_t HighCard       = MAKEVALUE(Rank::HighCard, 0);
}

#if 1
//#define HoldCardsByValue(x) x;
#define HoldCardsByValue(x, y) (x); (y);
#endif

} // namespace Hand

//////////////////////////////////////////////////////////////////////////////

#endif // Include _HAND_HPP

//////////////////////////////////////////////////////////////////////////////
