/*
 *  CurrentHand.hpp
 *  VideoPoker
 *
 *  Created by Mike McQueen on 7/16/10.
 *  Copyright 2010 Amazon.com. All rights reserved.
 *
 */

#ifndef Include_CURRENTHAND_HPP
#define Include_CURRENTHAND_HPP

#include "Card.hpp"
#include "SolverData_t.hpp"
#include "Hand.hpp"

class CurrentHand
{
public:
    
    CardVector_t    _cards;

private:
/*
 uint32_t        _payout;
    Hand::Value_t   _value;
    bool            _valueChanged;
 */   
    CardVector_t _bestCards;
    CardVector_t _heldCards;
    
public:
    
    CurrentHand();
        //    CurrentHand(Hand::Solver::Data_t& solverData);
        
    void
    setCard(
        int     index,
        Card_t  card);
    
    void
    setCard(int index, Card_t card, Hand::Solver::Data_t& solverData);

    void
    setCards(const CardVector_t& cards, Hand::Solver::Data_t& solverData);
    
    Card_t
    getCard(int index) const;
    
    const CardVector_t&
    getBestCards() const;
    
    bool
    isBestCard(int index) const;
    
    void
    setHeldCards(const CardVector_t& cards);
    
    void 
    clearHeldCards();
    
    const CardVector_t&
    getHeldCards() const;
    
    bool
    isHeldCard(Card_t card) const;
    
    uint32_t
    getCardBits(const CardVector_t& cards) const;
    
    void
    toggleHeldCard(Card_t card);
    
    uint32_t
    indexOf(Card_t card) const
    {
        CardVector_t::const_iterator it = find(_cards.begin(), _cards.end(), card);
        if (_cards.end() == it)
        {
            throw invalid_argument("indexOf() invalid card");
        }
        return (uint32_t) (it - _cards.begin());
    }

    void
    reset()
    {
        _cards.resize(5); 
        fill(_cards.begin(), _cards.end(), Card_t(Card_t::Value::Undefined));
        clearHeldCards();
    }
    
    bool
    isValid() const;
    
    const CardVector_t&    cards() const { return _cards; }
    

    /*
     void
    calcValueAndPayout(
        const PaySchedule_t& schedule, uint32_t bet);
    
    Hand::Value_t
    value() const {
        return _value;
    }

    bool
    valueChanged() const {
        return _valueChanged;
    }

    uint32_t
    payout() const {
        return _payout;
    }
     */
   
private:
    void
    init(const CardVector_t& hand, Hand::Solver::Data_t& solverData);

    void
    solve(
          Hand::Solver::Data_t& solverData);
    
};

#endif // Include_CURRENTHAND_HPP