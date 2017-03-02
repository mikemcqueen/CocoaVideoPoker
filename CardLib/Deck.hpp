////////////////////////////////////////////////////////////////////////////////
//
// Deck.hpp
//
// Copyright (C) 2009 Mike McQueen. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#if _MSC_VER >= 1000
#pragma once
#endif

#ifndef Include_DECK_HPP
#define Include_DECK_HPP

#include "Card.hpp"

////////////////////////////////////////////////////////////////////////////////

class Deck_t :
    public CardVector_t
{
private:


public:

//    typedef std::tr1::unordered_map<size_t, size_t> IndexMap_t;

    struct Shim_t
    {
        int32_t sortValue;
        size_t  index;
    };

private:

    typedef std::vector<Shim_t> ShimVector_t;

    ShimVector_t _shims;
    size_t       _topCard;

public:

    Deck_t(size_t deckSize);
    Deck_t(
        const CardVector_t& cards,
              bool          subtractFromFullDeck = false); 

    void     Shuffle();
    Card_t   Deal();

    static 
    size_t
    getHandIndex(
        const CardVector_t& cards,
              size_t        deckSize)
    {
        return getHandIndex(&cards[0], cards.size(), deckSize);
    }

    static 
    size_t
    getHandIndex(
        const Card_t* cards,
              size_t  cardCount,
              size_t  deckSize);

private:

    void
    Init(size_t cardCount, const CardVector_t* subtractCards = NULL);

/*
    static
    size_t
    getHandIndex2(
        IndexMap_t&   map,
        const Card_t* cards,
              size_t  cardCount,
              size_t  deckSize);
*/

    static
    size_t
    calculateHandIndex(
        const Card_t* cards,
              size_t  cardCount,
              size_t  deckSize);


    size_t Random();
};

////////////////////////////////////////////////////////////////////////////////

#endif // Include_DECK_HPP 

