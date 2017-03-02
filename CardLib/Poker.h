#ifndef Include_POKER_H
#define Include_POKER_H

namespace Poker
{
	namespace Card
	{
		static const size_t Joker = 15;
		static const size_t Ace = 14;
		static const size_t King = 13;
		static const size_t Queen = 12;
		static const size_t Jack = 11;
		static const size_t AceLow = 1;
		static const size_t Deuce = 2;
	}
}

#include "Hand.hpp"
#include "Deck.hpp"
#include "Card.hpp"
	
#endif // Include_POKER_H