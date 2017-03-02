/*
 *  DoubleDblBonus.cpp
 *  CardLib
 *
 *  Created by Mike McQueen on 8/31/10.
 *  Copyright 2010 Mike McQueen. All rights reserved.
 *
 */

#include "stdafx.h"
#include "Poker.h"
#include "PayScheduleManager.hpp"
//#include "DoubleDblBonus.h"
#include "Hand.hpp"

namespace PaySchedule
{
namespace DoubleDblBonus
{
    void registerPaySchedules()
    {
        PaySchedule::Manager_t::registerGame(PaySchedule::Game::DoubleDblBonus, string("Double Double Bonus"), string("DDBonus.dat"));
    }

    using namespace Hand::Value;
    using namespace PaySchedule::Static::Flag;
        
    namespace Base
    {
        const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 0,      0,      0,
            StraightFlush,              0,      0,      0,

        FourAces_With_234,              0,      0,      Group,
            FourAces_With_Two,          0,      0,      0,
            FourAces_With_Three,        0,      0,      0,
            FourAces_With_Four,         0,      0,      Group,
            
        Four2s3s4s_With_Ace234,         0,      0,      Group,
            FourTwos_With_Ace,          0,      0,      0,
            FourTwos_With_Three,        0,      0,      0,
            FourTwos_With_Four,         0,      0,      0,
            FourThrees_With_Ace,        0,      0,      0,
            FourThrees_With_Two,        0,      0,      0,
            FourThrees_With_Four,       0,      0,      0,
            FourFours_With_Ace,         0,      0,      0,
            FourFours_With_Two,         0,      0,      0,
            FourFours_With_Three,       0,      0,      Group,
            
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

    #define REGISTER    REGISTER_PAY_SCHEDULE2(PaySchedule::Game::DoubleDblBonus, DoubleDblBonus::Base::payLines, payLines, estimatedReturn, 0)

    namespace ps_10_6_4000
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0,
            StraightFlush,              50,     0,      0,
            FourAces_With_234,          400,    0,      0,
            Four2s3s4s_With_Ace234,     160,    0,      0,
            FourAces,                   160,    0,      0,
            Four2s3s4s,                 80,     0,      0,
            Four5sThruKings,            50,     0,      0,
            FullHouse,                  10,     0,      0,
            Flush,                      6,      0,      0,
            Straight,                   4,      0,      0,
            ThreeKind,                  3,      0,      0,
            TwoPair,                    1,      0,      0,
            Hand::Value::JacksOrBetter, 1,      0,      0, 
        };
        static const double estimatedReturn = 100.07;
        REGISTER;
    }

    namespace ps_99_96
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0,
            StraightFlush,              40,     0,      0,
            FourAces_With_234,          400,    0,      0,
            Four2s3s4s_With_Ace234,     160,    0,      0,
            FourAces,                   160,    0,      0,
            Four2s3s4s,                 80,     0,      0,
            Four5sThruKings,            50,     0,      0,
            FullHouse,                  10,     0,      0,
            Flush,                      6,      0,      0,
            Straight,                   4,      0,      0,
            ThreeKind,                  3,      0,      0,
            TwoPair,                    1,      0,      0,
            Hand::Value::JacksOrBetter, 1,      0,      0, 
        };
        static const double estimatedReturn = 99.96;
        REGISTER;
    }

    namespace ps_98_98
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0,
            StraightFlush,              50,     0,      0,
            FourAces_With_234,          400,    0,      0,
            Four2s3s4s_With_Ace234,     160,    0,      0,
            FourAces,                   160,    0,      0,
            Four2s3s4s,                 80,     0,      0,
            Four5sThruKings,            45,     0,      0,
            FullHouse,                  10,     0,      0,
            Flush,                      6,      0,      0,
            Straight,                   4,      0,      0,
            ThreeKind,                  3,      0,      0,
            TwoPair,                    1,      0,      0,
            Hand::Value::JacksOrBetter, 1,      0,      0, 
        };
        static const double estimatedReturn = 98.98;
        REGISTER;
    }

    namespace ps_97_89
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0,
            StraightFlush,              50,     0,      0,
            FourAces_With_234,          400,    0,      0,
            Four2s3s4s_With_Ace234,     160,    0,      0,
            FourAces,                   160,    0,      0,
            Four2s3s4s,                 80,     0,      0,
            Four5sThruKings,            50,     0,      0,
            FullHouse,                  8,     0,      0,
            Flush,                      6,      0,      0,
            Straight,                   4,      0,      0,
            ThreeKind,                  3,      0,      0,
            TwoPair,                    1,      0,      0,
            Hand::Value::JacksOrBetter, 1,      0,      0, 
        };
        static const double estimatedReturn = 97.89;
        REGISTER;
    }

    namespace ps_97_87
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0,
            StraightFlush,              50,     0,      0,
            FourAces_With_234,          400,    0,      0,
            Four2s3s4s_With_Ace234,     160,    0,      0,
            FourAces,                   160,    0,      0,
            Four2s3s4s,                 80,     0,      0,
            Four5sThruKings,            50,     0,      0,
            FullHouse,                  9,     0,      0,
            Flush,                      5,      0,      0,
            Straight,                   4,      0,      0,
            ThreeKind,                  3,      0,      0,
            TwoPair,                    1,      0,      0,
            Hand::Value::JacksOrBetter, 1,      0,      0, 
        };
        static const double estimatedReturn = 97.87;
        REGISTER;
    }

}
}