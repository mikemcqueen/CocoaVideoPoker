/*
 *  LooseDeuces.cpp
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
namespace LooseDeuces
{
    void registerPaySchedules()
    {
        Manager_t::registerGame(Game::LooseDeuces, string("Loose Deuces"), string("Deuces.dat"));
    }

    using namespace Hand::Value;
    using namespace PaySchedule::Static::Flag;
    
    #define REGISTER    REGISTER_PAY_SCHEDULE2(PaySchedule::Game::LooseDeuces, DeucesWild::Base::payLines, payLines, estimatedReturn, 2)

    namespace ps_101_73
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlushNoDeuces,      250,    4000,   0,
            FourDeuces,              500,    0,      0,
            RoyalFlushWithDeuce,     25,     0,      0,
            FiveOfAKind,             16,     0,      0,
            StraightFlush,           11,      0,      0,
            FourOfAKind,             4,      0,      0,
            FullHouse,               3,      0,      0,
            Flush,                   2,      0,      0,
            Straight,                2,      0,      0,
            ThreeKind,               1,      0,      0,
        };
        static const double estimatedReturn = 101.73;
        REGISTER;
    }

    namespace ps_101_60
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlushNoDeuces,      250,    4000,   0,
            FourDeuces,              500,    0,      0,
            RoyalFlushWithDeuce,     25,     0,      0,
            FiveOfAKind,             17,     0,      0,
            StraightFlush,           10,     0,      0,
            FourOfAKind,             4,      0,      0,
            FullHouse,               3,      0,      0,
            Flush,                   2,      0,      0,
            Straight,                2,      0,      0,
            ThreeKind,               1,      0,      0,
        };
        static const double estimatedReturn = 101.60;
        REGISTER;
    }

    namespace ps_101_29
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlushNoDeuces,      250,    4000,   0,
            FourDeuces,              500,    0,      0,
            RoyalFlushWithDeuce,     25,     0,      0,
            FiveOfAKind,             16,     0,      0,
            StraightFlush,           10,     0,      0,
            FourOfAKind,             4,      0,      0,
            FullHouse,               3,      0,      0,
            Flush,                   2,      0,      0,
            Straight,                2,      0,      0,
            ThreeKind,               1,      0,      0,
        };
        static const double estimatedReturn = 101.29;
        REGISTER;
    }

    namespace ps_100_97
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlushNoDeuces,      250,    4000,   0,
            FourDeuces,              500,    0,      0,
            RoyalFlushWithDeuce,     25,     0,      0,
            FiveOfAKind,             15,     0,      0,
            StraightFlush,           10,     0,      0,
            FourOfAKind,             4,      0,      0,
            FullHouse,               3,      0,      0,
            Flush,                   2,      0,      0,
            Straight,                2,      0,      0,
            ThreeKind,               1,      0,      0,
        };
        static const double estimatedReturn = 100.97;
        REGISTER;
    }

    namespace ps_100_47
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlushNoDeuces,      250,    4000,   0,
            FourDeuces,              500,    0,      0,
            RoyalFlushWithDeuce,     25,     0,      0,
            FiveOfAKind,             12,     0,      0,
            StraightFlush,           11,     0,      0,
            FourOfAKind,             4,      0,      0,
            FullHouse,               3,      0,      0,
            Flush,                   2,      0,      0,
            Straight,                2,      0,      0,
            ThreeKind,               1,      0,      0,
        };
        static const double estimatedReturn = 100.47;
        REGISTER;
    }

    namespace ps_100_15
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlushNoDeuces,      250,    4000,   0,
            FourDeuces,              500,    0,      0,
            RoyalFlushWithDeuce,     25,     0,      0,
            FiveOfAKind,             15,     0,      0,
            StraightFlush,           18,      0,      0,
            FourOfAKind,             4,      0,      0,
            FullHouse,               3,      0,      0,
            Flush,                   2,      0,      0,
            Straight,                2,      0,      0,
            ThreeKind,               1,      0,      0,
        };
        static const double estimatedReturn = 100.15;
        REGISTER;
    }

    namespace ps_100_02
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlushNoDeuces,      250,    4000,   0,
            FourDeuces,              500,    0,      0,
            RoyalFlushWithDeuce,     25,     0,      0,
            FiveOfAKind,             12,     0,      0,
            StraightFlush,           10,     0,      0,
            FourOfAKind,             4,      0,      0,
            FullHouse,               3,      0,      0,
            Flush,                   2,      0,      0,
            Straight,                2,      0,      0,
            ThreeKind,               1,      0,      0,
        };
        static const double estimatedReturn = 100.02;
        REGISTER;
    }

    namespace ps_99_78
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlushNoDeuces,      250,    4000,   0,
            FourDeuces,              500,    0,      0,
            RoyalFlushWithDeuce,     25,     0,      0,
            FiveOfAKind,             15,     0,      0,
            StraightFlush,           7,      0,      0,
            FourOfAKind,             4,      0,      0,
            FullHouse,               3,      0,      0,
            Flush,                   2,      0,      0,
            Straight,                2,      0,      0,
            ThreeKind,               1,      0,      0,
        };
        static const double estimatedReturn = 99.78;
        REGISTER;
    }

    namespace ps_99_42
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlushNoDeuces,      250,    4000,   0,
            FourDeuces,              500,    0,      0,
            RoyalFlushWithDeuce,     25,     0,      0,
            FiveOfAKind,             15,     0,      0,
            StraightFlush,           6,      0,      0,
            FourOfAKind,             4,      0,      0,
            FullHouse,               3,      0,      0,
            Flush,                   2,      0,      0,
            Straight,                2,      0,      0,
            ThreeKind,               1,      0,      0,
        };
        static const double estimatedReturn = 99.42;
        REGISTER;
    }

}
}
