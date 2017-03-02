/*
 *  BonusPokerDeluxe.cpp
 *  CardLib
 *
 *  Created by Mike McQueen on 4/1/11.
 *  Copyright 2011 Mike McQueen. All rights reserved.
 *
 */

#include "stdafx.h"
#include "Poker.h"
#include "PayScheduleManager.hpp"
#include "Hand.hpp"

#include "JacksOrBetterBase.cpp"

namespace PaySchedule
{
namespace BonusPokerDeluxe
{
    void registerPaySchedules()
    {
        PaySchedule::Manager_t::registerGame(PaySchedule::Game::BonusPokerDeluxe, string("Bonus Poker Deluxe"), string("Jacks.dat"));
    }

    #define REGISTER    REGISTER_PAY_SCHEDULE2(PaySchedule::Game::BonusPokerDeluxe, JacksOrBetter::Base::payLines, payLines, estimatedReturn, 0)

    using namespace Hand::Value;

    namespace ps_99_64 // Full Pay
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0,
            StraightFlush,              50,     0,      0,
            FourOfAKind,                80,     0,      0,
            FullHouse,                  9,      0,      0,
            Flush,                      6,      0,      0,
            Straight,                   4,      0,      0,
            ThreeKind,                  3,      0,      0,
            TwoPair,                    1,      0,      0,
            Hand::Value::JacksOrBetter, 1,      0,      0, 
        };
        static const double estimatedReturn = 99.64;
        REGISTER;
    }

    namespace ps_98_49
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0,
            StraightFlush,              50,     0,      0,
            FourOfAKind,                80,     0,      0,
            FullHouse,                  8,      0,      0,
            Flush,                      6,      0,      0,
            Straight,                   4,      0,      0,
            ThreeKind,                  3,      0,      0,
            TwoPair,                    1,      0,      0,
            Hand::Value::JacksOrBetter, 1,      0,      0, 
        };
        static const double estimatedReturn = 98.49;
        REGISTER;
    }

    namespace ps_97_40
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0,
            StraightFlush,              50,     0,      0,
            FourOfAKind,                80,     0,      0,
            FullHouse,                  8,      0,      0,
            Flush,                      5,      0,      0,
            Straight,                   4,      0,      0,
            ThreeKind,                  3,      0,      0,
            TwoPair,                    1,      0,      0,
            Hand::Value::JacksOrBetter, 1,      0,      0, 
        };
        static const double estimatedReturn = 97.40;
        REGISTER;
    }

    namespace ps_96_25
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,    4000,   0,
            StraightFlush,              50,     0,      0,
            FourOfAKind,                80,     0,      0,
            FullHouse,                  7,      0,      0,
            Flush,                      5,      0,      0,
            Straight,                   4,      0,      0,
            ThreeKind,                  3,      0,      0,
            TwoPair,                    1,      0,      0,
            Hand::Value::JacksOrBetter, 1,      0,      0, 
        };
        static const double estimatedReturn = 96.25;
        REGISTER;
    }

} // BonusPokerDeluxe
}
