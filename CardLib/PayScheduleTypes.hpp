////////////////////////////////////////////////////////////////////////////////
//
// PayScheduleTypes.hpp
//
// Copyright 2010 Mike McQueen. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#if _MSC_VER >= 1000
#pragma once
#endif

#ifndef Include_PAYSCHEDULETYPES_HPP
#define Include_PAYSCHEDULETYPES_HPP

#include "Hand.hpp"

namespace PaySchedule
{
    namespace Game
    {
        enum Id_t
        {
            First,
            
            AllAmerican = First,
            BonusDeuces,
            BonusPoker,
            BonusPokerDeluxe,
            DeucesWild,
            DoubleBonus,
            DoubleDblBonus,
            DoubleDeuces,
            JacksOrBetter,
            JokersWild,
            LooseDeuces,
            SuperAces,
            TripleDblBonus,
            WhiteHotAces,

            Last,
            Invalid,
        };
    }

    ////////////////////////////////////////////////////////////////////////////////

    namespace Static
    {
        namespace Flag
        {
            enum e
            {
                None      = 0,
                Group     = 1,
                NoWild    = 2,
            };
        }
        typedef Flag::e Flag_t;

        struct Payline_t
        {
            Hand::Value_t _handValue;
            size_t        _payout;
            size_t        _jackpot;
            size_t        _flags;

            bool
            testFlag(const Flag_t flag) const
            {
                return 0 != (_flags & flag);
            }
        };

        typedef vector<Payline_t> PaylineVector_t;

        ////////////////////////////////////////////////////////////////////////////////

        struct Data_t
        {
            PaylineVector_t _paylines;

            Data_t(
                const Payline_t* paylines,
                size_t           paylineCount)
            {
                for (size_t payline = 0; payline < paylineCount; ++payline)
                {
                    _paylines.push_back(paylines[payline]);
                }
            }

        private:

            Data_t();
            Data_t(const Data_t&);
            const Data_t& operator=(const Data_t&);
        };

    } // namespace Static

    ////////////////////////////////////////////////////////////////////////////////

    namespace Lookup
    {
        typedef vector<size_t>                 Payline_t;
        typedef pair<Hand::Value_t, Payline_t> PaylinePair_t;
        typedef vector<PaylinePair_t>          PaylineVector_t;

//        class Data_t;
//        struct CompareReturn;

        class Data_t :
            public PaylineVector_t
        {
        public:

            struct CompareReturn
            {
                bool operator() (const Data_t* a, const Data_t* b)
                {
                    return b->getReturn() < a->getReturn();
                }
            };

            typedef set<const Data_t*, CompareReturn> Set_t;

        private:

            Static::Data_t                  _staticSchedule;
            Hand::Value_t                   _lowestHandValue;
    
            PaySchedule::Game::Id_t         _gameId;
            double                          _payout;
            uint8_t                         _wildCard;
            size_t                          _paylineCount;

        public:

            Data_t(
                PaySchedule::Game::Id_t  gameId,
                const Static::Payline_t* paylines,
                      size_t             paylineCount,
                      size_t             coinCount,
                      double             payout);

            Data_t(
                PaySchedule::Game::Id_t  gameId,
                const Static::Payline_t* basePaylines,
                      size_t             basePaylineCount,
                const Static::Payline_t* paylines,
                      size_t             paylineCount,
                      size_t             coinCount,
                      double             payout,
                      uint8_t            wildCard);
    
            ~Data_t();

            PaySchedule::Game::Id_t    getGameId()       const    { return _gameId;          }
            double                     getReturn()       const    { return _payout;          }
            uint8_t                    getWildCard()     const    { return _wildCard;        }
            uint32_t                   getPaylineCount() const    { return (uint32_t)_paylineCount;    }

            Hand::Value_t
            Match(
                const Hand::Value_t handValue) const;

            Hand::Value_t
            oldMatch(
                const Hand::Value_t handValue) const;

            uint32_t
            getPaylineIndex(
                const Hand::Value_t handValue) const;

            const Payline_t&
            getPayline(
                uint32_t paylineIndex) const;

            Hand::Value_t
            getPaylineHandValue(
                uint32_t paylineIndex) const;
    
            uint32_t
            getPayout(
                uint32_t paylineIndex,
                uint32_t coinCount) const;
    
            uint32_t
            getHandPayout(
                Hand::Value_t handValue,
                uint32_t      coinCount) const
            {
                // TODO: add "normalized" boolean param if necessary
                /*        Hand::Value_t matchValue = Match(handValue);
                if (0 == matchValue)
                {
                return 0;
                }
                */
                return getPayout(getPaylineIndex(handValue), coinCount);
            }
    
            const string&
            getGameName() const;

        private:

            void
            initialize(
                size_t coinCount);

            bool
            equalHandValues(
                //        Hand::Value_t thisValue,
                const Static::Payline_t& staticPayline,
                      Hand::Value_t      matchValue) const;

            const PaylinePair_t&
            getPaylinePair(
                unsigned index) const;

            PaylinePair_t&
            getPaylinePair(
                unsigned index);

            Payline_t&
            getPayline(
                unsigned index);

            void
            updatePayline(
                      unsigned           paylineIndex,
                const Static::Payline_t& staticPayline,
                      unsigned           coinCount);
            
        }; // class Data_t

    } // namespace Lookup

    class Manager_t;

    namespace Game
    {
        
        class Data_t
        {
            
        public:
            
            string _name;
            string _filename;
            size_t _deckSize;
       
        public: // hack: friend not working for static calls from Manager_t on gcc

            friend class Manager_t;
            Lookup::Data_t::Set_t _scheduleSet;

        public:

            Data_t(
                const string& name,
                const string& filename,
                size_t        deckSize) 
                :
                _name(name),
                _filename(filename),
                _deckSize(deckSize)
            {
            }

            Data_t() {}
            
            size_t getScheduleCount() const
            {
                return _scheduleSet.size();
            }

        };

        typedef map<Id_t, Data_t> Map_t;

    } // namespace Game

} // namespace PaySchedule

#endif // Include_PAYSCHEDULETYPES_HPP

////////////////////////////////////////////////////////////////////////////////
