//////////////////////////////////////////////////////////////////////////////
//
// HandIterator_t.hpp
// 
//////////////////////////////////////////////////////////////////////////////

#if _MSC_VER >= 1000
#pragma once
#endif

#ifndef Include_HANDITERATOR_T_HPP
#define Include_HANDITERATOR_T_HPP

#include "Hand.hpp"
#include "HandIterator_t.hpp"
#include "IndexCombination.h"

using namespace stdcomb;

namespace Hand
{

//////////////////////////////////////////////////////////////////////////////

class Matrix_t;

class Iterator_t
{
private:

    Matrix_t*          _matrix;
//    Matrix_t::iterator _rowIter;
    size_t             _column;

    CardVector_t& _deck;
    Hand_t*       _hand;
    size_t        _indices[MaxCards];
    size_t        _drawCount;
    size_t        _counter;
    size_t        _maxCounter;

    CIdxComb      _comb;
    bool          _useComb;

public:

    // For iterating draws, some cards in supplied hand are set to DRAW
    explicit
    Iterator_t(
        Deck_t& deck,
        Hand_t& startHand);

    // For iterating all hands, or hands in a hand matrix.
    explicit
    Iterator_t(
        Deck_t&   deck,
        size_t    cardsPerHand,
        size_t    maxIterations = 0,
        Matrix_t* handMatrix = NULL);

    // For iterating all combinations of the specified length with within the specified set of cards
    explicit
    Iterator_t(
        const CardVector_t& cards,
              size_t        cardsPerHand);

    size_t
    deal(
        Hand_t& hand);

    size_t
    deal(
        Card_t* cards, 
        size_t  cardCount);

    size_t
    Draw(
        Hand_t& hand);

    bool
    Next();

void
get(CardVector_t& cards) const;

void Reset(Deck_t& deck);

const CardVector_t& getDeck() const { return _deck; }

/*bool
Next(
    CardVector_t& cards);*/

    size_t
    GetCount() const
    {
        return _counter;
    }

    const Matrix_t*
    GetMatrix() const
    {
        return _matrix;
    }

    Matrix_t*
    GetMatrix()
    {
        return _matrix;
    }

/*
     SetEv(
        const Ev_t& ev)
    {
ev;        throw std::runtime_error("not implemented");
//       TODO _rowiter type not declared 
//        _rowIter->second[_column].ev = ev;
    }
*/
    
private:

    size_t
    DealFromMatrix(
        Hand_t& hand);

    bool
    NextInMatrix();

    size_t
    GetDrawCount(
        const Hand_t& hand);

    void
    ResetIndices(
              size_t  count,
        const Hand_t* hand = NULL);

    bool
    RecursiveCycleIndex(
        size_t  indexCount,
        size_t  index); // = Hand::CardsPerHand - 1);

    bool
    IndicesValid(
        size_t lastIndex);

private:

    const Iterator_t& operator=(const Iterator_t&);
};

} // namespace Hand

#endif // Include_HANDITERATOR_T_HPP