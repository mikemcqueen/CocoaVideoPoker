/*
 *  WhiteHotAces.cpp
 *
 *  Created by Mike McQueen on 4/3/11.
 */

#include "stdafx.h"
#include "Poker.h"
#include "PayScheduleManager.hpp"
#include "Hand.hpp"

#include "BonusPokerBase.cpp"

namespace PaySchedule
{
namespace WhiteHotAces
{
    void registerPaySchedules()
    {
        PaySchedule::Manager_t::registerGame(PaySchedule::Game::WhiteHotAces, string("White Hot Aces"), string("Bonus.dat"));
    }

    #define REGISTER    REGISTER_PAY_SCHEDULE2(PaySchedule::Game::WhiteHotAces, BonusPoker::Base::payLines, payLines, estimatedReturn, 0)

    using namespace Hand::Value;

    namespace ps_99_80
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0,
            StraightFlush,              100,    0,      0,
            FourAces,                   240,    0,      0,
            Four2s3s4s,                 120,    0,      0,
            Four5sThruKings,            50,     0,      0,
            FullHouse,                  9,      0,      0,
            Flush,                      5,      0,      0,
            Straight,                   4,      0,      0,
            ThreeKind,                  3,      0,      0,
            TwoPair,                    1,      0,      0,
            Hand::Value::JacksOrBetter, 1,      0,      0, 
        };
        static const double estimatedReturn = 99.80;
        REGISTER;
    }

    namespace ps_99_57
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0,
            StraightFlush,              80,     0,      0,
            FourAces,                   240,    0,      0,
            Four2s3s4s,                 120,    0,      0,
            Four5sThruKings,            50,     0,      0,
            FullHouse,                  9,      0,      0,
            Flush,                      5,      0,      0,
            Straight,                   4,      0,      0,
            ThreeKind,                  3,      0,      0,
            TwoPair,                    1,      0,      0,
            Hand::Value::JacksOrBetter, 1,      0,      0, 
        };
        static const double estimatedReturn = 99.57;
        REGISTER;
    }

    namespace ps_99_24
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0,
            StraightFlush,              50,     0,      0,
            FourAces,                   240,    0,      0,
            Four2s3s4s,                 120,    0,      0,
            Four5sThruKings,            50,     0,      0,
            FullHouse,                  9,      0,      0,
            Flush,                      5,      0,      0,
            Straight,                   4,      0,      0,
            ThreeKind,                  3,      0,      0,
            TwoPair,                    1,      0,      0,
            Hand::Value::JacksOrBetter, 1,      0,      0, 
        };
        static const double estimatedReturn = 99.24;
        REGISTER;
    }

    namespace ps_98_50
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0,
            StraightFlush,              80,     0,      0,
            FourAces,                   240,    0,      0,
            Four2s3s4s,                 120,    0,      0,
            Four5sThruKings,            50,     0,      0,
            FullHouse,                  8,      0,      0,
            Flush,                      5,      0,      0,
            Straight,                   4,      0,      0,
            ThreeKind,                  3,      0,      0,
            TwoPair,                    1,      0,      0,
            Hand::Value::JacksOrBetter, 1,      0,      0, 
        };
        static const double estimatedReturn = 99.50;
        REGISTER;
    }

    namespace ps_98_16
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0,
            StraightFlush,              50,     0,      0,
            FourAces,                   240,    0,      0,
            Four2s3s4s,                 120,    0,      0,
            Four5sThruKings,            50,     0,      0,
            FullHouse,                  8,      0,      0,
            Flush,                      5,      0,      0,
            Straight,                   4,      0,      0,
            ThreeKind,                  3,      0,      0,
            TwoPair,                    1,      0,      0,
            Hand::Value::JacksOrBetter, 1,      0,      0, 
        };
        static const double estimatedReturn = 98.16;
        REGISTER;
    }

    namespace ps_97_44
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0,
            StraightFlush,              80,     0,      0,
            FourAces,                   240,    0,      0,
            Four2s3s4s,                 120,    0,      0,
            Four5sThruKings,            50,     0,      0,
            FullHouse,                  7,      0,      0,
            Flush,                      5,      0,      0,
            Straight,                   4,      0,      0,
            ThreeKind,                  3,      0,      0,
            TwoPair,                    1,      0,      0,
            Hand::Value::JacksOrBetter, 1,      0,      0, 
        };
        static const double estimatedReturn = 97.44;
        REGISTER;
    }

    namespace ps_97_11
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0,
            StraightFlush,              50,     0,      0,
            FourAces,                   240,    0,      0,
            Four2s3s4s,                 120,    0,      0,
            Four5sThruKings,            50,     0,      0,
            FullHouse,                  7,      0,      0,
            Flush,                      5,      0,      0,
            Straight,                   4,      0,      0,
            ThreeKind,                  3,      0,      0,
            TwoPair,                    1,      0,      0,
            Hand::Value::JacksOrBetter, 1,      0,      0, 
        };
        static const double estimatedReturn = 97.11;
        REGISTER;
    }

} // WhiteHotAces
}