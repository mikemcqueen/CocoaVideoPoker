/*
 *  BonusPokerBase.cpp
 *  CardLib
 *
 *  Created by Mike McQueen on 4/2/11.
 *  Copyright 2011 Mike McQueen. All rights reserved.
 *
 */

#include "stdafx.h"
#include "Poker.h"
#include "PayScheduleManager.hpp"
#include "Hand.hpp"

namespace PaySchedule
{
namespace BonusPoker
{
    using namespace Hand::Value;
    using namespace PaySchedule::Static::Flag;
        
    namespace Base
    {
        const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 0,      0,      0,
            StraightFlush,              0,      0,      0,

            FourAces,                   0,      0,      0,

        Four2s3s4s,                     0,      0,      Group,
            FourTwos,                   0,      0,      0,
            FourThrees,                 0,      0,      0,
            FourFours,                  0,      0,      Group,
            
        Four5sThruKings,                0,      0,      Group,
            FourFives,                  0,      0,      0,
            FourSixes,                  0,      0,      0,
            FourSevens,                 0,      0,      0,
            FourEights,                 0,      0,      0,
            FourNines,                  0,      0,      0,
            FourTens,                   0,      0,      0,
            FourJacks,                  0,      0,      0,
            FourQueens,                 0,      0,      0,
            FourKings,                  0,      0,      Group,
            
            FullHouse,                  0,      0,      0,
            Flush,                      0,      0,      0,
            Straight,                   0,      0,      0,
            ThreeKind,                  0,      0,      0,
            TwoPair,                    0,      0,      0,
            
        Hand::Value::JacksOrBetter,     0,      0,      Group,
            PairOfJacks,                0,      0,      0,
            PairOfQueens,               0,      0,      0,
            PairOfKings,                0,      0,      0,
            PairOfAces,                 0,      0,      Group,
        };
    }    
} // BonusPoker
} // PaySchedule
