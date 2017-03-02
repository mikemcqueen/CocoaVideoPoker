/*
 *  AllAmerican
 *  CardLib
 *
 *  Copyright 2011 Mike McQueen. All rights reserved.
 */

#include "stdafx.h"
#include "Poker.h"
#include "PaySchedule.hpp"

#include "JacksOrBetterBase.cpp"

namespace PaySchedule
{
namespace AllAmerican
{
    void registerPaySchedules()
    {
        Manager_t::registerGame(Game::AllAmerican, string("All American"), string("Jacks.dat"));
    }

    using namespace Hand::Value;
    
    #define REGISTER_ALL_AMERICAN    REGISTER_PAY_SCHEDULE2(PaySchedule::Game::AllAmerican, JacksOrBetter::Base::payLines, payLines, estimatedReturn, 0)

    namespace ps_40
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,   4000,   0,
            StraightFlush,              200,      0,   0,
            FourOfAKind,                40,       0,   0,
            FullHouse,                  8,        0,   0,
            Flush,                      8,        0,   0,
            Straight,                   8,        0,   0,
            ThreeKind,                  3,        0,   0,
            TwoPair,                    1,        0,   0,
            Hand::Value::JacksOrBetter, 1,        0,   0,
        };
        static const double estimatedReturn = 100.72;
        
        REGISTER_ALL_AMERICAN;
        //REGISTER_PAY_SCHEDULE2(payLines, DeucesWild::Base::payLines, estimatedReturn);
    }

    namespace ps_35
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,   4000,   0,
            StraightFlush,              200,       0,   0,
            FourOfAKind,                35,       0,   0,
            FullHouse,                  8,        0,   0,
            Flush,                      8,        0,   0,
            Straight,                   8,        0,   0,
            ThreeKind,                  3,        0,   0,
            TwoPair,                    1,        0,   0,
            Hand::Value::JacksOrBetter, 1,        0,   0,
        };
        static const double estimatedReturn = 99.60;
        
        REGISTER_ALL_AMERICAN;
        //REGISTER_PAY_SCHEDULE2(payLines, DeucesWild::Base::payLines, estimatedReturn);
    }

    namespace ps_30
    {
        static const Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 250,   4000,   0,
            StraightFlush,              200,       0,   0,
            FourOfAKind,                30,       0,   0,
            FullHouse,                  8,        0,   0,
            Flush,                      8,        0,   0,
            Straight,                   8,        0,   0,
            ThreeKind,                  3,        0,   0,
            TwoPair,                    1,        0,   0,
            Hand::Value::JacksOrBetter, 1,        0,   0,
        };
        static const double estimatedReturn = 98.49;
        
        REGISTER_ALL_AMERICAN;
        //REGISTER_PAY_SCHEDULE2(payLines, DeucesWild::Base::payLines, estimatedReturn);
    }

}
}