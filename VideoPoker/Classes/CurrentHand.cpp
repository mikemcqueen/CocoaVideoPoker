/*
 *  CurrentHand.cpp
 *  VideoPoker
 *
 *  Created by Mike McQueen on 7/17/10.
 *  Copyright 2010 Amazon.com. All rights reserved.
 *
 */

#include "Platform.h"
#include "CurrentHand.hpp"

CurrentHand::CurrentHand()
{
}

void
CurrentHand::
setCard(
    int                   index,
    Card_t                card)
{
    _cards[index] = card;
}

void
CurrentHand::
setCard(
    int                   index,
    Card_t                card,
    Hand::Solver::Data_t& solverData)
{
    setCard(index, card);
    solve(solverData);
}

void
CurrentHand::
setCards(
    const CardVector_t&   cards,
    Hand::Solver::Data_t& solverData)
{
    _cards = cards;
    solve(solverData);
}
    
void
CurrentHand::
solve(
    Hand::Solver::Data_t& solverData)
{
    CardVector_t sortedCards(_cards);
    std::sort(sortedCards.begin(), sortedCards.end(), greater<Card_t>());
    
    solverData.getPayouts(sortedCards, 0);
    
    _bestCards.clear();
    uint32_t bestCardBits = solverData.getBestHoldCardBits();
    for (int index = 0; index < 5; ++index)
    {
        if (0 != (bestCardBits & (1 << index)))
        {
            _bestCards.push_back(sortedCards[index]);
        }
    }
}

Card_t
CurrentHand::
getCard(int index) const
{
    return _cards[index];
}

const CardVector_t&
CurrentHand::
getBestCards() const
{
        //*cards = hand._bestCards;
    return _bestCards;
}

bool
CurrentHand::
isBestCard(int index) const
{
    return _bestCards.end() != find(_bestCards.begin(), _bestCards.end(), getCard(index));
}

void
CurrentHand::
setHeldCards(const CardVector_t& cards)
{
/*
 if (nil == cards)
    {
        _heldCards.clear();
    }
    else
*/
    {
        _heldCards = cards;
    }
}

void 
CurrentHand::
clearHeldCards()
{
    _heldCards.clear();
}

const CardVector_t&
CurrentHand::
getHeldCards() const
{
    return _heldCards;
}

bool
CurrentHand::
isHeldCard(Card_t card) const
{
    return _heldCards.end() != find(_heldCards.begin(), _heldCards.end(), card);
}

void
CurrentHand::
toggleHeldCard(Card_t card)
{
    if (isHeldCard(card))
    {
        _heldCards.erase(remove_if(_heldCards.begin(), _heldCards.end(), bind2nd(equal_to<Card_t>(), card)), _heldCards.end());
    }
    else
    {
        _heldCards.push_back(card);
    }
}

uint32_t
CurrentHand::
getCardBits(
    const CardVector_t& cards) const
{
    if (cards.size() == 0)
    {
        return 0;
    }
    
    uint32_t bits = 0;
    CardVector_t sortedCards(_cards);
    std::sort(sortedCards.begin(), sortedCards.end(), greater<Card_t>());
    
    for (int index = 0; index < cards.size(); ++index)
    {
        CardVector_t::const_iterator it = find(sortedCards.begin(), sortedCards.end(), cards[index]);
        if (sortedCards.end() == it)
        {
            throw runtime_error("held card not in hand");
        }
        bits |= (1 << (it - sortedCards.begin())); 
    }
    
    return bits;
}

bool
CurrentHand::
isValid() const
{
    if (!_cards.empty())
    {
        for (CardVector_t::const_iterator it = _cards.begin(); _cards.end() != it; ++it)
        {
            if (1 != count(_cards.begin(), _cards.end(), *it))
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

/*
 void
calcValueAndPayout(
    const PaySchedule_t& schedule, uint32_t bet)
{
    Hand::Value_t value = Hand::getValue(cards());
    if (value != _value)
    {
        _valueChanged = true;
    }
    _value = value;
    _payout = schedule.getHandPayout(_value, bet);
 }
*/
