/*
 *  SuperAces.cpp
 *  CardLib
 *
 *  Created by Mike McQueen on 4/2/11.
 *  Copyright 2011 Mike McQueen. All rights reserved.
 *
 */

#include "stdafx.h"
#include "Poker.h"
#include "PayScheduleManager.hpp"
#include "Hand.hpp"

#include "BonusPokerBase.cpp"

namespace PaySchedule
{
namespace SuperAces
{
    void registerPaySchedules()
    {
        PaySchedule::Manager_t::registerGame(PaySchedule::Game::SuperAces, string("Super Aces"), string("Bonus.dat"));
    }

    #define REGISTER    REGISTER_PAY_SCHEDULE2(PaySchedule::Game::SuperAces, BonusPoker::Base::payLines, payLines, estimatedReturn, 0)

    using namespace Hand::Value;

    namespace ps_99_94
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0,
            StraightFlush,              60,     0,      0,
            FourAces,                   400,    0,      0,
            Four2s3s4s,                 80,     0,      0,
            Four5sThruKings,            50,     0,      0,
            FullHouse,                  8,      0,      0,
            Flush,                      5,      0,      0,
            Straight,                   4,      0,      0,
            ThreeKind,                  3,      0,      0,
            TwoPair,                    1,      0,      0,
            Hand::Value::JacksOrBetter, 1,      0,      0, 
        };
        static const double estimatedReturn = 99.94;
        REGISTER;
    }

    namespace ps_99_84
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0,
            StraightFlush,              50,     0,      0,
            FourAces,                   400,    0,      0,
            Four2s3s4s,                 80,     0,      0,
            Four5sThruKings,            50,     0,      0,
            FullHouse,                  8,      0,      0,
            Flush,                      5,      0,      0,
            Straight,                   4,      0,      0,
            ThreeKind,                  3,      0,      0,
            TwoPair,                    1,      0,      0,
            Hand::Value::JacksOrBetter, 1,      0,      0, 
        };
        static const double estimatedReturn = 99.84;
        REGISTER;
    }

    namespace ps_98_85
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0,
            StraightFlush,              60,     0,      0,
            FourAces,                   400,    0,      0,
            Four2s3s4s,                 80,     0,      0,
            Four5sThruKings,            50,     0,      0,
            FullHouse,                  7,      0,      0,
            Flush,                      5,      0,      0,
            Straight,                   4,      0,      0,
            ThreeKind,                  3,      0,      0,
            TwoPair,                    1,      0,      0,
            Hand::Value::JacksOrBetter, 1,      0,      0, 
        };
        static const double estimatedReturn = 98.85;
        REGISTER;
    }

    namespace ps_98_75
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0,
            StraightFlush,              50,     0,      0,
            FourAces,                   400,    0,      0,
            Four2s3s4s,                 80,     0,      0,
            Four5sThruKings,            50,     0,      0,
            FullHouse,                  7,      0,      0,
            Flush,                      5,      0,      0,
            Straight,                   4,      0,      0,
            ThreeKind,                  3,      0,      0,
            TwoPair,                    1,      0,      0,
            Hand::Value::JacksOrBetter, 1,      0,      0, 
        };
        static const double estimatedReturn = 98.75;
        REGISTER;
    }

    namespace ps_98_30
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0,
            StraightFlush,              40,     0,      0,
            FourAces,                   400,    0,      0,
            Four2s3s4s,                 80,     0,      0,
            Four5sThruKings,            40,     0,      0,
            FullHouse,                  8,      0,      0,
            Flush,                      5,      0,      0,
            Straight,                   4,      0,      0,
            ThreeKind,                  3,      0,      0,
            TwoPair,                    1,      0,      0,
            Hand::Value::JacksOrBetter, 1,      0,      0, 
        };
        static const double estimatedReturn = 98.30;
        REGISTER;
    }

} // SuperAces
} // PaySchedule

