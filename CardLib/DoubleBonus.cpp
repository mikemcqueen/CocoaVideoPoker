/*
 *  DoubleBonus.cpp
 *
 *  Copyright 2011 Mike McQueen. All rights reserved.
 */

#include "stdafx.h"
#include "Poker.h"
#include "PayScheduleManager.hpp"
#include "Hand.hpp"

#include "BonusPokerBase.cpp"

namespace PaySchedule
{
namespace DoubleBonus
{
    void registerPaySchedules()
    {
        PaySchedule::Manager_t::registerGame(PaySchedule::Game::DoubleBonus, string("Double Bonus Poker"), string("Bonus.dat"));
    }

    #define REGISTER    REGISTER_PAY_SCHEDULE2(PaySchedule::Game::DoubleBonus, BonusPoker::Base::payLines, payLines, estimatedReturn, 0)

    using namespace Hand::Value;

    namespace ps_100_54
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4796,   0,
            StraightFlush,              50,     0,      0,
            FourAces,                   160,    0,      0,
            Four2s3s4s,                 80,     0,      0,
            Four5sThruKings,            50,     0,      0,
            FullHouse,                  10,     0,      0,
            Flush,                      7,      0,      0,
            Straight,                   5,      0,      0,
            ThreeKind,                  3,      0,      0,
            TwoPair,                    1,      0,      0,
            Hand::Value::JacksOrBetter, 1,      0,      0, 
        };
        static const double estimatedReturn = 100.54;
        REGISTER;
    }

    namespace ps_100_52
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0,
            StraightFlush,              80,     0,      0,
            FourAces,                   160,    0,      0,
            Four2s3s4s,                 80,     0,      0,
            Four5sThruKings,            50,     0,      0,
            FullHouse,                  10,     0,      0,
            Flush,                      7,      0,      0,
            Straight,                   5,      0,      0,
            ThreeKind,                  3,      0,      0,
            TwoPair,                    1,      0,      0,
            Hand::Value::JacksOrBetter, 1,      0,      0, 
        };
        static const double estimatedReturn = 100.52;
        REGISTER;
    }

    namespace ps_100_17
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0,
            StraightFlush,              50,     0,      0,
            FourAces,                   160,    0,      0,
            Four2s3s4s,                 80,     0,      0,
            Four5sThruKings,            50,     0,      0,
            FullHouse,                  10,     0,      0,
            Flush,                      7,      0,      0,
            Straight,                   5,      0,      0,
            ThreeKind,                  3,      0,      0,
            TwoPair,                    1,      0,      0,
            Hand::Value::JacksOrBetter, 1,      0,      0, 
        };
        static const double estimatedReturn = 100.17;
        REGISTER;
    }

    namespace ps_99_76
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0,
            StraightFlush,              50,     0,      0,
            FourAces,                   160,    0,      0,
            Four2s3s4s,                 80,     0,      0,
            Four5sThruKings,            50,     0,      0,
            FullHouse,                  12,     0,      0,
            Flush,                      6,      0,      0,
            Straight,                   5,      0,      0,
            ThreeKind,                  3,      0,      0,
            TwoPair,                    1,      0,      0,
            Hand::Value::JacksOrBetter, 1,      0,      0, 
        };
        static const double estimatedReturn = 99.76;
        REGISTER;
    }

} // namespace BonusPoker
} // namespace PaySchedule


