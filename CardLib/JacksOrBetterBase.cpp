/*
 *  JacksOrBetterBase.cpp
 *  CardLib
 *
 *  Created by Mike McQueen on 4/02/2011.
 *  Copyright 2011 Mike McQueen. All rights reserved.
 *
 */

#include "stdafx.h"
#include "Poker.h"
#include "PayScheduleManager.hpp"

namespace PaySchedule
{
namespace JacksOrBetter
{
    using namespace Hand::Value;
    using namespace Static::Flag;
        
    namespace Base
    {
        const Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0, 
            StraightFlush,              50,     0,      0, 
            FourOfAKind,                25,     0,      0, 
            FullHouse,                  9,      0,      0, 
            Flush,                      6,      0,      0, 
            Straight,                   4,      0,      0, 
            ThreeKind,                  3,      0,      0, 
            TwoPair,                    2,      0,      0, 
        Hand::Value::JacksOrBetter,     1,      0,      Group,
            PairOfJacks,                0,      0,      0,
            PairOfQueens,               0,      0,      0,
            PairOfKings,                0,      0,      0,
            PairOfAces,                 0,      0,      Group,
        };
    }
} // JacksOrBetter
}

