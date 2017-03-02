#if _MSC_VER >= 1000
#pragma once
#endif

#ifndef Include_SOLVERDATA_HPP
#define Include_SOLVERDATA_HPP

#include "Hand.hpp"

namespace Hand
{
namespace Solver
{
    struct Results_t         // results for one (of 32) ways to play a hand 
    {

    };

    namespace Default
    {
        struct Results_t : Solver::Results_t
        {
            vector<size_t> payouts;
            double         ev;

            size_t totalPayouts() const;
        };

        struct AllResults_t      // results for all (32) ways to play a hand
        {
            vector<Results_t> results;
            uint32_t          bestResultsIndex;  // results index, also bits of which cards to hold
        };
    }

    namespace Cuda
    {
        struct Results_t : Solver::Results_t        // results for one (of 32) ways to play a hand 
        {
            enum { MaxPayouts = 16 };

            size_t         payouts[MaxPayouts];
//            double         ev;

            //size_t totalPayouts() const;
        };

        struct AllResults_t      // results for all (32) ways to play a hand
        {
            enum { ResultCount = 32 };

            Results_t         results[ResultCount];
            uint32_t          bestResultsIndex;  // results index, also bits of which cards to hold
        };

    } // namespace Cuda

    class Data_t
    {
    public:

        virtual
        double
        getPayouts(
            const Hand_t& hand,
                  size_t  iterCount) = 0;

        virtual const
        Results_t&
        getResults(size_t cardBits) const = 0;

        virtual const
        Results_t&
        getBestResults() const = 0;

        virtual
        uint32_t
        getBestHoldCardBits() const
        {
            return 0;
        }

        virtual
        size_t
        getPaylineCount() const = 0;

        virtual
        void
        show(size_t cardCount) const = 0;
        
        virtual ~Data_t() {}
    };
}
}

#endif