////////////////////////////////////////////////////////////////////////////////
//
// SolverDataLookup.cpp
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SolverDataLookup.hpp"
#include "PaySchedule.hpp"
#include "Timer_t.h" 
#include "Log_t.h"
#include "IndexCombination.h"
#include "HandIterator_t.hpp"
#include "FileBuffer.h"
#include "HandAnalyzer_t.hpp"

#include <numeric>

//#define HACKY
//#define LOGGY

using namespace stdcomb;

////////////////////////////////////////////////////////////////////////////////

//extern size_t logLevel;

/*
extern void
LogHand(
    const Hand_t&    hand,
          size_t     handCounter,
          LogLevel_t atLogLevel);
*/

////////////////////////////////////////////////////////////////////////////////

namespace Hand
{
namespace Solver
{

namespace Default
{   

    size_t
    Results_t::totalPayouts() const
    {
        return std::accumulate(payouts.begin() + 1, payouts.end(), 0);
    }

}

namespace Lookup
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

//printf("getNibble(%d) byte(%02x) value(%02x) nibble(%02x)\n", iterCount, bytes[index], value, value & 0x0f);


    return value & 0x0f;
}

////////////////////////////////////////////////////////////////////////////////

//#define DEBUG_ONE_ROW

double
Data_t::
getPayouts(
    const Hand_t& hand,
    size_t        iterCount)
{
    static Card_t cards[5];

//    const size_t handIndex = Deck_t::getHandIndex(hand, PaySchedule::Manager_t::getGameData(_schedule.getGameId())._deckSize);
//    printf("handIndex(%d)\n", handIndex);

    bool show = true; // false; // (iterCount == 1) && (logLevel == Verbose);

    static const size_t waysToPlay = 32;
    _allResults.results.resize(waysToPlay);
    
    double bestEv = 0.0;

    size_t row = 0;
#ifndef DEBUG_ONE_ROW
    for (; row < waysToPlay; ++row)
#endif
    {
        // for each row
        //   calc payouts
        //   calc ev of payouts
        //   if ev > bestev then bestev = ev
        // bestev = best way to play that hand, should probably return bits

        bool firstPayout = true;

        PlusMinus *data = &(triangle[row][0]);

        Default::Results_t& results = _allResults.results[row];
        results.payouts.resize(_paylineCount);
        
#ifdef LOGLOTS
        printf(
            "row: %d\n", row);
#endif
        for (size_t col = 0; col < waysToPlay; ++col)
        {
            if (Z != data[col])
            {
                size_t cardCount = getCardsForBits(hand, cards, col);
                const uint32_t* payouts = getResultsForCards(cards, cardCount, iterCount);

#ifdef LOGLOTS
                printf(
                    "col(%2d)\n", col);
#endif
                if (P == data[col])
                {
                    // NOTE: can condense these two loops and put outer if (condition) inside it
                    for (size_t payline = 0; payline < _paylineCount; ++payline)
                    {
#ifdef LOGLOTS
                        printf(
                            "  (%2d:P%-3d)\n", payline, payouts[payline]);
#endif
                        if (firstPayout)
                        {
                            results.payouts[payline] = payouts[payline];
                        }
                        else
                        {
                            results.payouts[payline] += payouts[payline];
                        }
                    }
                    firstPayout = false;
                }
                else /*if (M == row[col])*/
                {
                    for (size_t payline = 0; payline < _paylineCount; ++payline)
                    {
#ifdef LOGLOTS
                        printf(
                            "  (%2d:M%-3d)\n", payline, payouts[payline]);

#endif
                        results.payouts[payline] -= payouts[payline];
                    }
                }
            }
        }

        size_t rowTotal = 0;
        //if (logLevel >= Ludicrous)
        {
            for (size_t payline = 0; payline < _paylineCount; ++payline)
            {
                const size_t payout = results.payouts[payline];
                rowTotal += payout;

#ifdef LOGLOTS
                printf(
                    "row(%d) payline(%2ld:%22s) (%ld)\n",
                    row,
                    payline,
                    (payline == 0 ? "nothing" : Hand::getName(_schedule.getPaylineHandValue(payline - 1)).c_str()),
                    payout);
#endif
            }
        }

        if (show)
        {
#ifdef LOGLOTS
                printf(
                       "row(%ld) total(%ld)\n------------\n", row, rowTotal);
#endif
        }
        size_t totalPayoutValue = 0;
        size_t totalPayoutCount = 0;
        for (size_t payline = 0; payline < _paylineCount; ++payline)
        {
            totalPayoutCount += results.payouts[payline];
            if (payline > 0)
            {
                size_t payout = _schedule.getPayout(payline - 1, 5);
                if (show)
                {
#ifdef LOGLOTS
                printf(
                       "(%2ld) count(%6ld) payout(%6ld) total(%10ld)\n",
                        payline, results.payouts[payline], payout, payout * results.payouts[payline]);
#endif
                }
                totalPayoutValue += payout * results.payouts[payline];
                //results[payline].ev = thisPayoutValue / totalPayoutCount; // but totalCount hasn't been calculated yet
            }
        }
        double ev = 0.0;
        if (totalPayoutValue > 0)
        {
            ev = (double)totalPayoutValue / double(totalPayoutCount);
        }

        results.ev = ev;
        if (show)
        {
#ifdef LOGLOTS
                printf(
                    "--------------\ntotalPayoutValue(%ld) ev(%2.2f)\n", totalPayoutValue, ev);
#endif
        }
        if (ev > bestEv)
        {
            bestEv = ev;
            _allResults.bestResultsIndex = row;
        }
    }

    return bestEv;
}

////////////////////////////////////////////////////////////////////////////////

uint32_t
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

static size_t g_iterCount;

const uint32_t*
Data_t::
getResultsForCards(
    const Card_t* cards,
          size_t  cardCount,
          size_t  iterCount) const
{
    g_iterCount = iterCount;

    const uint32_t* input = NULL;

    switch (cardCount)
    {

    case 0:
        input = &_handResults[0];
        break;

    case 1:
        input = &_oneCardResults[cards[0].getValue() * _paylineCount];
        break;

    case 2:
    case 3:
    case 4:
    case 5:
        {
            static uint32_t staticResults[20];
            fill(&staticResults[0], &staticResults[20], 0);
            input = &staticResults[0];

            switch (cardCount)
            {

            case 2:
                getResults<uint16_t, uint32_t>
                    (staticResults, _twoCardResultsMap, cards, cardCount);
                break;

            case 3:
                getResults<uint32_t, uint16_t>
                    (staticResults, _threeCardResultsMap, cards, cardCount);
                break;

            case 4:
                getResults<uint32_t, uint8_t>
                    (staticResults, _fourCardResultsMap, cards, cardCount);
                break;

            case 5:
                {
//                    const Card_t* cardsCopy = &cards[0];
//                    size_t handIndex = iterCount;
                    size_t handIndex = Deck_t::getHandIndex(&cards[0], cardCount, _deckSize);
                    size_t index = getNibble(_fiveCardResults, handIndex);
                    staticResults[index] = 1;
                    if (handIndex != iterCount)
                    {
                        printf ("%9ld - %9ld\n", iterCount, handIndex);
                    }
                }
                break;

            default:
#ifndef HACKY
                throw invalid_argument("getResultsForCards.1");
#else
                break;
#endif

            }
        }
        break;

    default:
#ifndef HACKY
        throw invalid_argument("getResultsForCards.2");
#endif
        break;
    }

    return input;
}

////////////////////////////////////////////////////////////////////////////////

template<class K, class N>
void
Data_t::
getResults(
          uint32_t*           results,
    const ResultsMap_t<K, N>& map,
    const Card_t*             cards,
          size_t              cardCount) const
{
    typedef typename ResultsMap_t<K, N>::PaylineData_t spd_t;

//printf("sizeof PaylineData_t(%d)\n", sizeof(spd_t));
    
#if GETRESULTS_EXCEPTIONS
try
{
#endif
    size_t handIndex;
    size_t dataIndex;
    const uint8_t* data = map.get(cards, cardCount, handIndex, dataIndex, _deckSize);

    uint8_t payoutCount = *data;
    if (payoutCount > _paylineCount)
    {
        printf("payOutCount(%d) > _paylineCount(%ld)\n", payoutCount, _paylineCount);
        throw runtime_error("Lookup::Data_t::getResults(): payoutCount > _paylineCount");
    }

#ifdef LOGGY
    if (handIndex == 0)
    {
        printf("ERROR: gr1: handIndex(%d) dataIndex(%d) payoutCount(%d) > paylineCount(%d)\n",
            handIndex, dataIndex, payoutCount, _paylineCount);
        for (int dataIndex = 0; dataIndex < 8; ++dataIndex)
        {
            printf("  data(%d):%02x (%d)\n", dataIndex, data[dataIndex], data[dataIndex]);
        }
    }
#endif

    if (cardCount == 5)
    {
        // payOutCount is always 1
        if (payoutCount != 1)
        {
            printf("ERROR: payoutCount(%d)\n", payoutCount);
            throw runtime_error("Lookup::Data_t::setResults(): payoutCount != 1");
        }
        results[*data] = 1;
    }
    else
    {
        const spd_t * paylineData = (const spd_t*)(data + 1);
        for (size_t payOutIndex = 0; payOutIndex < payoutCount; ++payOutIndex, ++paylineData)
        {
            if (paylineData->_payline > _paylineCount)
            {
                printf("paylineData->_payline(%d) > _paylineCount(%ld)\n", paylineData->_payline, _paylineCount);
                throw runtime_error("Lookup::Data_t::setResults(): paylineData->_payline > _paylineCount");
            }
            results[paylineData->_payline] = paylineData->_count;
        }
    }
#if GETRESULTS_EXCEPTIONS
}
catch (exception& e)
{
    printf("cardBits(%ld), count(%ld)\n", Card_t::getUniqueValue(cards, cardCount), cardCount);
    throw e;
}
#endif

}

////////////////////////////////////////////////////////////////////////////////

void
Data_t::
showPayouts(
    const Hand_t& hand,
          size_t  iterCount)
{
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
    if (cards.empty())
        iterCount = 0;
    throw runtime_error("showPayoutsForCards not implemented");
#endif
}

////////////////////////////////////////////////////////////////////////////////

void
Data_t::
save(
    const char* filename) const
{
    filename = NULL;
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
    
    uint32_t deckSize;
   
    uint32_t paylineCount;

    file.read((char*)&deckSize, sizeof(deckSize));
    file.read((char*)&paylineCount, sizeof(paylineCount));

    if (deckSize != _deckSize || paylineCount != _paylineCount)
    {
        printf("deckSize: %u, paylineCount: %u", _deckSize, paylineCount);
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
printf("%ld-card results(%ld) bytes(%ld)\n", 1L, _oneCardResults.size(), bytes);

    readTag(file, 2);
    bytes = _twoCardResultsMap.read(file, 2);
    totalBytes += bytes;
printf("%ld-card results(%ld) bytes(%ld)\n", 2L, _twoCardResultsMap.size(), bytes);

//return;
    readTag(file, 3);
    bytes = _threeCardResultsMap.read(file, 3);
    totalBytes += bytes;
printf("%ld-card results(%ld) bytes(%ld)\n", 3L, _threeCardResultsMap.size(), bytes);
    
    readTag(file, 4);
    bytes = _fourCardResultsMap.read(file, 4);
    totalBytes += bytes;
printf("%ld-card results(%ld) bytes(%ld)\n", 4L, _fourCardResultsMap.size(), bytes);

    readTag(file, 5);
    bytes = _fiveCardResults.size() * sizeof(_fiveCardResults[0]);
    file.read((char*)&_fiveCardResults[0], bytes);
printf("%ld-card results(%ld) bytes(%ld)\n", 5L, _fiveCardResults.size(), bytes);
    totalBytes += bytes;

    size_t fiveCardTotal = 0;
    vector<uint8_t>::const_iterator it = _fiveCardResults.begin();
    for (; it!= _fiveCardResults.end(); ++it)
    {
        fiveCardTotal += *it;
    }
printf("fiveCardTotal(%ld)\n", fiveCardTotal);

printf("Total Bytes: %ld\n", totalBytes);
}

////////////////////////////////////////////////////////////////////////////////

void
Data_t::
readTag(
    filebuffer& file, size_t /*section*/, size_t* /*version*/ /* =NULL */) 
{
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
    cardCount = 0;
    
    printf ("2CardData:\n");
    _twoCardResultsMap.show();
    printf ("3CardData:\n");
    _threeCardResultsMap.show();
    printf ("4CardData:\n");
    _fourCardResultsMap.show();
}

////////////////////////////////////////////////////////////////////////////////

} // namespace Lookup
} // namespace Solver
} // namespace Hand

////////////////////////////////////////////////////////////////////////////////
