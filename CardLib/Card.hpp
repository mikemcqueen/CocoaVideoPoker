////////////////////////////////////////////////////////////////////////////////
//
// Card.hpp
//
// Copyright (C) 2009 Mike McQueen. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#if _MSC_VER >= 1000
#pragma once
#endif

#ifndef Include_CARD_HPP
#define Include_CARD_HPP

class Card_t;

#define DRAW 0xff

const char* GetValueString( int iValue );
const char* GetSuitString( int iSuit );


////////////////////////////////////////////////////////////////////////////////

namespace Suit
{
	enum Type
	{
		Clubs,
		Diamonds,
		Hearts,
		Spades,
	};
    static const unsigned Count = 4;
}
typedef Suit::Type Suit_t;

////////////////////////////////////////////////////////////////////////////////

/*
 namespace Card
{
    typedef unsigned char Value_t;
}
*/

////////////////////////////////////////////////////////////////////////////////

class CardVector_t;

class Card_t
{
public:

    typedef unsigned char Value_t;
    
    struct Value
    {
        static const Card_t::Value_t Undefined = 99;
    };

private:

    Value_t _value;

public:

    Card_t() : _value(0) {}
    Card_t(Value_t value) : _value(value) {}
    Card_t(size_t value) : _value(Value_t(value)) {}

    static Value_t
    makeValue(unsigned number, Suit_t suit)
    {
        return Value_t((number - 2) * Suit::Count + suit);
    }
    
    size_t
    getNumber() const
	{
        // value 0-3 are deuces, number 2 is a deuce
        return _value / Suit::Count + 2;
	}

    void
    setNumber(
        unsigned number)
    {
        setValue(makeValue(number, getSuit()));
    }

	Suit_t 
	getSuit() const
	{
        return Suit_t(_value % Suit::Count);
	}

	void 
	setSuit(Suit_t suit)
	{
        setValue(makeValue((unsigned)getNumber(), suit));
	}

    Value_t
    getValue() const
    {
        return _value;
    }

    void
    setValue(
        Value_t value)
    {
        _value = value;
    }
    
    static size_t
    getUniqueValue(
        const CardVector_t& cards);
    
    static size_t
    getUniqueValue(
        const Card_t* cards,
              size_t  cardCount);
            
};

////////////////////////////////////////////////////////////////////////////////

    const char* GetShortDisplayString(const Card_t card);
    const char* GetShortDisplayString(size_t value);

////////////////////////////////////////////////////////////////////////////////

class CardVector_t :
    public std::vector<Card_t>
{
    typedef std::vector<Card_t> Base_t;

public:

    CardVector_t() {}
    CardVector_t(size_t size) : Base_t(size) {}

    struct SortByNumber
    {
        bool operator()(const Card_t c1, const Card_t c2)
        {
            return c1.getNumber() < c2.getNumber();
        }
    };

    size_t
    getUniqueValue() const
    { 
        return Card_t::getUniqueValue(&at(0), size());
    }

};

////////////////////////////////////////////////////////////////////////////////

bool operator<(const Card_t c1, const Card_t c2);
bool operator>(const Card_t c1, const Card_t c2);
bool operator==(const Card_t card, size_t value);
bool operator==(const Card_t c1, const Card_t c2);

////////////////////////////////////////////////////////////////////////////////

#endif // Include_CARD_HPP

////////////////////////////////////////////////////////////////////////////////
