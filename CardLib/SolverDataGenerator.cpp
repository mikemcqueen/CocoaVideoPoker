////////////////////////////////////////////////////////////////////////////////
//
// SolverDataGenerator.cpp
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SolverDataGenerator.hpp"
#include "PaySchedule.hpp"
#include "Timer_t.h" 
#include "Log_t.h"
#include "Combinations.h"
#include "IndexCombination.h"
#include "HandAnalyzer_t.hpp"
#include "HandIterator_t.hpp"
#include <numeric>

using namespace stdcomb;

////////////////////////////////////////////////////////////////////////////////


extern void
LogHand(
    const Hand_t&    hand,
          size_t     handCounter,
          LogLevel_t atLogLevel);

////////////////////////////////////////////////////////////////////////////////


namespace Hand
{
namespace Solver
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

GeneratorData_t::
GeneratorData_t(
                const PaySchedule::Lookup::Data_t& schedule,
          size_t         deckSize,
          size_t         cardsPerHand)
: 
    _schedule(schedule),
    _deckSize(deckSize),
    _cardsPerHand(cardsPerHand),
    _paylineCount(schedule.getPaylineCount() + 1),
    _twoCardResultsMap(2, _paylineCount, 4),
    _threeCardResultsMap(3, _paylineCount, 2),
    _fourCardResultsMap(4, _paylineCount, 1)
{
    Init();
}

////////////////////////////////////////////////////////////////////////////////

GeneratorData_t::
~GeneratorData_t()
{
}
////////////////////////////////////////////////////////////////////////////////

void
GeneratorData_t::
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
GeneratorData_t::
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

/*
void
GeneratorData_t::
Reset()
{
}
*/

////////////////////////////////////////////////////////////////////////////////

size_t
getNibble(const vector<unsigned char>& bytes, size_t iterCount)
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

void
setNibble(vector<unsigned char>& bytes, size_t iterCount, unsigned char nibble)
{
    size_t index = iterCount / 2;
    unsigned char& value = bytes[index];
    if (iterCount % 2 == 0)
    {
        value &= 0x0f;
        value |= (nibble << 4);
    }
    else
    {
        value &= 0xf0;
        value |= nibble;
    }
}

////////////////////////////////////////////////////////////////////////////////

void
GeneratorData_t::
generate(
    Deck_t deck) // copy
{
    using namespace Hand;

    Timer_t timer(L"Calculating...", true);

    Card_t::Value_t wildCardNumber = 0;
    if (_schedule.getGameId() == PaySchedule::Game::DeucesWild)
    {
        wildCardNumber = Deuce;
    }
    else if (_schedule.getGameId() == PaySchedule::Game::JokersWild)
    {
        wildCardNumber = Joker;
    }

    // Calculate data for all the possible hands & draws
    Hand_t hand(_cardsPerHand);
    Hand::Iterator_t handIter(deck, _cardsPerHand);
    do
    {
        handIter.deal(hand);

        Hand::Analyzer_t analyzer(hand, wildCardNumber);
        Hand::Value_t realValue = analyzer.GetValue();
        Hand::Value_t payoutValue = _schedule.Match(realValue);

        LogHand(hand, handIter.GetCount(), Ludicrous);

        record(hand, handIter.GetCount() - 1, payoutValue);
    }
    while (handIter.Next());

    printf("\nSolver::GeneratorData_t::generate():  %ld-card hands(%ld)\n",
           _cardsPerHand, handIter.GetCount());
}

////////////////////////////////////////////////////////////////////////////////

void
GeneratorData_t::
record(
    const Hand_t& hand,
    size_t        iterCount,
    Hand::Value_t value)
{
    size_t index = (value == 0) ? 0 : _schedule.getPaylineIndex(value) + 1;

    ++_handResults[index];

    for (Hand_t::const_iterator it = hand.begin(); hand.end() != it; ++it)
    {
        ++_oneCardResults[it->getValue() * _paylineCount + index];
    }

    RecordHandCombinations(hand, 2, index);
    RecordHandCombinations(hand, 3, index);
    RecordHandCombinations(hand, 4, index);

    setNibble(_fiveCardResults, iterCount, (unsigned char)index);
}

////////////////////////////////////////////////////////////////////////////////

void
GeneratorData_t::
RecordHandCombinations(
    const Hand_t& hand,
          size_t  count,
          size_t  index)
{
    Hand::Iterator_t iter(hand, count);

    Hand_t cards(count);
    size_t recordCount = 0;
    do
    {
        iter.deal(cards);
        RecordCards(cards, index);
        ++recordCount;
    }
    while (iter.Next());

    if (Ludicrous == logLevel)
    {
//        printf("Recorded %d combinations for %d cards\n", recordCount, count);
    }
}

////////////////////////////////////////////////////////////////////////////////

void
GeneratorData_t::
RecordCards(
    const CardVector_t& cards,
    size_t              index)
{
    ResultsMap_t& map = getHandResultsMap(cards);
    size_t* results = map.get(cards);
    ++results[index];
}

////////////////////////////////////////////////////////////////////////////////
/*
void
GeneratorData_t::
checkCards(
   const CardVector_t& cards,
   size_t key)
{
#if 1
cards; key;
#else
    size_t check[] = { 30, 39, 43, 47, 51 };

    if (cards.size() == _countof(check))
    {
        bool match = true;
        for (size_t index = 0; index < cards.size(); ++index)
        {
            if (cards[index].getValue() != check[index])
            {
                match = false;
                break;
            }
        }
        if (match)
        {
            printf ("checkCards match, key (%d)\n", key);
        }
    }
#endif
}
*/
////////////////////////////////////////////////////////////////////////////////

const ResultsMap_t&
GeneratorData_t::
getHandResultsMap(
    const CardVector_t& cards) const
{
    return getHandResultsMap(cards.size());
}

////////////////////////////////////////////////////////////////////////////////

ResultsMap_t&
GeneratorData_t::
getHandResultsMap(
    const CardVector_t& cards)
{
    return getHandResultsMap(cards.size());
}

////////////////////////////////////////////////////////////////////////////////

const ResultsMap_t&
GeneratorData_t::
getHandResultsMap(
    size_t cardCount) const
{
    return getHandResultsMapImpl(cardCount);
}

////////////////////////////////////////////////////////////////////////////////

ResultsMap_t&
GeneratorData_t::
getHandResultsMap( 
    size_t cardCount)
{
    return const_cast<ResultsMap_t&>(getHandResultsMapImpl(cardCount));
}

////////////////////////////////////////////////////////////////////////////////

const ResultsMap_t&
GeneratorData_t::
getHandResultsMapImpl( 
    size_t cardCount) const
{
    switch (cardCount)
    {
    case 2: return _twoCardResultsMap;
    case 3: return _threeCardResultsMap;
    case 4: return _fourCardResultsMap;
    default:
        throw std::invalid_argument("getHandResultsMap");
    }
}

////////////////////////////////////////////////////////////////////////////////

double
GeneratorData_t::
getPayouts(
    const Hand_t&   hand,
    size_t          iterCount)
{
    vector<size_t> payouts(_paylineCount);
    const size_t* results;
    static vector<size_t> bestPayouts(_paylineCount);

    static Card_t cards[5];

    bool show = (iterCount == 1) && (logLevel == Verbose);

    double bestEv = 0.0;

    for (size_t row = 0; row < 32; ++row)
    {
        // todo:
        // for each row
        //   calc payouts
        //   calc ev of payouts
        //   if ev > bestev then bestev = ev
        // bestev = best way to play that hand, should probably return bits

        bool firstPayout = true;

        PlusMinus *data = &(triangle[row][0]);

        for (size_t col = 0; col < 32; ++col)
        {
            if (Z != data[col])
            {
                size_t cardCount = getCardsForBits(hand, cards, col);
                results = getResultsForCards(cards, cardCount, iterCount);

                if (P == data[col])
                {
                    for (size_t payline = 0; payline < payouts.size(); ++payline)
                    {
                        if (firstPayout)
                        {
                            payouts[payline] = results[payline];
                        }
                        else
                        {
                            payouts[payline] += results[payline];
                        }
                    }
                    firstPayout = false;
                }
                else /*if (M == row[col])*/
                {
                    for (size_t payline = 0; payline < payouts.size(); ++payline)
                    {
                        payouts[payline] -= results[payline];
                    }
                }
            }
        }

        if (show)
        {
            printf("row(%ld) \n------------\n", row);
        }
        size_t totalPayoutValue = 0;
        size_t totalPayoutCount = 0;
        for (size_t payline = 0; payline < payouts.size(); ++payline)
        {
            totalPayoutCount += payouts[payline];
            if (payline > 0)
            {
                size_t payout = _schedule.getPayout(payline - 1, 5);
                if (show)
                {
                    printf ("(%2ld) count(%6ld) payout(%6ld) total(%10ld)\n",
                        payline, payouts[payline], payout, payout * payouts[payline]);
                }
                totalPayoutValue += payout * payouts[payline];
            }
        }
        double ev = 0.0;
        if (totalPayoutValue > 0)
        {
            ev = (double)totalPayoutValue / double(totalPayoutCount);
        }
        if (show)
        {
            printf("--------------\ntotalPayoutValue(%ld) ev(%2.2f)\n", totalPayoutValue, ev);
        }
        if (ev > bestEv)
        {
            bestEv = ev;
                //_allResults.bestResultsIndex = row;
            copy(payouts.begin(), payouts.end(), bestPayouts.begin());
        }
#if 0
        for (size_t index = 0; index < payouts.size(); ++index)
        {
            if (row == 0)
            {
                combinations[index] = payouts[index];
            }
            else
            {
                combinations[index] += payouts[index];
            }
        }
#endif
    }

    copy(bestPayouts.begin(), bestPayouts.end(), payouts.begin());
    return bestEv;
}

////////////////////////////////////////////////////////////////////////////////

size_t
GeneratorData_t::
getCardsForBits(
    const Hand_t& hand,
    Card_t* cards,
    size_t bits) const
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

const size_t*
GeneratorData_t::
getResultsForCards(
    const Card_t*   cards,
          size_t    cardCount,
          size_t    iterCount) const
{
    const size_t* input;

    switch (cardCount)
    {
    case 0:
        input = &_handResults[0];
        break;
    case 1:
        input = &_oneCardResults[cards[0].getValue() * _paylineCount];
        break;
    case 5:
        {
            static size_t staticResults[20];
            size_t index = getNibble(_fiveCardResults, iterCount);
            fill(&staticResults[0], &staticResults[20], 0);
            staticResults[index] = 1;
            input = &staticResults[0];
        }
        break;

    default:
        const ResultsMap_t& map = getHandResultsMap(cardCount);
        input = map.get(cards, cardCount);
        break;
    }
    return input;
}

////////////////////////////////////////////////////////////////////////////////

void
GeneratorData_t::
showPayouts(
    const Hand_t& hand,
          size_t  iterCount)
{
    iterCount = 0;
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
GeneratorData_t::
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
GeneratorData_t::
showPayoutsForCards(
    const CardVector_t& cards,
          size_t        iterCount)
{
    iterCount = 0;
   
    const size_t* results;

    if (cards.size() == 1)
    {
        results = &_oneCardResults[cards[0].getValue() * _paylineCount];
    }
    else if (cards.size() == 5)
    {
        throw logic_error("showPayoutsForCards error");
    }
    else
    {
        const ResultsMap_t& map = getHandResultsMap(cards);
        results = map.get(cards);
    }
    size_t sum = std::accumulate(results, results + _paylineCount, 0);

    printf ("%15s (%ld)\n", Hand::Analyzer_t::GetString(cards), sum);
}

////////////////////////////////////////////////////////////////////////////////

void
GeneratorData_t::
show(size_t cardCount) const
{
    vector<size_t> counts;
    vector<size_t> paylineCounts(_paylineCount + 1);

    if (cardCount == 2 || cardCount == 3 || cardCount == 4)
    {
        const ResultsMap_t& map = getHandResultsMap(cardCount);
        size_t maxElement = map.maxElement();
        printf("Map(%ld) max = %ld\n", cardCount, maxElement);
        if (cardCount == 2)
        {
            return;
        }
        counts.resize(maxElement + 1);
        size_t unique = 0;
        size_t nonZero = 0;
        map.getElementCounts(counts, paylineCounts);
        size_t index = 0;
        if (cardCount == 4)
        {
            for (vector<size_t>::const_iterator it = counts.begin(); counts.end() != it; ++it, ++index)
            {
                printf ("%ld:(%ld) ", index, *it);
                if (*it > 0)
                {
                    ++unique;
                }
                if (index > 0)
                {
                    nonZero += *it;
                }
            }
            printf("\nunique: %ld of %ld - nonZero(%ld)\n", unique, counts.size(), nonZero);
        }
    }
    if (cardCount == 5)
    {
        return;
    }
    size_t index = 0;
    printf("paylineCounts(%ld):\n", cardCount);
    size_t totalpaylineCounts = 0;
    size_t nonZeropaylineCounts = 0;
    for (vector<size_t>::const_iterator it = paylineCounts.begin(); paylineCounts.end() != it; ++it, ++index)
    {
        printf ("%ld:(%ld) ", index, *it);
        totalpaylineCounts += *it;
        if (index > 0)
        {
            nonZeropaylineCounts += *it;
        }
    }
    printf("\nTotal (%ld) NonZero(%ld)\n", totalpaylineCounts, nonZeropaylineCounts);
}

////////////////////////////////////////////////////////////////////////////////

void
GeneratorData_t::
save(
    const char* filename) const
{
    ofstream file(filename, ios::binary);
    file.exceptions(ios::failbit | ios::badbit);

    writeTag(file, 0);

    file.write((const char*)&_deckSize, sizeof(_deckSize));
    file.write((const char*)&_paylineCount, sizeof(_paylineCount));

    size_t totalBytes = 0;
    size_t bytes = 0;

    bytes = _handResults.size() * sizeof(_handResults[0]);
    file.write((const char*)&_handResults[0], bytes);
    totalBytes += bytes;

    writeTag(file, 1);
    bytes = _oneCardResults.size() * sizeof(_oneCardResults[0]);
    file.write((const char*)&_oneCardResults[0], bytes);
    totalBytes += bytes;

printf("%ld-card results(%ld) bytes(%ld)\n", 1L, _oneCardResults.size(), bytes);

    writeTag(file, 2);
//    bytes = _twoCardResultsMap.write(file);
    bytes = _twoCardResultsMap.writeCompressed<unsigned short>(file, 4);
    totalBytes += bytes;

printf("%ld-card results(%ld) bytes(%ld)\n", 2L, _twoCardResultsMap.size(), bytes);

    writeTag(file, 3);
    bytes = _threeCardResultsMap.writeCompressed<unsigned int>(file, 2);
    totalBytes += bytes;

printf("%ld-card results(%ld) bytes(%ld)\n", 3L, _threeCardResultsMap.size(), bytes);

    writeTag(file, 4);
    bytes = _fourCardResultsMap.writeCompressed<unsigned int>(file, 1);
    totalBytes += bytes;

printf("%ld-card results(%ld) bytes(%ld)\n", 4L, _fourCardResultsMap.size(), bytes);

    writeTag(file, 5);
    bytes = _fiveCardResults.size() * sizeof(_fiveCardResults[0]);
    file.write((const char*)&_fiveCardResults[0], bytes);
    totalBytes += bytes;

printf("%ld-card results(%ld) bytes(%ld)\n", 5L, _fiveCardResults.size(), bytes);
printf("Total Bytes: %ld\n", totalBytes);

}

////////////////////////////////////////////////////////////////////////////////

void
GeneratorData_t::
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
GeneratorData_t::
load(
    const char* filename)
{ 
    ifstream file(filename, ios::binary);
    file.exceptions(ios::failbit | ios::badbit);

    readTag(file, 0);

    size_t deckSize;
    size_t paylineCount;

    file.read((char*)&deckSize, sizeof(deckSize));
    file.read((char*)&paylineCount, sizeof(paylineCount));

    if (deckSize != _deckSize || paylineCount != _paylineCount)
    {
        throw runtime_error("Solver::GeneratorData_t::load(): invalid deck size or payline count");
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

printf("Bytes(%ld): %ld\n", 1L, bytes);

    readTag(file, 2);
    bytes = _twoCardResultsMap.read(file);
    totalBytes += bytes;

printf("Bytes(%ld): %ld\n", 2L, bytes);

    readTag(file, 3);
    bytes = _threeCardResultsMap.readCompressed(file, 2);
    totalBytes += bytes;

printf("Bytes(%ld): %ld\n", 3L, bytes);

    readTag(file, 4);
    bytes = _fourCardResultsMap.readCompressed(file, 1);
    totalBytes += bytes;

printf("Bytes(%ld): %ld\n", 4L, bytes);

    readTag(file, 5);
    bytes = _fiveCardResults.size() * sizeof(_fiveCardResults[0]);
    file.read((char*)&_fiveCardResults[0], bytes);
    totalBytes += bytes;

printf("Bytes(%ld): %ld\n", 5L, bytes);
printf("Total Bytes: %ld\n", totalBytes);
}

////////////////////////////////////////////////////////////////////////////////

void
GeneratorData_t::
readTag(
    ifstream& file, size_t section, size_t* version /* =NULL */) 
{
    section = 0;
    version = NULL;
    
    char buf[5];
    file.read(buf, 4);
    buf[4] = '\0';
    if (0 != strncmp(buf, "SD", 2))
    {
       throw runtime_error("Solver::GeneratorData_t::readTag(): Tag prefix mismatch");
    }

    printf("tag = %s\n", buf);
}

////////////////////////////////////////////////////////////////////////////////

} // namespace Solver
} // namespace Hand

////////////////////////////////////////////////////////////////////////////////
