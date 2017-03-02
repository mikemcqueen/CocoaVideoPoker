//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Hand.hpp"
#include "HandAnalyzer_t.hpp"
#include "Card.hpp"

namespace Hand
{

//////////////////////////////////////////////////////////////////////////////

Value_t
Analyzer_t::
GetValue()
{
    static struct HandValue_t
    {
        size_t handValue;
        size_t sameSuitCount;
        size_t sameNumberCount;
        size_t uniqueNumberCount;
        bool   bWild;
        size_t (Analyzer_t::*fn)(void);
    } rgHandValues[] =
    {
        Value::FourOfAKind,         0, 0, 0, true,    &Analyzer_t::GetFourDeuces,
//        Value::RoyalFlush,          5, 0, 5, false,   &Analyzer_t::GetRoyalFlush,      // is this even necessary?
        Value::StraightFlush,		5, 0, 5, false,   &Analyzer_t::GetStraightFlush,
        Value::FiveOfAKind,         0, 0, 0, true,    &Analyzer_t::GetFiveOfAKind,
        Value::FourOfAKind,			0, 4, 2, false,   &Analyzer_t::GetFourKind,
        Value::FullHouse,			0, 3, 2, false,   &Analyzer_t::GetFullHouse,
        Value::Flush,				5, 0, 5, false,   &Analyzer_t::GetFlush,
        Value::Straight,			0, 0, 5, false,   &Analyzer_t::GetStraight,
        Value::ThreeKind,			0, 3, 3, false,   &Analyzer_t::GetThreeKind,
        Value::TwoPair,		    	0, 2, 3, false,   &Analyzer_t::GetTwoPair,
        Value::OnePair,		    	0, 2, 4, false,   &Analyzer_t::GetOnePair,
    };

    size_t maxSuitCount;
    size_t maxNumberCount;
    size_t uniqueNumberCount;
	InitSuitsValues(maxSuitCount, maxNumberCount, _mostPopularNumber, uniqueNumberCount);
    
    int wildCardCount = _wildCardCount;

	for (int iHand = 0; iHand < _countof(rgHandValues); ++iHand)
    {
        _wildCardCount = wildCardCount;
//        if (rgHandValues[iHand].bWild)
//			continue;
		
        const HandValue_t& hv = rgHandValues[iHand];

// can be a little more clever here, if wildcardCount, use bWild from above and determine if count + wildcardCoutn > same Number/suit cout, 
// a second giant if block basically, switched on (if wildcardCount)

        if (wildCardCount || 
            (((hv.sameSuitCount == 0)  || (hv.sameSuitCount == maxSuitCount)) &&
            ((hv.sameNumberCount == 0) || (hv.sameNumberCount == maxNumberCount)) &&
            ((hv.uniqueNumberCount == 0) || (hv.uniqueNumberCount == uniqueNumberCount))))
        {
            size_t cardValue = (this->*hv.fn)();
            if (cardValue > 0)
            {
#if 0
extern bool showDots;
                if (showDots && hv.handValue == Value::StraightFlush)
                {
                    printf("\nsf: %s  - high = %d\n", Hand::Analyzer_t::GetString(_cardVector), GetFirstKicker(cardValue));
                }
#endif
                return hv.handValue | cardValue;
            }
        }
	}
    return 0;
    //throw std::logic_error("FiveDraw::GetValue() no hand found");
} /* GetValue */

} // namespace Hand
