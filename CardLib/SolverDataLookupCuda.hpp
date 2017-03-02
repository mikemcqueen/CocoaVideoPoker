//////////////////////////////////////////////////////////////////////////////
//
// SOLVERDATALOOKUPCUDA.HPP
// 
//////////////////////////////////////////////////////////////////////////////

#if _MSC_VER >= 1000
#pragma once
#endif

#ifndef Include_SOLVERDATALOOKUPCUDA_HPP
#define Include_SOLVERDATALOOKUPCUDA_HPP

#include "Poker.h"
#include "Card.hpp"
#include "Deck.hpp"
#include "Hand.hpp"
#include "HandIterator_t.hpp"
#include "SolverData_t.hpp"
#include "Combinations.h"
#include "Log_t.h"
#include "FileBuffer.h"

#include "PaySchedule.hpp"

#define ALIGNED_DATA

namespace Hand
{

//////////////////////////////////////////////////////////////////////////////

namespace Solver
{
namespace Lookup
{
namespace Cuda
{
    struct AlignedPaylineData_t
    {
        uint32_t _payline;
        uint32_t _count;

        AlignedPaylineData_t() :
            _payline(0),
            _count(0)
        {
        }

        AlignedPaylineData_t(
            uint32_t payline,
            uint32_t count)
            :
            _payline(payline),
            _count(count)
        {
        }
    };

    template<class Key_t, class ByteCount_t>
    class ResultsMap_t
    {
    public:

        template<class T>
        struct PaylineCount_t
        {
            uint8_t _payline;
            T       _count;

            PaylineCount_t() :
                _payline(0),
                _count(0)
            {
            }

            PaylineCount_t(
                uint8_t payline,
                T       count)
                :
                _payline(payline),
                _count(count)
            {
            }
        };

        typedef PaylineCount_t<ByteCount_t> PaylineData_t;
        
    private:

        typedef vector<uint32_t> Map_t;
#ifdef ALIGNED_DATA
        typedef vector<uint32_t> Data_t;
#else
        typedef vector<uint8_t>  Data_t;
#endif

        const size_t _paylineCount;
        const size_t _paylineByteCount;

    public: // hack?

        Map_t        _map;
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
            cardCount;
        }

#if 0
        const uint8_t*
        get(
           const Card_t* cards,
                 size_t  cardCount) const
        {
            extern size_t hackIterCount;
            extern size_t hackCardCount;

            size_t index = _map[Deck_t::getHandIndex(cards, cardCount)];
#if 0
            if (index != hackIterCount)
            {
                printf("%d: %9d - %9d\n", hackCardCount, hackIterCount, index);
            }
#endif
            return &_data[index];
        }

        const uint8_t*
        get(
            size_t key) const
        {
            return &_data[_map[key]];
        }
#endif

        size_t
        mapSize(const bool inBytes = false) const
        {
            return _map.size() * (inBytes ? sizeof(_map[0]) : 1);
        }

        size_t
        dataSize(const bool inBytes = false) const
        {
            return _data.size() * (inBytes ? sizeof(_data[0]) : 1);
        }

#if 0
        size_t
        sizeInBytes() const
        {
            return _data.size() * sizeof(_data[0]);
        }
#endif

        void
        show(
            Key_t key) const
        {
            key;
            throw std::runtime_error("Solver::Data::Lookup::ResultsMap_t::show() not implemented");
        }

        size_t
        write(
            ofstream& file) const
        {
            file;
            throw std::runtime_error("Solver::Data::Lookup::ResultsMap_t::write() not implemented");
        }

        bool
        logCheck(size_t keyIndex) const
        { keyIndex;
            return (logLevel == Verbose);// && (keyIndex >= 660) && (keyIndex <= 663);
        }

        size_t
        read(
            filebuffer& file,
            size_t      cardCount)
        {
            _map.clear();
            _data.clear();

            size_t byteCount = 0;

            // read the # of keys
            uint32_t keyCount;
            readBigEndian<uint32_t>(file, &keyCount);
            printf("keyCount(%d) sizeof Key_t(%ld) sizeof PaylineData_t(%ld)\n", keyCount, sizeof(Key_t), sizeof(PaylineData_t));

            _map.resize(keyCount);

            for (size_t keyIndex = 0; keyIndex < keyCount; ++keyIndex)
            {
                // read the key (cardBits) NOTE: this key isn't even used !?
                Key_t key;
                readBigEndian<Key_t>(file, &key);

                // read the # of paylines
                uint8_t paylineCount;
                file.read(reinterpret_cast<char*>(&paylineCount), sizeof(paylineCount));

                if (logCheck(keyIndex))
                {
                    printf ("index(%d) key(%ld), payouts(%d)\n", (int)keyIndex, (long)key, paylineCount);
                }

                size_t location = _data.size();

#ifdef ALIGNED_DATA
                static const size_t STRIDE = 8; // words; 32 / word_size
                if ((location % STRIDE) != 0)
                {
                    printf("location(%d) stride(%d) remainder(%d)", location, location / STRIDE, location % STRIDE);
                    throw std::exception("location % STRIDE != 0");
                }
                byteCount += sizeof(_data[0]);
#else
                byteCount += sizeof(paylineCount);
#endif
                _data.push_back(paylineCount);

                // read data for each payline
                for (size_t paylineIndex = 0; paylineIndex < paylineCount; ++paylineIndex)
                {
                    PaylineData_t paylineData;
                    file.read(reinterpret_cast<char*>(&paylineData._payline), sizeof(paylineData._payline));
                    readBigEndian<ByteCount_t>(file, &paylineData._count);

                    if (logCheck(keyIndex))
                    {
                        printf ("index(%ld) line(%d) count(%d)\n", paylineIndex, paylineData._payline, paylineData._count);
                    }
#ifdef ALIGNED_DATA
                    AlignedPaylineData_t alignedData(paylineData._payline, paylineData._count);
                    _data.insert(_data.end(), (uint32_t*)&alignedData, (uint32_t*)(&alignedData + 1));
                    byteCount += sizeof(alignedData);
#else
                    _data.insert(_data.end(), (uint8_t*)&paylineData, (uint8_t*)(&paylineData + 1));
                    byteCount += sizeof(paylineData);
#endif
                }
#ifdef ALIGNED_DATA
                static const uint32_t PAD_VALUE = 0x45454545;

                size_t endLocation = _data.size();
                size_t padWordCount = 0;
                if (endLocation % STRIDE > 0)
                {
                    padWordCount = STRIDE - endLocation % STRIDE;
                }
                _data.insert(_data.end(), padWordCount, PAD_VALUE);
                byteCount += padWordCount * sizeof(uint32_t);
#endif

                // insert map entry
                size_t slot = keyCount - keyIndex - 1;
                _map[slot] = location;

                // verify
                cardCount;
                size_t index = _map[slot];
#ifdef ALIGNED_DATA
                uint32_t* data = &_data[index];
#else
                uint8_t* data = &_data[index];
#endif
                if (*data != paylineCount)
                {
                    printf("*data(%d)\n", *data);
                    throw runtime_error("Lookup::Data_t::read(): *data != paylineCount");
                }
            }
#ifdef ALIGNED_DATA
            return _data.size() * sizeof(_data[0]);
#else
            return _data.size();
#endif
        }

    private:

        template<class T>
        void
        readBigEndian(
            filebuffer& file,
            T*        value)
        {
#pragma warning(push)
#pragma warning(disable:4127)
            if (sizeof(T) > 4)
            {
                throw runtime_error("readBigEndian only supports 4 bytes max");
            }
#pragma warning(pop)
            uint8_t data[4];
            file.read((char*)&data, sizeof(T));
 
            static const uint8_t SwapTest[2] = { 1, 0 };
            if( *(const uint16_t *)SwapTest != 1 )
            {
                // big endian

                switch (sizeof (T))
                {
                case 1:
                    break;
                case 2:
                    swap(data[0], data[1]);
                    break;
                case 4:
                    swap(data[0], data[3]);
                    swap(data[1], data[2]);
                    break;
                default:
                    throw runtime_error("readBigEndian invalid sizeof(T)");
                }
            }
            *value = *(T *)&data[0];
        }

    };

//////////////////////////////////////////////////////////////////////////////

class Data_t :
    public Solver::Data_t
{
    typedef ResultsMap_t<uint16_t, uint32_t> TwoCardResultsMap_t;
    typedef ResultsMap_t<uint32_t, uint16_t> ThreeCardResultsMap_t;
    typedef ResultsMap_t<uint32_t, uint8_t>  FourCardResultsMap_t;

private:

    const PaySchedule::Lookup::Data_t& _schedule;
    const size_t    _paylineCount;
    const size_t    _deckSize;
    const size_t    _cardsPerHand;

public:

    vector<uint32_t>  _handResults;
    vector<uint32_t>  _oneCardResults;
    vector<uint8_t>   _fiveCardResults;

    TwoCardResultsMap_t     _twoCardResultsMap;
    ThreeCardResultsMap_t   _threeCardResultsMap;
    FourCardResultsMap_t    _fourCardResultsMap;
    
    Solver::Cuda::AllResults_t      _allResults;

public:

//    Data_t();
    // todo: only requires a subset of payschedule; the # and type of hands, not the payouts
    // full payout can be passed to generate
    Data_t(
        const PaySchedule::Lookup::Data_t& schedule,
              size_t                       deckSize,
              size_t                       cardsPerHand);

    ~Data_t();

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
              size_t     iterCount);

    double
    getManyPayouts(
        const Card_t* cards,
              size_t  handCount,
              size_t* iterCounts,
              bool shared);

    virtual const Results_t&
    getResults(
        size_t index) const
    {
        return _allResults.results[index];   
    }
    
    virtual const Results_t&
    getBestResults() const 
    {
        return getResults(getBestHoldCardBits());
    }
    
    virtual uint32_t
    getBestHoldCardBits() const
    {
        return _allResults.bestResultsIndex;
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

    const PaySchedule::Lookup::Data_t& getPaySchedule() { return _schedule; }

    size_t getCardsPerHand() const { return _cardsPerHand; }
    size_t getDeckSize() const { return _deckSize; }

private:

    template<class K, class N>
    void
    getResults(
              size_t*             results,
        const ResultsMap_t<K, N>& map,
        const Card_t*             cards,
              size_t              cardCount) const;

    void
    showPayouts(
        const Hand_t& hand,
              size_t  iterCount);

    void
    showPayoutsForCombinations(
        const CardVector_t& cards,
              size_t        cardCount);

    void
    showPayoutsForCards(
        const CardVector_t& cards,
              size_t        iterCount);

    void
    checkCards(
        const CardVector_t& cards,
              size_t        key);

    const size_t*
    getResultsForCards(
        const Card_t*   cards,
              size_t    cardCount,
              size_t    iterCount) const;

    size_t
    getCardsForBits(
        const Hand_t& hand,
              Card_t* cards,
              size_t  bits) const;

    void
    Init();

    void
    writeTag(
        ofstream& file, size_t section, size_t version = 0) const;

    void
    readTag(
        filebuffer& file, size_t section, size_t* version = NULL) ;

private:

    Data_t();
    Data_t(const Data_t&);
    const Data_t& operator=(const Data_t&);
};

} // Cuda
} // Lookup
} // Solver
} // Hand

#endif // 
