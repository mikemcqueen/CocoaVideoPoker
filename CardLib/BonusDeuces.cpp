/*
 *  DeucesWild.cpp
 *  CardLib
 *
 *  Created by Mike McQueen on 8/27/10.
 *  Copyright 2010 Mike McQueen. All rights reserved.
 *
 */

#include "stdafx.h"
#include "Poker.h"
#include "PaySchedule.hpp"
#include "Hand.hpp"

#include "DeucesWildBase.cpp"

// TODO: has its own base pay table, 4 2s + ace, 6-King

namespace PaySchedule
{
namespace BonusDeuces
{
    void registerPaySchedules()
    {
        Manager_t::registerGame(Game::DeucesWild, string("Deuces Wild"), string("Deuces.dat"));
    }

    using namespace Hand::Value;
    using namespace PaySchedule::Static::Flag;
    
    #define REGISTER    REGISTER_PAY_SCHEDULE2(PaySchedule::Game::DeucesWild, DeucesWild::Base::payLines, payLines, estimatedReturn, 2)

    namespace ps_25_15_9_5 // FullPay 100.76%
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlushNoDeuces,      250,    4000,   0,
            FourDeuces,              200,    0,      0,
            RoyalFlushWithDeuce,     25,     0,      0,
            FiveOfAKind,             15,     0,      0,
            StraightFlush,           9,      0,      0,
            FourOfAKind,             5,      0,      0,
            FullHouse,               3,      0,      0,
            Flush,                   2,      0,      0,
            Straight,                2,      0,      0,
            ThreeKind,               1,      0,      0,
        };
        static const double estimatedReturn = 100.76;
        
        REGISTER;
    }

}
}