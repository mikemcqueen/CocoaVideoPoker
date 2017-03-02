/*
 *  JacksOrBetter.cpp
 *  CardLib
 *
 *  Created by Mike McQueen on 8/27/10.
 *  Copyright 2010 Mike McQueen. All rights reserved.
 *
 */

#include "stdafx.h"
#include "Poker.h"
#include "PayScheduleManager.hpp"

#include "JacksOrBetterBase.cpp"

////////////////////////////////////////////////////////////////////////////////

namespace PaySchedule
{
namespace JacksOrBetter
{
    void registerPaySchedules()
    {
        // TODO: move game name and game filename into array in PayScheduleManager.cpp or private file included by it
        PaySchedule::Manager_t::registerGame(Game::JacksOrBetter, string("Jacks or Better"), string("Jacks.dat"));
    }

    using namespace Hand::Value;
        
    #define REGISTER    REGISTER_PAY_SCHEDULE2(PaySchedule::Game::JacksOrBetter, JacksOrBetter::Base::payLines, payLines, estimatedReturn, 0)

    namespace ps_9_6_976
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,   4880,   0,
            StraightFlush,              50,       0,   0,
            FourOfAKind,                25,       0,   0,
            FullHouse,                  9,        0,   0,
            Flush,                      6,        0,   0,
            Straight,                   4,        0,   0,
            ThreeKind,                  3,        0,   0,
            TwoPair,                    2,        0,   0,
            Hand::Value::JacksOrBetter, 1,        0,   0,
        };
        static const double estimatedReturn = 100.00;
        REGISTER;
    }
    namespace ps_9_6_4000
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0, 
            StraightFlush,              50,     0,      0, 
            FourOfAKind,                25,     0,      0, 
            FullHouse,                  9,      0,      0, 
            Flush,                      6,      0,      0, 
            Straight,                   4,      0,      0, 
            ThreeKind,                  3,      0,      0, 
            TwoPair,                    2,      0,      0, 
            Hand::Value::JacksOrBetter, 1,      0,      0,
        };
        static const double estimatedReturn = 99.54;
        REGISTER;
    }

    namespace ps_9_5_4000
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0, 
            StraightFlush,              50,     0,      0, 
            FourOfAKind,                25,     0,      0, 
            FullHouse,                  9,      0,      0, 
            Flush,                      5,      0,      0, 
            Straight,                   4,      0,      0, 
            ThreeKind,                  3,      0,      0, 
            TwoPair,                    2,      0,      0, 
            Hand::Value::JacksOrBetter, 1,      0,      0,
        };
        static const double estimatedReturn = 98.44;
        REGISTER;
    }

    namespace ps_8_6_4000
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0, 
            StraightFlush,              50,     0,      0, 
            FourOfAKind,                25,     0,      0, 
            FullHouse,                  8,      0,      0, 
            Flush,                      6,      0,      0, 
            Straight,                   4,      0,      0, 
            ThreeKind,                  3,      0,      0, 
            TwoPair,                    2,      0,      0, 
            Hand::Value::JacksOrBetter, 1,      0,      0,
        };
        static const double estimatedReturn = 98.39;
        REGISTER;
    }

    namespace ps_8_5_4000
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0, 
            StraightFlush,              50,     0,      0, 
            FourOfAKind,                25,     0,      0, 
            FullHouse,                  8,      0,      0, 
            Flush,                      5,      0,      0, 
            Straight,                   4,      0,      0, 
            ThreeKind,                  3,      0,      0, 
            TwoPair,                    2,      0,      0, 
            Hand::Value::JacksOrBetter, 1,      0,      0,
        };
        static const double estimatedReturn = 97.29;
        REGISTER;
    }

    namespace ps_7_6_4000
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0, 
            StraightFlush,              50,     0,      0, 
            FourOfAKind,                25,     0,      0, 
            FullHouse,                  7,      0,      0, 
            Flush,                      6,      0,      0, 
            Straight,                   4,      0,      0, 
            ThreeKind,                  3,      0,      0, 
            TwoPair,                    2,      0,      0, 
            Hand::Value::JacksOrBetter, 1,      0,      0,
        };
        static const double estimatedReturn = 97.24;
        REGISTER;
    }

    namespace ps_7_5_4000
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0, 
            StraightFlush,              50,     0,      0, 
            FourOfAKind,                25,     0,      0, 
            FullHouse,                  7,      0,      0, 
            Flush,                      5,      0,      0, 
            Straight,                   4,      0,      0, 
            ThreeKind,                  3,      0,      0, 
            TwoPair,                    2,      0,      0, 
            Hand::Value::JacksOrBetter, 1,      0,      0,
        };
        static const double estimatedReturn = 96.14;
        REGISTER;
    }

    namespace ps_6_5_4000
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0, 
            StraightFlush,              50,     0,      0, 
            FourOfAKind,                25,     0,      0, 
            FullHouse,                  6,      0,      0, 
            Flush,                      5,      0,      0, 
            Straight,                   4,      0,      0, 
            ThreeKind,                  3,      0,      0, 
            TwoPair,                    2,      0,      0, 
            Hand::Value::JacksOrBetter, 1,      0,      0,
        };
        static const double estimatedReturn = 94.99;
        REGISTER;
    }
}
}