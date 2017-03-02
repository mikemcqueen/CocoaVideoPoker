/*
 *  BonusPoker.cpp
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
namespace BonusPoker
{
    void registerPaySchedules()
    {
        PaySchedule::Manager_t::registerGame(PaySchedule::Game::BonusPoker, string("BonusPoker"), string("Bonus.dat"));
    }

    #define REGISTER    REGISTER_PAY_SCHEDULE2(PaySchedule::Game::BonusPoker, BonusPoker::Base::payLines, payLines, estimatedReturn, 0)

    using namespace Hand::Value;

    namespace ps_99_66
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0,
            StraightFlush,              50,     0,      0,
            FourAces,                   35,     0,      0,
            Four2s3s4s,                 35,     0,      0,
            Four5sThruKings,            35,     0,      0,
            FullHouse,                  8,      0,      0,
            Flush,                      5,      0,      0,
            Straight,                   4,      0,      0,
            ThreeKind,                  3,      0,      0,
            TwoPair,                    2,      0,      0,
            Hand::Value::JacksOrBetter, 1,      0,      0, 
        };
        static const double estimatedReturn = 99.66;
        REGISTER;
    }

    namespace ps_99_50
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0,
            StraightFlush,              50,     0,      0,
            FourAces,                   80,     0,      0,
            Four2s3s4s,                 40,     0,      0,
            Four5sThruKings,            20,     0,      0,
            FullHouse,                  9,      0,      0,
            Flush,                      5,      0,      0,
            Straight,                   4,      0,      0,
            ThreeKind,                  3,      0,      0,
            TwoPair,                    2,      0,      0,
            Hand::Value::JacksOrBetter, 1,      0,      0, 
        };
        static const double estimatedReturn = 99.50;
        REGISTER;
    }

    namespace ps_99_44
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0,
            StraightFlush,              50,     0,      0,
            FourAces,                   80,     0,      0,
            Four2s3s4s,                 40,     0,      0,
            Four5sThruKings,            20,     0,      0,
            FullHouse,                  8,      0,      0,
            Flush,                      6,      0,      0,
            Straight,                   4,      0,      0,
            ThreeKind,                  3,      0,      0,
            TwoPair,                    2,      0,      0,
            Hand::Value::JacksOrBetter, 1,      0,      0, 
        };
        static const double estimatedReturn = 99.44;
        REGISTER;
    }

    namespace ps_99_36
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0,
            StraightFlush,              100,    0,      0,
            FourAces,                   200,    0,      0,
            Four2s3s4s,                 40,     0,      0,
            Four5sThruKings,            25,     0,      0,
            FullHouse,                  12,     0,      0,
            Flush,                      8,      0,      0,
            Straight,                   5,      0,      0,
            ThreeKind,                  3,      0,      0,
            TwoPair,                    1,      0,      0,
            Hand::Value::JacksOrBetter, 1,      0,      0, 
        };
        static const double estimatedReturn = 99.36;
        REGISTER;
    }

    namespace ps_99_17
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0,
            StraightFlush,              50,     0,      0,
            FourAces,                   80,     0,      0,
            Four2s3s4s,                 40,     0,      0,
            Four5sThruKings,            25,     0,      0,
            FullHouse,                  8,      0,      0,
            Flush,                      5,      0,      0,
            Straight,                   4,      0,      0,
            ThreeKind,                  3,      0,      0,
            TwoPair,                    2,      0,      0,
            Hand::Value::JacksOrBetter, 1,      0,      0, 
        };
        static const double estimatedReturn = 99.17;
        REGISTER;
    }

    namespace ps_99_07
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0,
            StraightFlush,              50,     0,      0,
            FourAces,                   75,     0,      0,
            Four2s3s4s,                 40,     0,      0,
            Four5sThruKings,            25,     0,      0,
            FullHouse,                  8,      0,      0,
            Flush,                      5,      0,      0,
            Straight,                   4,      0,      0,
            ThreeKind,                  3,      0,      0,
            TwoPair,                    2,      0,      0,
            Hand::Value::JacksOrBetter, 1,      0,      0, 
        };
        static const double estimatedReturn = 99.07;
        REGISTER;
    }

    namespace ps_98_54
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0,
            StraightFlush,              50,     0,      0,
            FourAces,                   80,     0,      0,
            Four2s3s4s,                 50,     0,      0,
            Four5sThruKings,            25,     0,      0,
            FullHouse,                  7,      0,      0,
            Flush,                      5,      0,      0,
            Straight,                   4,      0,      0,
            ThreeKind,                  3,      0,      0,
            TwoPair,                    2,      0,      0,
            Hand::Value::JacksOrBetter, 1,      0,      0, 
        };
        static const double estimatedReturn = 98.54;
        REGISTER;
    }

    namespace ps_98_48
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0,
            StraightFlush,              50,     0,      0,
            FourAces,                   30,     0,      0,
            Four2s3s4s,                 30,     0,      0,
            Four5sThruKings,            30,     0,      0,
            FullHouse,                  8,      0,      0,
            Flush,                      5,      0,      0,
            Straight,                   4,      0,      0,
            ThreeKind,                  3,      0,      0,
            TwoPair,                    2,      0,      0,
            Hand::Value::JacksOrBetter, 1,      0,      0, 
        };
        static const double estimatedReturn = 98.48;
        REGISTER;
    }

    namespace ps_98_01
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0,
            StraightFlush,              50,     0,      0,
            FourAces,                   80,     0,      0,
            Four2s3s4s,                 40,     0,      0,
            Four5sThruKings,            25,     0,      0,
            FullHouse,                  7,      0,      0,
            Flush,                      5,      0,      0,
            Straight,                   4,      0,      0,
            ThreeKind,                  3,      0,      0,
            TwoPair,                    2,      0,      0,
            Hand::Value::JacksOrBetter, 1,      0,      0, 
        };
        static const double estimatedReturn = 98.01;
        REGISTER;
    }

    namespace ps_97_36
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0,
            StraightFlush,              50,     0,      0,
            FourAces,                   35,     0,      0,
            Four2s3s4s,                 35,     0,      0,
            Four5sThruKings,            35,     0,      0,
            FullHouse,                  6,      0,      0,
            Flush,                      5,      0,      0,
            Straight,                   4,      0,      0,
            ThreeKind,                  3,      0,      0,
            TwoPair,                    2,      0,      0,
            Hand::Value::JacksOrBetter, 1,      0,      0, 
        };
        static const double estimatedReturn = 97.36;
        REGISTER;
    }

#if 0
    namespace ps_96_87
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0,
            StraightFlush,              50,     0,      0,
            FourAces,                   80,     0,      0,
            Four2s3s4s,                 40,     0,      0,
            Four5sThruKings,            25,     0,      0,
            FullHouse,                  6,      0,      0,
            Flush,                      5,      0,      0,
            Straight,                   4,      0,      0,
            ThreeKind,                  3,      0,      0,
            TwoPair,                    2,      0,      0,
            Hand::Value::JacksOrBetter, 1,      0,      0, 
        };
        static const double estimatedReturn = 96.87;
        REGISTER;
    }
#endif

} // namespace BonusPoker
} // namespace PaySchedule

