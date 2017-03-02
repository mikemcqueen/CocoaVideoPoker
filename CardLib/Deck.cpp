//
// Deck.cpp
//
// Copyright (C) 2009 Mike McQueen. All rights reserved.
//

#include "stdafx.h"
#include "Deck.hpp"
#include "Card.hpp"
#include "Poker.h"
#include "Combinations.h"

using namespace Poker::Card;

////////////////////////////////////////////////////////////////////////////////

Deck_t::
Deck_t(size_t deckSize) :
    _topCard(0)
{
    Init(deckSize);
}

////////////////////////////////////////////////////////////////////////////////

#if 0
Deck_t::
Deck_t(
    const CardVector_t& cards,
          bool          subtractFromFullDeck /*= false*/)
:
    _topCard(0)
{
    if (subtractFromFullDeck)
    {
        Init(52, &cards);
    }
    else
    {
        assign(cards.begin(), cards.end());
    }
}
#endif

////////////////////////////////////////////////////////////////////////////////

void
Deck_t::
Init(
          size_t        cardCount,
    const CardVector_t* subtractCards)
{
    int64_t cardBits = 0LL;

    size_t deckSize = cardCount;
    if (NULL != subtractCards)
    {
        for (size_t index = 0; index < subtractCards->size(); ++index)
        {
            cardBits |= 1LL << (*subtractCards)[index].getValue();
        }
        deckSize -= subtractCards->size();
    }

    _shims.resize(deckSize);
    clear();
    reserve(deckSize);
    int64_t bit = 1LL;
    for (size_t index = 0; index < cardCount; ++index, bit <<= 1)
    {
        if (0 == (cardBits & bit))
        {
            push_back(Card_t(index));
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

Deck_t::Shim_t* g_shims;

int CompareCards(const void* s1, const void* s2)
{
    Card_t card1 = *(const Card_t*)s1;
    Card_t card2 = *(const Card_t*)s2;
    return g_shims[card1.getValue()].sortValue - g_shims[card2.getValue()].sortValue;
}

int CompareShims(const void* s1, const void* s2)
{
    const Deck_t::Shim_t& shim1 = *(const Deck_t::Shim_t*)s1;
    const Deck_t::Shim_t& shim2 = *(const Deck_t::Shim_t*)s2;
    return shim1.sortValue < shim2.sortValue;
}

////////////////////////////////////////////////////////////////////////////////

void
Deck_t::
Shuffle()
{
    struct SortByValue
    {
        const ShimVector_t& shims;

        SortByValue(const ShimVector_t& s) : shims(s) {}

        __inline bool operator()(const Card_t& card1, const Card_t& card2)
        {
            return shims[card1.getValue()].sortValue < shims[card2.getValue()].sortValue;
        }
    };

    for (size_t i = 0; i < _shims.size(); ++i)
    {
        _shims[i].index = i;
        _shims[i].sortValue = Random() % 1000000;
    }
    g_shims = &_shims[0];
    qsort(&(*this)[0], size(), sizeof(Card_t), CompareCards);
//    std::sort(_cards.begin(), _cards.end(), SortByValue(_shims));
    _topCard = 0;
}

////////////////////////////////////////////////////////////////////////////////

Card_t
Deck_t::
Deal()
{
    return at(_topCard++);
}

size_t Deck_t::Random() 
{
#ifdef WIN32
    return rand();
#else
    return random();
#endif
}

////////////////////////////////////////////////////////////////////////////////

#if 0
Deck_t::IndexMap_t indexMap2;
Deck_t::IndexMap_t indexMap3;
Deck_t::IndexMap_t indexMap4;
#endif

/*static*/
size_t
Deck_t::
getHandIndex(
    const Card_t* cards,
          size_t  cardCount,
          size_t  deckSize)
{
/*
    if (cardCount == 2)
    {
        return getHandIndex2(indexMap2, cards, cardCount, deckSize);
    }
    if (cardCount == 3)
    {
        return getHandIndex2(indexMap3, cards, cardCount, deckSize);
    }
    if (cardCount == 4)
    {
        return getHandIndex2(indexMap4, cards, cardCount, deckSize);
    }
*/
    return calculateHandIndex(cards, cardCount, deckSize);
}

#if 0
/*static*/
size_t
Deck_t::
getHandIndex2(
    IndexMap_t&   indexMap,
    const Card_t* cards,
          size_t  cardCount,
          size_t  deckSize)
{
    size_t index = 0;
    size_t key = Card_t::getUniqueValue(cards, cardCount);
    IndexMap_t::const_iterator it = indexMap.find(key);

    if (indexMap.end() != it)
    {
        index = it->second;
    }
    else
    {
        index = calculateHandIndex(cards, cardCount, deckSize);
        IndexMap_t::_Pairib ibPair = indexMap.insert(make_pair(key, index));
        if (!ibPair.second)
        {
            throw runtime_error("indexMap2.insert failed");
        }
    }

    return index;
}
#endif

/*static*/
size_t
Deck_t::
calculateHandIndex(
    const Card_t* cards,
          size_t  cardCount,
          size_t  deckSize)
{
    static Combinations::Data_t data[53];
    static bool test = false;
    if (!test) { Combinations::InitData(data, deckSize); test = true; }

    size_t handIndex = 0;
    size_t highestCardForIndex = deckSize - 1;
    for (size_t cardIndex = 0; cardIndex < cardCount; ++cardIndex)
    {
        size_t remainingCardsInHand = cardCount - cardIndex - 1;
        if (remainingCardsInHand > 0)
        {
            Card_t::Value_t highCardValue = (Card_t::Value_t)highestCardForIndex;
            for (; highCardValue > cards[cardIndex].getValue(); --highCardValue)
            {
                if (remainingCardsInHand > 1)
                {
                    size_t C = highCardValue;
                    size_t K = remainingCardsInHand;

                    size_t combos = Combinations::Calculate(C, K, &data[0]);
                    handIndex += combos;
                }
                else
                {
                    handIndex += highCardValue;
                }
            }

        }
        else
        {
            //printf("before last card handIndex(%ld) highestCardForIndex(%ld)\n", handIndex, highestCardForIndex);
            handIndex += highestCardForIndex - cards[cardIndex].getValue();
        }
        //printf ("cardIndex(%d) handIndex(%d)\n", cardIndex, handIndex);
        highestCardForIndex = cards[cardIndex].getValue() - 1;
    }

    return handIndex;
}
