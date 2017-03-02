////////////////////////////////////////////////////////////////////////////////
//
// SolverDataLookup.cpp
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SolverDataLookupCuda.hpp"
#include "PaySchedule.hpp"
#include "Timer_t.h" 
#include "Log_t.h"
#include "IndexCombination.h"
#include "CombinationsCuda.h"
#include "HandIterator_t.hpp"
#include "FileBuffer.h"
#include "Solver.cuh"

#include <numeric>

using namespace stdcomb;

////////////////////////////////////////////////////////////////////////////////

namespace Hand
{
namespace Solver
{
/*    size_t
    Results_t::totalPayouts() const;
    {
        return std::accumulate(payouts.begin(), payouts.end(), 0) - *payouts.begin();
    }*/
    
namespace Lookup
{
namespace Cuda
{

enum PlusMinus
{
    M = -1,
    Z = 0,
    P = 1
};

static PlusMinus triangle[32][32] =
{
    { P, M, M, P, M, P, P, M, M, P, P, M, P, M, M, P, M, P, P, M, P, M, M, P, P, M, M, P, M, P, P, M },
    { Z, P, Z, M, Z, M, Z, P, Z, M, Z, P, Z, P, Z, M, Z, M, Z, P, Z, P, Z, M, Z, P, Z, M, Z, M, Z, P },
    { Z, Z, P, M, Z, Z, M, P, Z, Z, M, P, Z, Z, P, M, Z, Z, M, P, Z, Z, P, M, Z, Z, P, M, Z, Z, M, P },
    { Z, Z, Z, P, Z, Z, Z, M, Z, Z, Z, M, Z, Z, Z, P, Z, Z, Z, M, Z, Z, Z, P, Z, Z, Z, P, Z, Z, Z, M },
    { Z, Z, Z, Z, P, M, M, P, Z, Z, Z, Z, M, P, P, M, Z, Z, Z, Z, M, P, P, M, Z, Z, Z, Z, P, M, M, P },
    { Z, Z, Z, Z, Z, P, Z, M, Z, Z, Z, Z, Z, M, Z, P, Z, Z, Z, Z, Z, M, Z, P, Z, Z, Z, Z, Z, P, Z, M },
    { Z, Z, Z, Z, Z, Z, P, M, Z, Z, Z, Z, Z, Z, M, P, Z, Z, Z, Z, Z, Z, M, P, Z, Z, Z, Z, Z, Z, P, M },
    { Z, Z, Z, Z, Z, Z, Z, P, Z, Z, Z, Z, Z, Z, Z, M, Z, Z, Z, Z, Z, Z, Z, M, Z, Z, Z, Z, Z, Z, Z, P },
    { Z, Z, Z, Z, Z, Z, Z, Z, P, M, M, P, M, P, P, M, Z, Z, Z, Z, Z, Z, Z, Z, M, P, P, M, P, M, M, P },
    { Z, Z, Z, Z, Z, Z, Z, Z, Z, P, Z, M, Z, M, Z, P, Z, Z, Z, Z, Z, Z, Z, Z, Z, M, Z, P, Z, P, Z, M },
    { Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, P, M, Z, Z, M, P, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, M, P, Z, Z, P, M },
    { Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, P, Z, Z, Z, M, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, M, Z, Z, Z, P },
    { Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, P, M, M, P, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, M, P, P, M },
    { Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, P, Z, M, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, M, Z, P },
    { Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, P, M, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, M, P },
    { Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, P, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, M },
    { Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, P, M, M, P, M, P, P, M, M, P, P, M, P, M, M, P },
    { Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, P, Z, M, Z, M, Z, P, Z, M, Z, P, Z, P, Z, M },
    { Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, P, M, Z, Z, M, P, Z, Z, M, P, Z, Z, P, M },
    { Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, P, Z, Z, Z, M, Z, Z, Z, M, Z, Z, Z, P },
    { Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, P, M, M, P, Z, Z, Z, Z, M, P, P, M },
    { Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, P, Z, M, Z, Z, Z, Z, Z, M, Z, P },
    { Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, P, M, Z, Z, Z, Z, Z, Z, M, P },
    { Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, P, Z, Z, Z, Z, Z, Z, Z, M },
    { Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, P, M, M, P, M, P, P, M },
    { Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, P, Z, M, Z, M, Z, P },
    { Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, P, M, Z, Z, M, P },
    { Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, P, Z, Z, Z, M },
    { Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, P, M, M, P },
    { Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, P, Z, M },
    { Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, P, M },
    { Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, Z, P },
};

////////////////////////////////////////////////////////////////////////////////

Data_t::
Data_t(
    const PaySchedule::Lookup::Data_t& schedule,
          size_t                       deckSize,
          size_t                       cardsPerHand)
: 
    _schedule(schedule),
    _deckSize(deckSize),
    _cardsPerHand(cardsPerHand),
    _paylineCount(schedule.getPaylineCount() + 1), // +1 = "no match" count
    _twoCardResultsMap(2, _paylineCount, 4),  // TODO: last param, sizeof map.ByteCount_t
    _threeCardResultsMap(3, _paylineCount, 2),
    _fourCardResultsMap(4, _paylineCount, 1)
{
    Init();
}

////////////////////////////////////////////////////////////////////////////////

Data_t::
~Data_t()
{
}

////////////////////////////////////////////////////////////////////////////////

void
Data_t::
Init()
{
    _handResults.resize(_paylineCount);
    fill(_handResults.begin(), _handResults.end(), 0);
    _oneCardResults.resize(_deckSize * _paylineCount);
    fill(_oneCardResults.begin(), _oneCardResults.end(), 0);

    size_t comb = Combinations::Calculate(_deckSize, _cardsPerHand);
    printf("DeckSize(%ld) CardsPerHand(%ld) Combinations(%ld)\n", _deckSize, _cardsPerHand, comb); 

    _fiveCardResults.resize(comb / 2 + 1);
    fill(_fiveCardResults.begin(), _fiveCardResults.end(), 0);
}

////////////////////////////////////////////////////////////////////////////////

/*
size_t
Data_t::
size() const
{
    return _handResults.size() * sizeof(_handResults[0]) + 
        _oneCardResults.size() * sizeof(_oneCardResults[0]) +
        _twoCardResultsMap.size() +
        _threeCardResultsMap.size() +
        _fourCardResultsMap.size() +
        _fiveCardResults.size() * sizeof(_fiveCardResults[0]);
}
*/

////////////////////////////////////////////////////////////////////////////////

size_t
getNibble(
    const vector<unsigned char>& bytes,
          size_t                 iterCount)
{
    size_t index = iterCount / 2;
    unsigned char value = bytes[index];
    if (iterCount % 2 == 0)
    {
        value >>= 4;
    }

    return value & 0x0f;
}

////////////////////////////////////////////////////////////////////////////////

//#define VALIDATE_CUDA
#define USE_CUDA

double
Data_t::
getPayouts(
    const Hand_t& hand,
    size_t        iterCount)
{
iterCount; hand; return 6.6; //    return Hand::Solver::Cuda::processHand(hand, iterCount, _paylineCount);
}

////////////////////////////////////////////////////////////////////////////////

double
Data_t::
getManyPayouts(
    const Card_t* cards,
          size_t  handCount,
          size_t* iterCounts,
          bool    shared)
{
    return Hand::Solver::Cuda::processManyHands(cards, handCount, iterCounts, _paylineCount, shared);
}

////////////////////////////////////////////////////////////////////////////////

size_t
Data_t::
getCardsForBits(
    const Hand_t& hand,
          Card_t* cards,
          size_t  bits) const
{
    size_t cardCount = 0;
    if (bits > 0)
    {
        for (size_t card = 0; card < hand.size(); ++card)
        {
            if (0 != (bits & (1 << card)))
            {
                cards[cardCount++] = hand[card];
            }
        }
    }
    return cardCount;
}

////////////////////////////////////////////////////////////////////////////////

void
Data_t::
showPayouts(
    const Hand_t& hand,
          size_t  iterCount)
{
    hand; iterCount;
    if (iterCount == 0)
    {
        CardVector_t cards(1);
        int count = 1;
        for (Hand_t::const_iterator it = hand.begin(); it != hand.end(); ++it)
        {
            cards[0] = *it;
            printf("(1:%2d): ", count++);
            showPayoutsForCards(cards, 0);
        }
        showPayoutsForCombinations(hand, 2);
        showPayoutsForCombinations(hand, 3);
        showPayoutsForCombinations(hand, 4);
//        showPayoutsForCards(hand, iterCount);
    }
}

////////////////////////////////////////////////////////////////////////////////

void
Data_t::
showPayoutsForCombinations(
    const CardVector_t& cards,
    size_t cardCount)
{
    Hand_t hand(cardCount);

    Hand::Iterator_t iter(cards, cardCount);
    size_t count = 1;
    do
    {
        iter.deal(hand);
        printf("(%ld:%2ld): ", cardCount, count);
        showPayoutsForCards(hand, 0);
        ++count;
    }
    while (iter.Next());
}

////////////////////////////////////////////////////////////////////////////////

void
Data_t::
showPayoutsForCards(
    const CardVector_t& cards,
          size_t        iterCount)
{
#if 0
    const size_t* results;

    if (cards.size() == 1)
    {
        results = &_oneCardResults[cards[0].getValue() * _paylineCount];
    }
    else if (cards.size() == 5)
    {
        throw logic_error("showPayoutsForCards error"); iterCount;
    }
    else
    {
        const ResultsMap_t& map = getHandResultsMap(cards);
        results = map.get(cards);
    }
    size_t sum = std::accumulate(results, results + _paylineCount, 0);

    printf ("%15s (%d)\n", Hand::Analyzer_t::GetString(cards), sum);
#else
    cards; iterCount;
    throw runtime_error("showPayoutsForCards not implemented");
#endif
}

////////////////////////////////////////////////////////////////////////////////

void
Data_t::
save(
    const char* filename) const
{
    filename;
    throw runtime_error("Data_t::save() not implemented");
}

////////////////////////////////////////////////////////////////////////////////

void
Data_t::
writeTag(
    ofstream& file, size_t section, size_t version) const
{
    char prefix[] = "SD";

    stringstream ss;
    ss << prefix << section << version;

    printf("tag = %s\n", ss.str().c_str());

    file.write(ss.str().c_str(), ss.str().length());
}

////////////////////////////////////////////////////////////////////////////////

#define LOAD_LOG

void
Data_t::
load(
    const char* filename)
{ 
    printf("opening file: %s\n", filename);
    filebuffer file(filename);

    printf("reading first tag\n");
    readTag(file, 0);
    printf("reading first tag... done.\n");
    
    size_t deckSize;
   
    size_t paylineCount;

    file.read((char*)&deckSize, sizeof(deckSize));
    file.read((char*)&paylineCount, sizeof(paylineCount));

    if (deckSize != _deckSize || paylineCount != _paylineCount)
    {
        throw runtime_error("Solver::Data_t::load(): invalid deck size or payline count");
    }

    size_t totalBytes = 0;
    size_t bytes = 0;

    bytes = _handResults.size() * sizeof(_handResults[0]);
    file.read((char*)&_handResults[0], bytes);
    totalBytes += bytes;

    readTag(file, 1);
    bytes = _oneCardResults.size() * sizeof(_oneCardResults[0]);
    file.read((char*)&_oneCardResults[0], bytes);
    totalBytes += bytes;
#ifdef LOAD_LOG
    printf("%ld-card results(%ld) bytes(%ld)\n", 1L, _oneCardResults.size(), bytes);
#endif

    readTag(file, 2);
    _twoCardResultsMap.read(file, 2);
    bytes = _twoCardResultsMap.dataSize(true);
    totalBytes += bytes;
#ifdef LOAD_LOG
    printf("%ld-card map(%ld) data(%ld) bytes(%ld)\n", 2L, _twoCardResultsMap.mapSize(), _twoCardResultsMap.dataSize(), bytes);
#endif

    readTag(file, 3);
    _threeCardResultsMap.read(file, 3);
    bytes = _threeCardResultsMap.dataSize(true);
    totalBytes += bytes;
#ifdef LOAD_LOG
    printf("%ld-card map(%ld) data(%ld) bytes(%ld)\n", 3L, _threeCardResultsMap.mapSize(), _threeCardResultsMap.dataSize(), bytes);
#endif
    
    readTag(file, 4);
    _fourCardResultsMap.read(file, 4);
    bytes = _fourCardResultsMap.dataSize(true);
    totalBytes += bytes;
#ifdef LOAD_LOG
    printf("%ld-card map(%ld) data(%d) bytes(%ld)\n", 4L, _fourCardResultsMap.mapSize(), _fourCardResultsMap.dataSize(), bytes);
#endif

    readTag(file, 5);
    bytes = _fiveCardResults.size() * sizeof(_fiveCardResults[0]);
    file.read((char*)&_fiveCardResults[0], bytes);
#ifdef LOAD_LOG
    printf("%ld-card results(%ld) bytes(%ld)\n", 5L, _fiveCardResults.size(), bytes);
#endif
    totalBytes += bytes;

#ifdef LOAD_LOG
    printf("Total Bytes: %ld\n", totalBytes);
#endif

}

////////////////////////////////////////////////////////////////////////////////

void
Data_t::
readTag(
//    ifstream& file, size_t section, size_t* version /* =NULL */) 
    filebuffer& file, size_t section, size_t* version /* =NULL */) 
{
section; version;
    char buf[5];
    file.read(buf, 4);
    buf[4] = '\0';
    if (0 != strncmp(buf, "SD", 2))
    {
       throw runtime_error("Solver::Data_t::readTag(): Tag prefix mismatch");
    }

    printf("tag = %s\n", buf);
}

void
Data_t::
show(
    size_t cardCount) const
{
cardCount;
/*
    printf ("2CardData:\n");
    _twoCardResultsMap.show();
    printf ("3CardData:\n");
    _threeCardResultsMap.show();
    printf ("4CardData:\n");
    _fourCardResultsMap.show();
*/
}

////////////////////////////////////////////////////////////////////////////////

} // namespace Cuda
} // namespace Lookup
} // namespace Solver
} // namespace Hand

////////////////////////////////////////////////////////////////////////////////
