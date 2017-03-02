//////////////////////////////////////////////////////////////////////////////
//
// SOLVERDATAGENERATOR.HPP
// 
//////////////////////////////////////////////////////////////////////////////

#if _MSC_VER >= 1000
#pragma once
#endif

#ifndef Include_SOLVERDATAGENERATOR_HPP
#define Include_SOLVERDATAGENERATOR_HPP

#include "Poker.h"
#include "Card.hpp"
#include "Deck.hpp"
#include "Hand.hpp"
#include "SolverData_t.hpp"

#define OLDDATA

namespace Hand
{

//////////////////////////////////////////////////////////////////////////////

namespace Solver
{

#ifdef OLDDATA
    class ResultsMap_t
#else
    template<class Key_t, class ByteCount_t>
    class ResultsMap_t
#endif
    {
/*
        template<class T>
        struct PaylineCount_t
        {
            uint8_t _payline;
            T       _count;

            PaylineCount_t(uint8_t payline, T count)
                :
                _payline(payline),
                _count(count)
            {
            }
        };

        typedef PaylineCount_t<ByteCount_t> PaylineData_t;
*/

        typedef vector<size_t>       Data_t;

        const size_t _paylineCount;
        const size_t _paylineByteCount;
        Data_t       _data;

    public:

        ResultsMap_t(
            size_t cardCount,
            size_t paylineCount,
            size_t paylineByteCount)
            :
            _paylineCount(paylineCount),
            _paylineByteCount(paylineByteCount)
        {
            size_t bits = cardCount * 6;
            size_t size = (1 << bits) * paylineCount;
        #if 0
            printf("ResultsMap_t(%d, %d) : bits(%d) size(%d)\n", cardCount, paylineCount, bits, size);
        #endif

            _data.resize(size);
        }

        void
        put(
            const CardVector_t&   cards,
            const vector<size_t>& data)
        {
            memcpy(get(cards), &data[0], _paylineCount * sizeof(size_t));
        }

        const size_t*
        get(
            const CardVector_t& cards) const
        {
            return &_data[cards.getUniqueValue() * _paylineCount];       
        }

        const size_t*
        get(
           const Card_t* cards, size_t cardCount) const
        {
            return &_data[Card_t::getUniqueValue(cards, cardCount) * _paylineCount];       
        }

        size_t*
        get(
            const CardVector_t& cards) 
        {
            return &_data[cards.getUniqueValue() * _paylineCount];
        }

        size_t
        maxElement() const
        {
            return *max_element(_data.begin(), _data.end());
        }

        size_t
        size() const
        {
            //TODO: add size of hash map
            return _data.size() * sizeof(_data[0]);
        }

        void
        getElementCounts(vector<size_t>& counts, vector<size_t>& paylineCounts) const
        {
            getElementCounts(_data, counts, paylineCounts);
        }

        static void
        getElementCounts(const vector<size_t>& data, vector<size_t>& counts, vector<size_t>& paylineCounts)
        {
            fill(counts.begin(), counts.end(), 0);
            fill(paylineCounts.begin(), paylineCounts.end(), 0);
            for (Data_t::const_iterator it = data.begin(); data.end() != it;)
            {
                size_t paylineCount = 0;
                for (size_t payline = 0; payline < paylineCounts.size() - 1; ++payline, ++it)
                {
                    if (*it < counts.size())
                    {
                        ++counts[*it];
                    }
                    if (*it > 0)
                    {
                        ++paylineCount;
                    }
                }
                ++paylineCounts[paylineCount];
            }
        }

        size_t
        write(
            ofstream& file) const
        {
            size_t bytes = _data.size() * sizeof(_data[0]);
            file.write(reinterpret_cast<const char*>(&_data[0]), bytes);
            return bytes;
        }

        template<class Key_t>
        size_t
        writeCompressed(
            ofstream& file,
            size_t byteCount) const
        {
            size_t handCount = 0;

bool first = true;

            for (Data_t::const_iterator it = _data.begin(); _data.end() != it; it += _paylineCount)
            {
                Data_t::const_iterator itPayline = it;
                for (size_t payline = 0; payline < _paylineCount; ++payline, ++itPayline)
                {
                    if (*itPayline > 0)
                    {
                        ++handCount;
                        break;
                    }
                }
            }
            file.write((const char*)&handCount, sizeof(handCount));
            printf ("handCount(%ld)\n", handCount);

            size_t bytes = 0;
            Key_t cardBits = 0;

            for (Data_t::const_iterator it = _data.begin(); _data.end() != it; ++cardBits)
            {
                Data_t::const_iterator itPayline = it;
                unsigned char payOutCount = 0;
                for (size_t payline = 0; payline < _paylineCount; ++payline, ++it)
                {
                    if (*it > 0)
                    {
                        ++payOutCount;
                    }
                }
                if (payOutCount > 0)
                {
/*
if (4 == byteCount && (cardBits <= 7))
{
    printf ("bits(%ld), payouts(%d)\n", (long)cardBits, payOutCount);
}
*/
                    file.write((char*)&cardBits, sizeof(cardBits));
                    file.write((char*)&payOutCount, sizeof(payOutCount));

                    bytes += sizeof(cardBits) + sizeof(payOutCount);

                    for (size_t payline = 0; payline < _paylineCount; ++payline, ++itPayline)
                    {
                        if (*itPayline > 0)
                        {
                            unsigned char ucPayline = (unsigned char)payline;
                            file.write((char*)&ucPayline, sizeof(ucPayline));
                            bytes += sizeof(ucPayline);
/*
                            if (first)
                            {
                                printf ("payline(%ld:%ld)\n", payline, *itPayline);
                            }
*/
                            switch (byteCount)
                            {
                            case 1: 
                                {
                                    unsigned char count = (unsigned char)*itPayline;
                                    file.write((const char*)&count, sizeof(count));
                                    bytes += sizeof(count);
                                }
                                break;
                            case 2: 
                                {
                                    unsigned short count = (unsigned short)*itPayline;
                                    file.write((const char *)&count, sizeof(count));
                                    bytes += sizeof(count);
                                }
                                break;
                            case 4: 
                                {
                                    unsigned int count = (unsigned int)*itPayline;
                                    file.write((const char *)&count, sizeof(count));
                                    bytes += sizeof(count);
                                }
                                break;
                            default:
                                throw std::invalid_argument("compressedWrite");
                            }
                        }
                    }
first = false;
                }
            }

            return bytes;
        }

        size_t
        read(
            ifstream& file) 
        {
            size_t bytes = _data.size() * sizeof(_data[0]);
            file.read(reinterpret_cast<char*>(&_data[0]), bytes);
            return bytes;
        }

        size_t
        readCompressed(
            ifstream& file,
            size_t    byteCount)
        {
            size_t handCount = 0;
            file.read((char*)&handCount, sizeof(handCount));

            printf("handCount (%ld)\n", handCount);

            bool needBits = true;
            size_t bytes = 0;
            size_t handIndex = 0;
            size_t processedHandCount = 0;

            size_t cardBits = 0;
            unsigned char payOutCount = 0;
            for (Data_t::iterator it = _data.begin(); processedHandCount < handCount; ++handIndex, it += _paylineCount)
            {
                bool show = false;

                if (needBits)
                {
                    file.read((char*)&cardBits, sizeof(cardBits));
                    file.read((char*)&payOutCount, sizeof(payOutCount));
                    needBits = false;
                    if (payOutCount > _paylineCount)
                    {
                        throw runtime_error("payOutCount > _paylineCount");
                    }
                }
                if (handIndex < cardBits)
                {
                    continue;
                }
                else if (handIndex > cardBits && !show)
                {
                    throw logic_error("Solver::ResultsMap_t::readCompressed(): handIndex > cardBits");
                }
                needBits = true;

                bytes += sizeof(cardBits) + sizeof(payOutCount);

                for (size_t payline = 0; payline < payOutCount; ++payline)
                {
                    unsigned char ucpayline;
                    file.read((char*)&ucpayline, sizeof(ucpayline));
                    bytes += sizeof(ucpayline);

                    if (show) printf("line (%d:", ucpayline);

                    switch (byteCount)
                    {
                    case 1: 
                        {
                            unsigned char count;
                            file.read((char*)&count, sizeof(count));
                            it[ucpayline] = count;
                            bytes += sizeof(count);
                            if (show) printf("%d)\n", count);
                        }
                        break;
                    case 2: 
                        {
                            unsigned short count;
                            file.read((char *)&count, sizeof(count));
                            it[ucpayline] = count;
                            bytes += sizeof(count);
                            if (show) printf("%d)\n", count);
                        }
                        break;
                    default:
                        throw std::invalid_argument("byteCount");
                    }
                }
                ++processedHandCount;
            }
            return bytes;
        }
    };


//////////////////////////////////////////////////////////////////////////////

class GeneratorData_t :
    public Solver::Data_t
{
private:

    const PaySchedule::Lookup::Data_t&   _schedule;
    const size_t           _paylineCount;
    const size_t           _deckSize;
    const size_t           _cardsPerHand;

    vector<size_t>         _handResults;
    vector<size_t>         _oneCardResults;
    vector<unsigned char>  _fiveCardResults;

    ResultsMap_t           _twoCardResultsMap;
    ResultsMap_t           _threeCardResultsMap;
    ResultsMap_t           _fourCardResultsMap;

public:

    // todo: only requires a subset of payschedule; the # and type of hands, not the payouts
    // full payout can be passed to generate
    GeneratorData_t(
        const PaySchedule::Lookup::Data_t& schedule,
              size_t         deckSize,
              size_t         cardsPerHand);

    ~GeneratorData_t();

    void
    generate(
        Deck_t deck); // copy

    void
    save(
        const char* filename) const;

    void
    load(
        const char* filename);

    double
    getPayouts(
        const Hand_t&    hand,
               size_t           iterCount);

    virtual
    const Results_t&
    getResults(
        size_t /*cardBits*/) const
    {
        throw runtime_error("not implemented");
    }
    
    virtual const Results_t&
    getBestResults() const
    {
        return getResults(0);
    }
    
    size_t
    getPaylineCount() const
    {
        return _paylineCount;
    }

    size_t
    size() const;

    void
    show(
        size_t cardCount) const;

    const vector<size_t>& getHandResults() { return _handResults; }

private:

    // Record data for a hand
    void
    record(
        const Hand_t& hand,
        size_t        iterCount,
        Hand::Value_t value);

    void
    showPayouts(
        const Hand_t& hand,
        size_t        iterCount);

    void
    showPayoutsForCombinations(
        const CardVector_t& cards,
        size_t              cardCount);

    void
    showPayoutsForCards(
        const CardVector_t& cards,
        size_t              iterCount);


    void
    checkCards(
        const CardVector_t& cards,
        size_t              key);

    const ResultsMap_t&
    getHandResultsMap(
        const CardVector_t& cards) const;

    ResultsMap_t&
    getHandResultsMap(
        const CardVector_t& cards);

    const ResultsMap_t&
    getHandResultsMap( 
        size_t cardCount) const;

    ResultsMap_t&
    getHandResultsMap(
        size_t cardCount);

    const ResultsMap_t&
    getHandResultsMapImpl( 
        size_t cardCount) const;

    void
    RecordCards(
        const CardVector_t& cards,
              size_t        index);

    void
    RecordHandCombinations(
        const Hand_t& hand,
              size_t  count,
              size_t  index);

    const size_t*
    getResultsForCards(
        const Card_t* cards,
              size_t  cardCount,
              size_t  iterCount) const;

    size_t
    getCardsForBits(
        const Hand_t& hand,
        Card_t* cards,
        size_t bits) const;

    void
        Init();

    void
    writeTag(
        ofstream& file, size_t section, size_t version = 0) const;

    void
    readTag(
        ifstream& file, size_t section, size_t* version = NULL);

private:

    GeneratorData_t();
    GeneratorData_t(const GeneratorData_t&);
    const GeneratorData_t& operator=(const GeneratorData_t&);
};

} // Solver

} // Hand

#endif // Include_SOLVERDATAGENERATOR_HPP
