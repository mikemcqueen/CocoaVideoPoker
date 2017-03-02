/*
 *  JokersWild.cpp
 *  CardLib
 *
 *  Copyright 2011 Mike McQueen. All rights reserved.
 */

#include "stdafx.h"
#include "Poker.h"
#include "PaySchedule.hpp"

namespace PaySchedule
{
namespace JokersWild
{
    void registerPaySchedules()
    {
        Manager_t::registerGame(Game::JokersWild, string("Jokers Wild"), string("Jokers.dat"), 53);
    }

    using namespace Hand::Value;
    using namespace PaySchedule::Static::Flag;
    
    #define REGISTER_KINGSORBETTER \
        REGISTER_PAY_SCHEDULE2(PaySchedule::Game::JokersWild, KingsOrBetter::Base::payLines, payLines, estimatedReturn, Joker)

    ////////////////////////////////////////////////////////////////////////////////

    namespace KingsOrBetter
    {
        namespace Base
        {
            using namespace Hand::Value;
            const Static::Payline_t payLines[] = 
            {
            RoyalFlushNoJokers,             0,      0,      Group,
                RoyalFlush,                 0,      0,      Group | NoWild,

                FiveOfAKind,                0,      0,      0,

            RoyalFlushWithJoker,            0,      0,      Group,
                RoyalFlushWild,             0,      0,      Group,

                StraightFlush,              0,      0,      0,
                FourOfAKind,                0,      0,      0,
                FullHouse,                  0,      0,      0,
                Flush,                      0,      0,      0,
                Straight,                   0,      0,      0,
                ThreeKind,                  0,      0,      0,
                TwoPair,                    0,      0,      0,

            Hand::Value::KingsOrBetter,     0,      0,      Group,
                PairOfKings,                0,      0,      0,
                PairOfAces,                 0,      0,      Group,
            };
            static const size_t payLineCount = _countof(payLines);
        }

        namespace ps_101
        {
            static const Static::Payline_t payLines[] = 
            {
                RoyalFlushNoJokers,         250,   4700,      0,
                FiveOfAKind,                200,      0,      0,
                RoyalFlushWithJoker,        100,      0,      0,
                StraightFlush,              50,       0,      0,
                FourOfAKind,                20,       0,      0,
                FullHouse,                  7,        0,      0,
                Flush,                      5,        0,      0,
                Straight,                   3,        0,      0,
                ThreeKind,                  2,        0,      0,
                TwoPair,                    1,        0,      0,
                Hand::Value::KingsOrBetter, 1,        0,      0,
            };
            static const double estimatedReturn = 101.00;
            REGISTER_KINGSORBETTER;
        }

        namespace ps_100_65
        {
            static const Static::Payline_t payLines[] = 
            {
                RoyalFlushNoJokers,         250,   4000,      0,
                FiveOfAKind,                200,      0,      0,
                RoyalFlushWithJoker,        100,      0,      0,
                StraightFlush,              50,       0,      0,
                FourOfAKind,                20,       0,      0,
                FullHouse,                  7,        0,      0,
                Flush,                      5,        0,      0,
                Straight,                   3,        0,      0,
                ThreeKind,                  2,        0,      0,
                TwoPair,                    1,        0,      0,
                Hand::Value::KingsOrBetter, 1,        0,      0,
            };
            static const double estimatedReturn = 100.65;
            REGISTER_KINGSORBETTER;
        }

/*
        namespace ps_100_46
        {
            static const Static::Payline_t payLines[] = 
            {
                RoyalFlushNoJokers,         250,   4700,      0,
//600!          FiveOfAKind,                600,      0,      0,
                RoyalFlushWithJoker,        100,      0,      0,
                StraightFlush,              50,       0,      0,
                FourOfAKind,                20,       0,      0,
                FullHouse,                  7,        0,      0,
                Flush,                      5,        0,      0,
                Straight,                   3,        0,      0,
                ThreeKind,                  2,        0,      0,
                TwoPair,                    1,        0,      0,
                Hand::Value::KingsOrBetter, 1,        0,      0,
            };
            static const double estimatedReturn = 100.65;
            REGISTER_KINGSORBETTER;
        }
*/
        namespace ps_99_29
        {
            static const Static::Payline_t payLines[] = 
            {
                RoyalFlushNoJokers,         250,   4000,      0,
                FiveOfAKind,                200,      0,      0,
                RoyalFlushWithJoker,        100,      0,      0,
                StraightFlush,              50,       0,      0,
                FourOfAKind,                18,       0,      0,
                FullHouse,                  7,        0,      0,
                Flush,                      5,        0,      0,
                Straight,                   3,        0,      0,
                ThreeKind,                  2,        0,      0,
                TwoPair,                    1,        0,      0,
                Hand::Value::KingsOrBetter, 1,        0,      0,
            };
            static const double estimatedReturn = 99.29;
            REGISTER_KINGSORBETTER;
        }

        namespace ps_99_08
        {
            static const Static::Payline_t payLines[] = 
            {
                RoyalFlushNoJokers,         250,   4000,      0,
                FiveOfAKind,                200,      0,      0,
                RoyalFlushWithJoker,        100,      0,      0,
                StraightFlush,              50,       0,      0,
                FourOfAKind,                20,       0,      0,
                FullHouse,                  6,        0,      0,
                Flush,                      5,        0,      0,
                Straight,                   3,        0,      0,
                ThreeKind,                  2,        0,      0,
                TwoPair,                    1,        0,      0,
                Hand::Value::KingsOrBetter, 1,        0,      0,
            };
            static const double estimatedReturn = 99.08;
            REGISTER_KINGSORBETTER;
        }

         namespace ps_98_94
        {
            static const Static::Payline_t payLines[] = 
            {
                RoyalFlushNoJokers,         250,   4000,      0,
                FiveOfAKind,                200,      0,      0,
                RoyalFlushWithJoker,        100,      0,      0,
                StraightFlush,              50,       0,      0,
                FourOfAKind,                18,       0,      0,
                FullHouse,                  7,        0,      0,
                Flush,                      5,        0,      0,
                Straight,                   3,        0,      0,
                ThreeKind,                  2,        0,      0,
                TwoPair,                    1,        0,      0,
                Hand::Value::KingsOrBetter, 1,        0,      0,
            };
            static const double estimatedReturn = 98.94;
            REGISTER_KINGSORBETTER;
        }

        namespace ps_98_44
        {
            static const Static::Payline_t payLines[] = 
            {
                RoyalFlushNoJokers,         250,   4700,      0,
                FiveOfAKind,                200,      0,      0,
                RoyalFlushWithJoker,        100,      0,      0,
                StraightFlush,              50,       0,      0,
                FourOfAKind,                17,       0,      0,
                FullHouse,                  7,        0,      0,
                Flush,                      5,        0,      0,
                Straight,                   3,        0,      0,
                ThreeKind,                  2,        0,      0,
                TwoPair,                    1,        0,      0,
                Hand::Value::KingsOrBetter, 1,        0,      0,
            };
            static const double estimatedReturn = 98.44;
            REGISTER_KINGSORBETTER;
        }

        namespace ps_98_09
        {
            static const Static::Payline_t payLines[] = 
            {
                RoyalFlushNoJokers,         250,   4000,      0,
                FiveOfAKind,                200,      0,      0,
                RoyalFlushWithJoker,        100,      0,      0,
                StraightFlush,              50,       0,      0,
                FourOfAKind,                17,       0,      0,
                FullHouse,                  7,        0,      0,
                Flush,                      5,        0,      0,
                Straight,                   3,        0,      0,
                ThreeKind,                  2,        0,      0,
                TwoPair,                    1,        0,      0,
                Hand::Value::KingsOrBetter, 1,        0,      0,
            };
            static const double estimatedReturn = 98.09;
            REGISTER_KINGSORBETTER;
        }

        namespace ps_97_95
        {
            static const Static::Payline_t payLines[] = 
            {
                RoyalFlushNoJokers,         250,   4000,      0,
                FiveOfAKind,                200,      0,      0,
                RoyalFlushWithJoker,        100,      0,      0,
                StraightFlush,              50,       0,      0,
                FourOfAKind,                15,       0,      0,
                FullHouse,                  8,        0,      0,
                Flush,                      5,        0,      0,
                Straight,                   3,        0,      0,
                ThreeKind,                  2,        0,      0,
                TwoPair,                    1,        0,      0,
                Hand::Value::KingsOrBetter, 1,        0,      0,
            };
            static const double estimatedReturn = 97.95;
            REGISTER_KINGSORBETTER;
        }

   } // namespace KingsOrBetter


} // namespace JokersWild
}