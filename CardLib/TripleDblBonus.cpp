/*
 *  TripleDblBonus.cpp
 *  CardLib
 *
 *  Created by Mike McQueen on 8/27/10.
 *  Copyright 2010 Mike McQueen. All rights reserved.
 *
 */


#include "stdafx.h"
#include "Poker.h"
#include "PaySchedule.hpp"
#include "PayScheduleManager.hpp"
//#include "TripleDblBonus.h"

////////////////////////////////////////////////////////////////////////////////

namespace PaySchedule
{
    
namespace TripleDblBonus
{    
    using namespace PaySchedule;

    void registerPaySchedules()
    {        
        Manager_t::registerGame(Game::TripleDblBonus, string("Triple Double Bonus"), string("TDBonus.dat"));
    }

    using namespace Hand::Value;
    using namespace PaySchedule::Static::Flag;
    
    namespace Base
    {
        const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 0,      0,      0,

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
            
            StraightFlush,              0,      0,      0,
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

    #define REGISTER REGISTER_PAY_SCHEDULE2(Game::TripleDblBonus, TripleDblBonus::Base::payLines, payLines, estimatedReturn, 0);

    namespace ps_99_58
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 400,    4000,   0,
            FourAces_With_234,          400,    4000,   0,
            Four2s3s4s_With_Ace234,     400,    0,      0,
            FourAces,                   160,    0,      0,
            Four2s3s4s,                 80,     0,      0,
            Four5sThruKings,            50,     0,      0,
            StraightFlush,              50,     0,      0,
            FullHouse,                  9,      0,      0,
            Flush,                      7,      0,      0,
            Straight,                   4,      0,      0,
            ThreeKind,                  2,      0,      0,
            TwoPair,                    1,      0,      0,
            JacksOrBetter,              1,      0,      0,
        };
        static const double estimatedReturn = 99.58;
        REGISTER;
    }

    namespace ps_98_54
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 400,    4000,   0,
            FourAces_With_234,          400,    4000,   0,
            Four2s3s4s_With_Ace234,     400,    0,      0,
            FourAces,                   160,    0,      0,
            Four2s3s4s,                 80,     0,      0,
            Four5sThruKings,            50,     0,      0,
            StraightFlush,              50,     0,      0,
            FullHouse,                  8,      0,      0,
            Flush,                      7,      0,      0,
            Straight,                   4,      0,      0,
            ThreeKind,                  2,      0,      0,
            TwoPair,                    1,      0,      0,
            JacksOrBetter,              1,      0,      0,
        };
        static const double estimatedReturn = 98.54; // 98.54317512%
        REGISTER;        //         REGISTER_PAY_SCHEDULE2(Game::TripleDblBonus, DoubleDblBonus::Base::payLines, payLines, estimatedReturn);
    }

    namespace ps_98_15
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 400,    4000,   0,
            FourAces_With_234,          400,    4000,   0,
            Four2s3s4s_With_Ace234,     400,    0,      0,
            FourAces,                   160,    0,      0,
            Four2s3s4s,                 80,     0,      0,
            Four5sThruKings,            50,     0,      0,
            StraightFlush,              50,     0,      0,
            FullHouse,                  9,      0,      0,
            Flush,                      6,      0,      0,
            Straight,                   4,      0,      0,
            ThreeKind,                  2,      0,      0,
            TwoPair,                    1,      0,      0,
            JacksOrBetter,              1,      0,      0,
        };
        static const double estimatedReturn = 98.15; 
        REGISTER;     
    }

    namespace ps_97_11
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 400,    4000,   0,
            FourAces_With_234,          400,    4000,   0,
            Four2s3s4s_With_Ace234,     400,    0,      0,
            FourAces,                   160,    0,      0,
            Four2s3s4s,                 80,     0,      0,
            Four5sThruKings,            50,     0,      0,
            StraightFlush,              50,     0,      0,
            FullHouse,                  8,      0,      0,
            Flush,                      6,      0,      0,
            Straight,                   4,      0,      0,
            ThreeKind,                  2,      0,      0,
            TwoPair,                    1,      0,      0,
            JacksOrBetter,              1,      0,      0,
        };
        static const double estimatedReturn = 97.11; // 97.10882949%
        REGISTER;        //         REGISTER_PAY_SCHEDULE2(Game::TripleDblBonus, DoubleDblBonus::Base::payLines, payLines, estimatedReturn);
    }

    namespace ps_97_02
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 400,    4000,   0,
            FourAces_With_234,          400,    4000,   0,
            Four2s3s4s_With_Ace234,     400,    0,      0,
            FourAces,                   160,    0,      0,
            Four2s3s4s,                 80,     0,      0,
            Four5sThruKings,            50,     0,      0,
            StraightFlush,              50,     0,      0,
            FullHouse,                  9,      0,      0,
            Flush,                      5,      0,      0,
            Straight,                   4,      0,      0,
            ThreeKind,                  2,      0,      0,
            TwoPair,                    1,      0,      0,
            JacksOrBetter,              1,      0,      0,
        };
        static const double estimatedReturn = 97.02; 
        REGISTER;     
    }

#if 0
    namespace ps_8_5_4000
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 400,    4000,   0,
            FourAces_With_234,          400,    4000,   0,
            Four2s3s4s_With_Ace234,     400,    0,      0,
            FourAces,                   160,    0,      0,
            Four2s3s4s,                 80,     0,      0,
            Four5sThruKings,            50,     0,      0,
            StraightFlush,              50,     0,      0,
            FullHouse,                  8,      0,      0,
            Flush,                      5,      0,      0,
            Straight,                   4,      0,      0,
            ThreeKind,                  2,      0,      0,
            TwoPair,                    1,      0,      0,
            JacksOrBetter,              1,      0,      0,
        };
        static const double estimatedReturn = 95.97; // 95.96865163%
        REGISTER;        //         REGISTER_PAY_SCHEDULE2(Game::TripleDblBonus, DoubleDblBonus::Base::payLines, payLines, estimatedReturn);
    }

    namespace ps_7_6_4000
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 400,    4000,   0,
            FourAces_With_234,          400,    4000,   0,
            Four2s3s4s_With_Ace234,     400,    0,      0,
            FourAces,                   160,    0,      0,
            Four2s3s4s,                 80,     0,      0,
            Four5sThruKings,            50,     0,      0,
            StraightFlush,              50,     0,      0,
            FullHouse,                  7,      0,      0,
            Flush,                      6,      0,      0,
            Straight,                   4,      0,      0,
            ThreeKind,                  2,      0,      0,
            TwoPair,                    1,      0,      0,
            JacksOrBetter,              1,      0,      0,
        };
        static const double estimatedReturn = 96.06; //96.06440579%
        REGISTER;        //         REGISTER_PAY_SCHEDULE2(Game::TripleDblBonus, DoubleDblBonus::Base::payLines, payLines, estimatedReturn);
    }

    namespace ps_7_5_4000
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 400,    4000,   0,
            FourAces_With_234,          400,    4000,   0,
            Four2s3s4s_With_Ace234,     400,    0,      0,
            FourAces,                   160,    0,      0,
            Four2s3s4s,                 80,     0,      0,
            Four5sThruKings,            50,     0,      0,
            StraightFlush,              50,     0,      0,
            FullHouse,                  7,      0,      0,
            Flush,                      5,      0,      0,
            Straight,                   4,      0,      0,
            ThreeKind,                  2,      0,      0,
            TwoPair,                    1,      0,      0,
            JacksOrBetter,              1,      0,      0,
        };
        static const double estimatedReturn = 94.92; // 94.91777293%
        REGISTER;        //         REGISTER_PAY_SCHEDULE2(Game::TripleDblBonus, DoubleDblBonus::Base::payLines, payLines, estimatedReturn);
    }

    namespace ps_6_5_4000
    {
        static const
        Static::Payline_t payLines[] = 
        {
            RoyalFlush,                 400,    4000,   0,
            FourAces_With_234,          400,    4000,   0,
            Four2s3s4s_With_Ace234,     400,    0,      0,
            FourAces,                   160,    0,      0,
            Four2s3s4s,                 80,     0,      0,
            Four5sThruKings,            50,     0,      0,
            StraightFlush,              50,     0,      0,
            FullHouse,                  6,      0,      0,
            Flush,                      5,      0,      0,
            Straight,                   4,      0,      0,
            ThreeKind,                  2,      0,      0,
            TwoPair,                    1,      0,      0,
            JacksOrBetter,              1,      0,      0,
        };
        static const double estimatedReturn = 93.87; // 93.86727137%
        REGISTER;        //         REGISTER_PAY_SCHEDULE2(Game::TripleDblBonus, DoubleDblBonus::Base::payLines, payLines, estimatedReturn);
    }
#endif

}
  
}