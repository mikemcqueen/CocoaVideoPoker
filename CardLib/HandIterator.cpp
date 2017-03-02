////////////////////////////////////////////////////////////////////////////////
//
// HandIterator.cpp
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HandIterator_t.hpp"

namespace Hand
{

////////////////////////////////////////////////////////////////////////////////
//
// For iterating draws, some cards in supplied hand are set to DRAW
//
Iterator_t::
Iterator_t(
    Deck_t& deck,
    Hand_t& startHand)
    :
    _deck(deck),
    _hand(&startHand),
    _matrix(NULL),
    _drawCount(GetDrawCount(startHand)),
    _counter(0),
    _maxCounter(0),
    _useComb(false)
{
    ResetIndices(_drawCount);
}

////////////////////////////////////////////////////////////////////////////////
//
// For iterating all hands, or all hands in a matrix.
//
Iterator_t::
Iterator_t(
    Deck_t&   deck,
    size_t    cardsPerHand,
    size_t    maxIterations,
    Matrix_t* handMatrix /* = NULL */)
    :
    _deck(deck),
    _hand(NULL),
    _matrix(handMatrix),
    _drawCount(cardsPerHand),
    _counter(0),
    _maxCounter(maxIterations),
    _useComb(false)
{
    ResetIndices(_drawCount);
    if (NULL != _matrix)
    {
#if 1
        throw runtime_error("rowIter type not declared");
#else
        _rowIter = _matrix->begin();
        _column = 0;
#endif
    }
}

////////////////////////////////////////////////////////////////////////////////

void Iterator_t::Reset(Deck_t& deck)
{
    _counter = 0;
    _deck = deck;
    ResetIndices(_drawCount);

}

////////////////////////////////////////////////////////////////////////////////

Iterator_t::
Iterator_t(
    const CardVector_t& cards,
          size_t        cardsPerHand)
    :
    _deck(const_cast<CardVector_t&>(cards)),
    _drawCount(cardsPerHand),
    _counter(0),
    _maxCounter(0),
    _matrix(NULL),
    _useComb(true)
{
    _comb.SetSizes(_deck.size(), cardsPerHand);
    for (size_t index = 0; index < cardsPerHand; ++index)
    {
        _indices[index] = index;
    }
}

////////////////////////////////////////////////////////////////////////////////

size_t
Iterator_t::
GetDrawCount(
    const Hand_t& hand)
{
    size_t drawCount = 0;
    Hand_t::const_iterator it = hand.begin();
    for (; hand.end() != it; ++it)
    {
        if (DRAW ==it->getValue())
        {
            ++drawCount;
        }
    }
    return drawCount;
}

////////////////////////////////////////////////////////////////////////////////

void
Iterator_t::
ResetIndices(
          size_t  count,
    const Hand_t* hand /*=NULL*/)
{
    size_t index = _deck.size() - 1;
    for (size_t card = 0; card < count; ++card)
    {
        _indices[card] = (NULL == hand) ? index-- : (*hand)[card].getValue();
    }
}

////////////////////////////////////////////////////////////////////////////////

size_t
Iterator_t::
deal(
    Hand_t& hand)
{
    if (hand.size() != _drawCount)
    {
        throw invalid_argument("Hand::Iterator_t::Deal()");
    }

    return deal(&hand[0], hand.size());
}

size_t
Iterator_t::
deal(
    Card_t* cards, 
    size_t  cardCount)
{
    ++_counter;

    for (size_t cardIndex = 0; cardIndex < cardCount; ++cardIndex)
    {
        cards[cardIndex] = _deck[_indices[cardIndex]];
    }

    return cardCount;
}

size_t
Iterator_t::
Draw(
    Hand_t& hand)
{
    ++_counter;

    size_t count = 0;
    for (size_t card = 0; card < _hand->size(); ++card)
    {
        if (DRAW == (*_hand)[card].getValue())
        {
            hand[card] = _deck[_indices[count++]];
        }
    }
    return count;
}

////////////////////////////////////////////////////////////////////////////////

/*
void
Iterator_t::
get(CardVector_t& cards) const
{
    for (size_t index = 0; index < _drawCount; ++index)
    {
        cards[index] = _cards[_indices[index]];
    }
}
*/

bool
Iterator_t::
Next()
{
    if (_useComb)
    {
        return _comb.GetNextComb(_indices);
    }

    bool foundNext = false;
    if ((0 == _maxCounter) || (_counter < _maxCounter))
    {
        if (NULL == _matrix)
        {
            foundNext = RecursiveCycleIndex(_drawCount, _drawCount - 1);
        }
/*
        else
        {
            foundNext = NextInMatrix();
        }
*/
    }
    return foundNext;
}

////////////////////////////////////////////////////////////////////////////////

/*bool
Iterator_t::
Next(
    CardVector_t& cards)
{
}*/

////////////////////////////////////////////////////////////////////////////////

bool
Iterator_t::
NextInMatrix()
{
#if 1
    throw std::runtime_error("_rowIter type not declared");
#else
    bool foundNext = false;
    do
    {
        for (++_column; !foundNext && _column < _matrix->GetColumnCount();)
        {
            if (0 < _rowIter->second[_column].count)
            {
                foundNext = true;
            }
            else
            {
                ++_column;
            }
        }
        if (!foundNext)
        {
            ++_rowIter;
            _column = 0;
        }
    }
    while (!foundNext && (_matrix->end() != _rowIter));
    return foundNext;
#endif
}

////////////////////////////////////////////////////////////////////////////////

bool
Iterator_t::
RecursiveCycleIndex(
    size_t  indexCount,
    size_t  index)
{
    do
    {
        if (index > 0)
        {
            if (_indices[index] > indexCount - index - 1)
            {
                --_indices[index];
            }
            else
            {
                _indices[index] = _deck.size() - 1;
                if (RecursiveCycleIndex(indexCount, index - 1) && (_deck.size() > index))
                {
                    _indices[index] = _indices[index - 1] - 1;
                }
                else
                {
                    return false;
                }
            }
        }
        else
        {
            _deck.pop_back();
            if (_deck.empty())
            {
                return false;
            }
            else
            {
                _indices[index] = _deck.size() - 1;
                return true;
            }
        }
    }
    while (!IndicesValid(index));
    return true;
}

////////////////////////////////////////////////////////////////////////////////

bool
Iterator_t::
IndicesValid(
    size_t lastIndex)
{
    int64_t values = 0LL;
    size_t index = 0;
    for (; index <= lastIndex; ++index)
    {
        int64_t bit = (1LL << _indices[index]);
        if (1 == (values & bit))
        {
            break;
        }
        values |= bit;
    }
    return index > lastIndex;
}

////////////////////////////////////////////////////////////////////////////////

} // namespace Hand

////////////////////////////////////////////////////////////////////////////////
