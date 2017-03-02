/*
 *  DeucesWildBase.cpp
 *  CardLib
 *
 *  Created by Mike McQueen on 4/3/2011.
 *  Copyright 2011 Mike McQueen. All rights reserved.
 *
 */

#include "stdafx.h"
#include "Poker.h"
#include "PaySchedule.hpp"
#include "Hand.hpp"

namespace PaySchedule
{
namespace DeucesWild
{
    using namespace Hand::Value;
    using namespace PaySchedule::Static::Flag;

    namespace Base
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlushNoDeuces,     0,      0,      Group,
            RoyalFlush,             0,      0,      Group | NoWild,

            FourDeuces,             0,      0,      0,

            RoyalFlushWithDeuce,    0,      0,      Group,
            RoyalFlushWild,         0,      0,      Group,

            FiveOfAKind,            0,      0,      0,
            StraightFlush,          0,      0,      0,
            FourOfAKind,            0,      0,      0,
            FullHouse,              0,      0,      0,
            Flush,                  0,      0,      0,
            Straight,               0,      0,      0,
            ThreeKind,              0,      0,      0,
        };
        static const size_t payLineCount = _countof(payLines);
    }

} // DeucesWild
}
