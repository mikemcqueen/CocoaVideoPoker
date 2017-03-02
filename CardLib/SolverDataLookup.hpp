//////////////////////////////////////////////////////////////////////////////
//
// SOLVERDATALOOKUP.HPP
// 
//////////////////////////////////////////////////////////////////////////////

#if _MSC_VER >= 1000
#pragma once
#endif

#ifndef Include_SOLVERDATALOOKUP_HPP
#define Include_SOLVERDATALOOKUP_HPP

#include "Poker.h"
#include "Card.hpp"
#include "Deck.hpp"
#include "Hand.hpp"
#include "HandIterator_t.hpp"
#include "SolverData_t.hpp"
#include "Combinations.h"
#include "Log_t.h"
#include "FileBuffer.h"

//class PaySchedule_t;

namespace Hand
{

//////////////////////////////////////////////////////////////////////////////

namespace Solver
{
namespace Lookup
{

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
        typedef vector<uint8_t>  Data_t;

   public:

        Map_t        _map;
        Data_t       _data;

        const size_t _paylineCount;
        const size_t _paylineByteCount;

    public:

        ResultsMap_t(
            size_t /*cardCount*/,
            size_t paylineCount,
            size_t paylineByteCount)
            :
            _paylineCount(paylineCount),
            _paylineByteCount(paylineByteCount)
        {
        }

        const uint8_t*
        get(
           const Card_t* cards,
                 size_t  cardCount,
                 size_t& handIndex,
                 size_t& dataIndex,
                 size_t  deckSize) const
        {
            handIndex = Deck_t::getHandIndex(cards, cardCount, deckSize);
            return get(handIndex, dataIndex);
        }

        const uint8_t*
        get(
            const size_t  handIndex,
                  size_t& dataIndex) const
        {
            dataIndex = _map[handIndex];
            return &_data[dataIndex];
        }

        size_t
        size() const
        {
            return _map.size();
        }

        size_t
        sizeInBytes() const
        {
            return _data.size() * sizeof(_data[0]);
        }

        void
        show() const
        {
            printf("_map.size(%ld)\n_data.size(%ld)\n", _map.size(), _data.size());
        }

        void
        show(
            Key_t key) const
        {
            key;
            throw std::runtime_error("Solver::Data::Lookup::ResultsMap_t::show() not implemented");
        }

        size_t
        write(
            ofstream& /*file*/) const
        {
            throw std::runtime_error("Solver::Data::Lookup::ResultsMap_t::write() not implemented");
        }

        bool
        logCheck(size_t /*keyIndex*/) const
        {
            return (logLevel == Ludicrous);// && (keyIndex >= 660) && (keyIndex <= 663);
        }

        size_t
        read(
            filebuffer& file,
            size_t      /*cardCount*/)
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
                byteCount += sizeof(key) + sizeof(paylineCount);

                size_t location = _data.size();
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
                    _data.insert(_data.end(), (uint8_t*)&paylineData, (uint8_t*)(&paylineData + 1));
                    byteCount += sizeof(paylineData);
                }

                // insert map entry
                size_t slot = keyCount - keyIndex - 1;
                _map[slot] = location;

                // verify
                size_t index = _map[slot];
                uint8_t* data = &_data[index];
                if (*data != paylineCount)
                {
                    printf("*data(%d)\n", *data);
                    throw runtime_error("Lookup::Data_t::read(): *data != paylineCount");
                }
            }
            return byteCount;
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

private:

    typedef ResultsMap_t<uint16_t, uint32_t> TwoCardResultsMap_t;
    typedef ResultsMap_t<uint32_t, uint16_t> ThreeCardResultsMap_t;
    typedef ResultsMap_t<uint32_t, uint8_t>  FourCardResultsMap_t;

    const PaySchedule::Lookup::Data_t& _schedule;

    const size_t            _paylineCount;
    const size_t            _deckSize;
    const size_t            _cardsPerHand;

    vector<uint32_t>        _handResults;
    vector<uint32_t>        _oneCardResults;
    TwoCardResultsMap_t     _twoCardResultsMap;
    ThreeCardResultsMap_t   _threeCardResultsMap;
    FourCardResultsMap_t    _fourCardResultsMap;
    vector<uint8_t>         _fiveCardResults;

    Default::AllResults_t   _allResults;

public:

    // TODO: only requires a subset of payschedule; the # and type of hands, not the payouts
    // full payout can be passed to generate
    Data_t(
        const PaySchedule::Lookup::Data_t& schedule,
              size_t         deckSize,
              size_t         cardsPerHand);

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

    virtual const Default::Results_t&
    getResults(
        size_t index) const
    {
        return _allResults.results[index];   
    }
    
    virtual const Default::Results_t&
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

private:

    template<class K, class N>
    void
    getResults(
              uint32_t*           results,
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

    const uint32_t*
    getResultsForCards(
        const Card_t*   cards,
              size_t    cardCount,
              size_t    iterCount) const;

    uint32_t
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

} // Lookup
} // Solver
} // Hand

#endif // 
