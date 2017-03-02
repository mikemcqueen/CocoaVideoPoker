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

namespace PaySchedule
{
namespace DeucesWild
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

    namespace ps_25_15_11 // 99.96%
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlushNoDeuces,      250,    4000,   0,
            FourDeuces,              200,    0,      0,
            RoyalFlushWithDeuce,     25,     0,      0,
            FiveOfAKind,             15,     0,      0,
            StraightFlush,           11,     0,      0,
            FourOfAKind,             4,      0,      0,
            FullHouse,               4,      0,      0,
            Flush,                   3,      0,      0,
            Straight,                2,      0,      0,
            ThreeKind,               1,      0,      0,
        };
        static const double estimatedReturn = 99.96;
        REGISTER;
    }

    namespace ps_25_16_10 // Not So Ugly Ducks 99.73%
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlushNoDeuces,      250,    4000,   0,
            FourDeuces,              200,    0,      0,
            RoyalFlushWithDeuce,     25,     0,      0,
            FiveOfAKind,             16,     0,      0,
            StraightFlush,           10,     0,      0,
            FourOfAKind,             4,      0,      0,
            FullHouse,               4,      0,      0,
            Flush,                   3,      0,      0,
            Straight,                2,      0,      0,
            ThreeKind,               1,      0,      0,
        };
        static const double estimatedReturn = 99.73;
        REGISTER;
    }

    namespace ps_25_15_10_44 // 99.42%
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlushNoDeuces,      250,    4000,   0,
            FourDeuces,              200,    0,      0,
            RoyalFlushWithDeuce,     25,     0,      0,
            FiveOfAKind,             15,     0,      0,
            StraightFlush,           10,     0,      0,
            FourOfAKind,             4,      0,      0,
            FullHouse,               4,      0,      0,
            Flush,                   3,      0,      0,
            Straight,                2,      0,      0,
            ThreeKind,               1,      0,      0,
        };
        static const double estimatedReturn = 99.42;
        REGISTER;
    }
    namespace ps_25_16_9 // 99.23%
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlushNoDeuces,      250,    4000,   0,
            FourDeuces,              200,    0,      0,
            RoyalFlushWithDeuce,     25,     0,      0,
            FiveOfAKind,             16,     0,      0,
            StraightFlush,           9,      0,      0,
            FourOfAKind,             4,      0,      0,
            FullHouse,               4,      0,      0,
            Flush,                   3,      0,      0,
            Straight,                2,      0,      0,
            ThreeKind,               1,      0,      0,
        };
        static const double estimatedReturn = 99.23;
        REGISTER;
    }
    namespace ps_20_12_9_5 // 98.94%
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlushNoDeuces,      250,    4000,   0,
            FourDeuces,              200,    0,      0,
            RoyalFlushWithDeuce,     20,     0,      0,
            FiveOfAKind,             12,     0,      0,
            StraightFlush,           9,      0,      0,
            FourOfAKind,             5,      0,      0,
            FullHouse,               3,      0,      0,
            Flush,                   2,      0,      0,
            Straight,                2,      0,      0,
            ThreeKind,               1,      0,      0,
        };
        static const double estimatedReturn = 98.94;
        REGISTER;
    }

    namespace ps_25_15_9_4 // Ugly Ducks 98.91%
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlushNoDeuces,      250,    4000,   0,
            FourDeuces,              200,    0,      0,
            RoyalFlushWithDeuce,     25,     0,      0,
            FiveOfAKind,             15,     0,      0,
            StraightFlush,           9,      0,      0,
            FourOfAKind,             4,      0,      0,
            FullHouse,               4,      0,      0,
            Flush,                   3,      0,      0,
            Straight,                2,      0,      0,
            ThreeKind,               1,      0,      0,
        };
        static const double estimatedReturn = 98.91;
        REGISTER;
    }

    namespace ps_20_15_9 // 97.97%
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlushNoDeuces,      250,    4000,   0,
            FourDeuces,              200,    0,      0,
            RoyalFlushWithDeuce,     20,     0,      0,
            FiveOfAKind,             15,     0,      0,
            StraightFlush,           9,      0,      0,
            FourOfAKind,             4,      0,      0,
            FullHouse,               4,      0,      0,
            Flush,                   3,      0,      0,
            Straight,                2,      0,      0,
            ThreeKind,               1,      0,      0,
        };
        static const double estimatedReturn = 97.97;
        REGISTER;
    }


    namespace ps_20_12_10 // 97.58%
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlushNoDeuces,      250,    4000,   0,
            FourDeuces,              200,    0,      0,
            RoyalFlushWithDeuce,     20,     0,      0,
            FiveOfAKind,             12,     0,      0,
            StraightFlush,           10,     0,      0,
            FourOfAKind,             4,      0,      0,
            FullHouse,               4,      0,      0,
            Flush,                   3,      0,      0,
            Straight,                2,      0,      0,
            ThreeKind,               1,      0,      0,
        };
        static const double estimatedReturn = 97.58;
        REGISTER;
    }

    namespace ps_20_12_9_4 // 97.06%
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlushNoDeuces,      250,    4000,   0,
            FourDeuces,              200,    0,      0,
            RoyalFlushWithDeuce,     20,     0,      0,
            FiveOfAKind,             12,     0,      0,
            StraightFlush,           9,      0,      0,
            FourOfAKind,             4,      0,      0,
            FullHouse,               4,      0,      0,
            Flush,                   3,      0,      0,
            Straight,                2,      0,      0,
            ThreeKind,               1,      0,      0,
        };
        static const double estimatedReturn = 97.06;
        REGISTER;
    }

#if 0
    namespace ps_25_16_13 // 96.77%
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlushNoDeuces,      250,    4000,   0,
            FourDeuces,              200,    0,      0,
            RoyalFlushWithDeuce,     25,     0,      0,
            FiveOfAKind,             16,     0,      0,
            StraightFlush,           13,     0,      0,
            FourOfAKind,             4,      0,      0,
            FullHouse,               3,      0,      0,
            Flush,                   2,      0,      0,
            Straight,                2,      0,      0,
            ThreeKind,               1,      0,      0,
        };
        static const double estimatedReturn = 96.77;
        REGISTER;
    }

    namespace ps_25_12_9 // 96.01%
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlushNoDeuces,      250,    4000,   0,
            FourDeuces,              200,    0,      0,
            RoyalFlushWithDeuce,     25,     0,      0,
            FiveOfAKind,             12,     0,      0,
            StraightFlush,           9,      0,      0,
            FourOfAKind,             4,      0,      0,
            FullHouse,               4,      0,      0,
            Flush,                   2,      0,      0,
            Straight,                2,      0,      0,
            ThreeKind,               1,      0,      0,
        };
        static const double estimatedReturn = 96.01;
        REGISTER;
   }

    namespace ps_20_10_8 // 95.96%
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlushNoDeuces,      250,    4000,   0,
            FourDeuces,              200,    0,      0,
            RoyalFlushWithDeuce,     20,     0,      0,
            FiveOfAKind,             10,     0,      0,
            StraightFlush,           8,      0,      0,
            FourOfAKind,             4,      0,      0,
            FullHouse,               4,      0,      0,
            Flush,                   3,      0,      0,
            Straight,                2,      0,      0,
            ThreeKind,               1,      0,      0,
        };
        static const double estimatedReturn = 95.96;
        REGISTER;
    }

    namespace ps_25_12_8 // 95.59%
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlushNoDeuces,      250,    4000,   0,
            FourDeuces,              200,    0,      0,
            RoyalFlushWithDeuce,     25,     0,      0, 
            FiveOfAKind,             12,     0,      0,
            StraightFlush,           8,      0,      0,
            FourOfAKind,             4,      0,      0,
            FullHouse,               4,      0,      0,
            Flush,                   2,      0,      0,
            Straight,                2,      0,      0,
            ThreeKind,               1,      0,      0,
        };
        static const double estimatedReturn = 95.59;
        REGISTER;
    }

    namespace ps_25_12_7 // 95.19%
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlushNoDeuces,      250,    4000,   0,
            FourDeuces,              200,    0,      0,
            RoyalFlushWithDeuce,     25,     0,      0,
            FiveOfAKind,             12,     0,      0,
            StraightFlush,           7,      0,      0,
            FourOfAKind,             4,      0,      0,
            FullHouse,               4,      0,      0,
            Flush,                   2,      0,      0,
            Straight,                2,      0,      0,
            ThreeKind,               1,      0,      0,
        };
        static const double estimatedReturn = 95.19;
        REGISTER;
    }

    namespace ps_25_10_8 // 94.97%
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlushNoDeuces,      250,    4000,   0,
            FourDeuces,              200,    0,      0,
            RoyalFlushWithDeuce,     25,     0,      0,
            FiveOfAKind,             16,     0,      0,
            StraightFlush,           10,     0,      0,
            FourOfAKind,             4,      0,      0,
            FullHouse,               4,      0,      0,
            Flush,                   2,      0,      0,
            Straight,                2,      0,      0,
            ThreeKind,               1,      0,      0,
        };
        static const double estimatedReturn = 94.97;
        REGISTER;
    }

    namespace ps_25_15_10_33 // 94.82%
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlushNoDeuces,      250,    4000,   0,
            FourDeuces,              200,    0,      0,
            RoyalFlushWithDeuce,     25,     0,      0,
            FiveOfAKind,             15,     0,      0,
            StraightFlush,           10,     0,      0,
            FourOfAKind,             4,      0,      0,
            FullHouse,               3,      0,      0,
            Flush,                   3,      0,      0,
            Straight,                2,      0,      0,
            ThreeKind,               1,      0,      0,
        };
        static const double estimatedReturn = 94.82;
        REGISTER;
    }

    namespace ps_25_10_7 // 94.58%
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlushNoDeuces,      250,    4000,   0,
            FourDeuces,              200,    0,      0,
            RoyalFlushWithDeuce,     25,     0,      0,
            FiveOfAKind,             10,     0,      0,
            StraightFlush,           7,      0,      0,
            FourOfAKind,             4,      0,      0,
            FullHouse,               4,      0,      0,
            Flush,                   2,      0,      0,
            Straight,                2,      0,      0,
            ThreeKind,               1,      0,      0,
        };
        static const double estimatedReturn = 94.58;
        REGISTER;
    }

    namespace ps_20_10_8_33 // 93.37%
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlushNoDeuces,      250,    4000,   0,
            FourDeuces,              200,    0,      0,
            RoyalFlushWithDeuce,     20,     0,      0,
            FiveOfAKind,             10,     0,      0,
            StraightFlush,           8,      0,      0,
            FourOfAKind,             4,      0,      0,
            FullHouse,               3,      0,      0,
            Flush,                   3,      0,      0,
            Straight,                2,      0,      0,
            ThreeKind,               1,      0,      0,
        };
        static const double estimatedReturn = 93.37;
        REGISTER;
    }

    namespace ps_20_10_8_22 // 91.42%
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlushNoDeuces,      250,    4000,   0,
            FourDeuces,              200,    0,      0,
            RoyalFlushWithDeuce,     20,     0,      0,
            FiveOfAKind,             10,     0,      0,
            StraightFlush,           8,      0,      0,
            FourOfAKind,             4,      0,      0,
            FullHouse,               3,      0,      0,
            Flush,                   2,      0,      0,
            Straight,                2,      0,      0,
            ThreeKind,               1,      0,      0,
        };
        static const double estimatedReturn = 91.42;
        REGISTER;
    }
#endif

}
}
