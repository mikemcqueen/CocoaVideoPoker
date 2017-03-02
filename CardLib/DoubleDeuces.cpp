/*
 *  DoubleDeuces.cpp
 *
 *  Copyright 2011 Mike McQueen. All rights reserved.
 */

#include "stdafx.h"
#include "Poker.h"
#include "PaySchedule.hpp"
#include "Hand.hpp"

#include "DeucesWildBase.cpp"

namespace PaySchedule
{
namespace DoubleDeuces
{
    void registerPaySchedules()
    {
        Manager_t::registerGame(Game::DoubleDeuces, string("Double Deuces"), string("Deuces.dat"));
    }

    using namespace Hand::Value;
    
    #define REGISTER    REGISTER_PAY_SCHEDULE2(PaySchedule::Game::DoubleDeuces, DeucesWild::Base::payLines, payLines, estimatedReturn, 2)

    namespace ps_100_95
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlushNoDeuces,      250,    4000,   0,
            FourDeuces,              400,    0,      0,
            RoyalFlushWithDeuce,     20,     0,      0,
            FiveOfAKind,             10,     0,      0,
            StraightFlush,           10,     0,      0,
            FourOfAKind,             4,      0,      0,
            FullHouse,               4,      0,      0,
            Flush,                   3,      0,      0,
            Straight,                2,      0,      0,
            ThreeKind,               1,      0,      0,
        };
        static const double estimatedReturn = 100.95;
        REGISTER;
    }

    namespace ps_100_92 // full pay? but 4700
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlushNoDeuces,      250,    4700,   0,
            FourDeuces,              400,    0,      0,
            RoyalFlushWithDeuce,     25,     0,      0,
            FiveOfAKind,             16,     0,      0,
            StraightFlush,           13,     0,      0,
            FourOfAKind,             4,      0,      0,
            FullHouse,               3,      0,      0,
            Flush,                   2,      0,      0,
            Straight,                2,      0,      0,
            ThreeKind,               1,      0,      0,
        };
        static const double estimatedReturn = 100.92;
        REGISTER;
    }

    namespace ps_100_61 // full pay?
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlushNoDeuces,      250,    4000,   0,
            FourDeuces,              400,    0,      0,
            RoyalFlushWithDeuce,     25,     0,      0,
            FiveOfAKind,             16,     0,      0,
            StraightFlush,           13,     0,      0,
            FourOfAKind,             4,      0,      0,
            FullHouse,               3,      0,      0,
            Flush,                   2,      0,      0,
            Straight,                2,      0,      0,
            ThreeKind,               1,      0,      0,
        };
        static const double estimatedReturn = 100.61;
        REGISTER;
    }

    namespace ps_100_09 
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlushNoDeuces,      250,    4000,   0,
            FourDeuces,              400,    0,      0,
            RoyalFlushWithDeuce,     25,     0,      0,
            FiveOfAKind,             16,     0,      0,
            StraightFlush,           12,     0,      0,
            FourOfAKind,             4,      0,      0,
            FullHouse,               3,      0,      0,
            Flush,                   2,      0,      0,
            Straight,                2,      0,      0,
            ThreeKind,               1,      0,      0,
        };
        static const double estimatedReturn = 100.09;
        REGISTER;
    }

    namespace ps_99_94
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlushNoDeuces,      250,    4700,   0,
            FourDeuces,              400,    0,      0,
            RoyalFlushWithDeuce,     25,     0,      0,
            FiveOfAKind,             16,     0,      0,
            StraightFlush,           11,     0,      0,
            FourOfAKind,             4,      0,      0,
            FullHouse,               3,      0,      0,
            Flush,                   2,      0,      0,
            Straight,                2,      0,      0,
            ThreeKind,               1,      0,      0,
        };
        static const double estimatedReturn = 99.94;
        REGISTER;
    }

    namespace ps_99_78
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlushNoDeuces,      250,    4000,   0,
            FourDeuces,              400,    0,      0,
            RoyalFlushWithDeuce,     25,     0,      0,
            FiveOfAKind,             15,     0,      0,
            StraightFlush,           12,     0,      0,
            FourOfAKind,             4,      0,      0,
            FullHouse,               3,      0,      0,
            Flush,                   2,      0,      0,
            Straight,                2,      0,      0,
            ThreeKind,               1,      0,      0,
        };
        static const double estimatedReturn = 99.78;
        REGISTER;
    }

    namespace ps_99_62
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlushNoDeuces,      250,    4700,   0,
            FourDeuces,              400,    0,      0,
            RoyalFlushWithDeuce,     25,     0,      0,
            FiveOfAKind,             16,     0,      0,
            StraightFlush,           11,     0,      0,
            FourOfAKind,             4,      0,      0,
            FullHouse,               3,      0,      0,
            Flush,                   2,      0,      0,
            Straight,                2,      0,      0,
            ThreeKind,               1,      0,      0,
        };
        static const double estimatedReturn = 99.62;
        REGISTER;
    }

    namespace ps_99_17
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlushNoDeuces,      250,    4700,   0,
            FourDeuces,              400,    0,      0,
            RoyalFlushWithDeuce,     25,     0,      0,
            FiveOfAKind,             16,     0,      0,
            StraightFlush,           10,     0,      0,
            FourOfAKind,             4,      0,      0,
            FullHouse,               3,      0,      0,
            Flush,                   2,      0,      0,
            Straight,                2,      0,      0,
            ThreeKind,               1,      0,      0,
        };
        static const double estimatedReturn = 99.17;
        REGISTER;
    }

    namespace ps_98_86
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlushNoDeuces,      250,    4000,   0,
            FourDeuces,              400,    0,      0,
            RoyalFlushWithDeuce,     25,     0,      0,
            FiveOfAKind,             15,     0,      0,
            StraightFlush,           10,     0,      0,
            FourOfAKind,             4,      0,      0,
            FullHouse,               3,      0,      0,
            Flush,                   2,      0,      0,
            Straight,                2,      0,      0,
            ThreeKind,               1,      0,      0,
        };
        static const double estimatedReturn = 98.86;
        REGISTER;
    }

} // DoubleDeuces
}

