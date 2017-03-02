////////////////////////////////////////////////////////////////////////////////
//
// PaySchedule.cpp
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Poker.h"
#include "PaySchedule.hpp"
#include "PayScheduleManager.hpp"

using namespace std;

namespace PaySchedule
{
namespace Lookup
{

Data_t::
Data_t(
    PaySchedule::Game::Id_t gameId,
    const Static::Payline_t*  paylines,
          size_t            paylineCount,
          size_t            coinCount,
          double            payout)
:
    _gameId(gameId),
    _staticSchedule(paylines, paylineCount),
    _payout(payout),
    _wildCard(0)
{
    initialize(coinCount);
    
    Manager_t::registerPaySchedule(_gameId, this);
}

////////////////////////////////////////////////////////////////////////////////

Data_t::
Data_t(
    PaySchedule::Game::Id_t gameId,
    const Static::Payline_t*  paylines,
          size_t            paylineCount,
    const Static::Payline_t*  paylineOverrides,
          size_t            paylineOverrideCount,
          size_t            coinCount,
          double            payout,
          uint8_t           wildCard)
:
    _gameId(gameId),
    _staticSchedule(paylines, paylineCount),
    _payout(payout),
    _wildCard(wildCard)
{
    initialize(coinCount);
    
    for (size_t paylineOverrideIndex = 0; paylineOverrideIndex < paylineOverrideCount; ++paylineOverrideIndex)
    {
        const Static::Payline_t& staticPayline = paylineOverrides[paylineOverrideIndex];
        unsigned paylineIndex = getPaylineIndex(staticPayline._handValue);
        updatePayline(paylineIndex, staticPayline, coinCount);
    }
    
    Manager_t::registerPaySchedule(_gameId, this);
}

////////////////////////////////////////////////////////////////////////////////

Data_t::
~Data_t()
{
}

////////////////////////////////////////////////////////////////////////////////

void
Data_t::
initialize(
    size_t coinCount)
{
    _lowestHandValue = Hand::Value::RoyalFlush;
    _paylineCount = 0;
    bool inGroup = false;
    
    for(unsigned staticPaylineIndex = 0; staticPaylineIndex < _staticSchedule._paylines.size(); ++staticPaylineIndex)
    {
        const Static::Payline_t& staticPayline = _staticSchedule._paylines[staticPaylineIndex];
        const bool groupFlag = 0 != (staticPayline._flags & Static::Flag::Group);

        // if this payline is in a group, or is not the first element of a group (group header)
        if (inGroup || !groupFlag)
        {
            if (staticPayline._handValue < _lowestHandValue)
            {
                _lowestHandValue = staticPayline._handValue;
            }
        }
        
        if (!inGroup)
        {
            push_back(make_pair(staticPayline._handValue, Payline_t(coinCount)));
            updatePayline(_paylineCount, staticPayline, coinCount);

            ++_paylineCount;
            inGroup = groupFlag;
        }
        else if (groupFlag)
        {
            inGroup = false;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void
Data_t::
updatePayline(
          unsigned         paylineIndex,
    const Static::Payline_t& staticPayline,
          unsigned         coinCount)
{
    Payline_t& payline = getPayline(paylineIndex);
    size_t totalPayout = 0;
    for (size_t coin = 1; coin <= coinCount; ++coin)
    {
        if ((coinCount == coin) && (0 != staticPayline._jackpot))
        {
            totalPayout = staticPayline._jackpot;
        }
        else
        {
            totalPayout += staticPayline._payout;
        }
        payline[coin - 1] = totalPayout;
    }
}
    
////////////////////////////////////////////////////////////////////////////////

uint32_t
Data_t::
getPaylineIndex(
    const Hand::Value_t handValue) const
{
    for (size_t index = 0; index < size(); ++index)
    {
        if (at(index).first == handValue)
        {
            return index;
        }
    }
    printf("handValue(%ld)\n", handValue);
    throw std::invalid_argument("getPayLineIndex() bad handValue");
}

////////////////////////////////////////////////////////////////////////////////

const Payline_t&
Data_t::
getPayline(
    uint32_t paylineIndex) const
{
    return getPaylinePair(paylineIndex).second;
}

////////////////////////////////////////////////////////////////////////////////

Payline_t&
Data_t::
getPayline(
    uint32_t paylineIndex)
{
    return getPaylinePair(paylineIndex).second;
}

////////////////////////////////////////////////////////////////////////////////

Hand::Value_t
Data_t::
getPaylineHandValue(
    uint32_t paylineIndex) const
{
    return getPaylinePair(paylineIndex).first;
}

////////////////////////////////////////////////////////////////////////////////

const PaylinePair_t&
Data_t::
getPaylinePair(
    uint32_t paylineIndex) const
{

    if (paylineIndex >= size())
    {
        printf("paylineIndex(%d)\n", paylineIndex);
        throw invalid_argument("getPaylinePair() bad index");
    }

    return at(paylineIndex);
}

////////////////////////////////////////////////////////////////////////////////

PaylinePair_t&
Data_t::
getPaylinePair(
    uint32_t paylineIndex)
{
    if (paylineIndex >= size())
    {
        printf("paylineIndex(%d)\n", paylineIndex);
        throw invalid_argument("getPaylinePair() bad index");
    }

    return at(paylineIndex);
}

////////////////////////////////////////////////////////////////////////////////
// todo: return double, bool& inDollars

uint32_t
Data_t::
getPayout(
    uint32_t paylineIndex,
    uint32_t coinCount) const
{
    const Payline_t& payline = getPayline(paylineIndex);
    if ((0 == coinCount) || (coinCount > payline.size()))
    {
        throw invalid_argument("getPayout() bad coinCount");
    }
    return payline[coinCount - 1];
}

////////////////////////////////////////////////////////////////////////////////

Hand::Value_t
Data_t::
oldMatch(
      const Hand::Value_t handValue) const
{
    const_reverse_iterator it = rbegin();
    for (; rend() != it; ++it)
    {   
        if (Hand::isExact(handValue))
        {
            if (handValue == it->first)
            {
                return it->first;
            }
            else if (handValue < it->first)
            {
                return 0;
            } 
            else
            {
                    // continue;
            }
        }
        else if (handValue < it->first)
        {
            if (rbegin() != it)
            {
                break;
            }
            else
            {
                return 0;
            }
        }
        else if (handValue == it->first)
        {
            return it->first;
        }
        else
        {
                // continue;
        }
    }
        //    ASSERT(GETHANDRANK((it + 1)->first) == GETHANDRANK(handValue));
    return (--it)->first;
        //    printf("Data_t::Match(): handValue(0x%08x) schedule.FirstHandValue(0x%08x)\n", handValue, begin()->first);
        //    throw logic_error("hand value is greater than highest hand in schedule!?");
}

////////////////////////////////////////////////////////////////////////////////

Hand::Value_t
Data_t::
Match(
    const Hand::Value_t handValue) const
{
    if (handValue < _lowestHandValue)
    {
        return 0;
    }
    
    Hand::Value_t bestMatch = 0;

    bool inGroup = false;
    bool waitingForGroup = false;
    
    Static::PaylineVector_t::const_reverse_iterator it = _staticSchedule._paylines.rbegin();
    for (; _staticSchedule._paylines.rend() != it; ++it)
    {   
        bool groupFlag = 0 != (it->_flags & Static::Flag::Group);
        
        if (groupFlag)
        {
            inGroup = !inGroup;
        }
        
        if (waitingForGroup)
        {
            if (groupFlag)
            {
                bestMatch = it->_handValue;
                waitingForGroup = false;
            }
            continue;
        }
        
        if (equalHandValues(*it, handValue))
        {
            if (!inGroup)
            {
                bestMatch = it->_handValue;
            }
            else
            {
                waitingForGroup = true;
            }
        }
    }
    return bestMatch;
}

///////////////////////////////////////////////////////////////////////////////

bool
Data_t::
equalHandValues(
    const Static::Payline_t& staticPayline,
    Hand::Value_t    matchValue) const
{
    using namespace Hand;

    Hand::Value_t scheduleValue = staticPayline._handValue;

    if (getRank(scheduleValue) != getRank(matchValue))
    {   
        return false;
    }
    
    if (staticPayline.testFlag(Static::Flag::NoWild) && isWild(matchValue))
    {
        return false;
    }
    
    if (isSuited(scheduleValue) && (GetSuit(scheduleValue) != GetSuit(matchValue)))
    {
        return false;
    }

    size_t firstKicker = GetFirstKicker(scheduleValue);
    if (0 != firstKicker)
    {
        if (firstKicker != GetFirstKicker(matchValue))
        {
            return false;
        }

        size_t secondKicker = GetSecondKicker(scheduleValue);
        if (0 != secondKicker)
        {
            if (secondKicker != GetSecondKicker(matchValue))
            {
                return false;
            }

            size_t thirdKicker = GetThirdKicker(scheduleValue);
            if (0 != thirdKicker)
            {
                if (thirdKicker != GetThirdKicker(matchValue))
                {
                    return false;
                }
            }
        }
    }
    return true;
}
    
const string&
Data_t::
getGameName() const
{
    return Manager_t::getGameName(getGameId());
}

///////////////////////////////////////////////////////////////////////////////

bool operator==(const PaylinePair_t& vpPair, const Hand::Value_t& v)
{
    return v == vpPair.first;
}

} // namespace Lookup
} // namespace PaySchedule

///////////////////////////////////////////////////////////////////////////////
