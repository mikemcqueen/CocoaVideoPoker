
#ifdef WIN32
#pragma once
#endif

#include "Hand.hpp"

namespace Hand
{

//////////////////////////////////////////////////////////////////////////////
	
class Analyzer_t
{
	static const size_t MaxValues = 16;
	static const size_t MaxKickers = CardsPerHand;
	
    const Hand_t&   _cardVector;
	
	int				m_rgiValues[MaxValues];
    int				m_rgiSuits[Suit::Count];
	size_t			m_rgdwKickers[MaxKickers];
	int				m_cKickers;
    const Card_t*	_heldCards[CardsPerHand];
	size_t          _heldCardCount;

    Card_t::Value_t _wildCardNumber;
    int             _wildCardCount;

    size_t          _mostPopularNumber;
	
public:

	Analyzer_t(
        const Hand_t& cardVector,
        Card_t::Value_t wildCard = 0);

	~Analyzer_t();

	const Card_t&			GetHeldCard( int iCard ) const;
	void                    HoldCard(const Card_t& card);
	
	size_t					GetHeldCardCount( void ) const { return _heldCardCount; }

    Value_t					GetValue();

	static const char*      GetString( const CardVector_t& cardVector );
	static const char*      GetString( const size_t* cards, size_t cardCount);
    static const char*      GetValueString( Value_t dwHandValue );

#ifdef WIN32
    static size_t           AddCards(CardVector_t& hand, const std::string& handString);
private:
    static bool             AddCard(CardVector_t& hand, const std::string& cardString);
public:
#endif

	void					Display();
	void					DisplayValue( size_t dwHandValue );
	
	size_t					HoldCards();

	void					Invalidate( );
	bool					IsValid() const;

protected:

	void	
    InitSuitsValues(
        size_t& maxSuitCount,
        size_t& maxValueCount,
        size_t& mostPopularValue,
        size_t& uniqueNumberCount);


#if 0
	int							HoldCardsByValue( int iValue, int iHoldMax=5 );
#endif
	int							HoldCardsBySuit( int iSuit, int iValueMin=Deuce, int iValueMax=Ace );
	void						HoldAllCards( void );
	int							CountCardsBySuit( int iSuit, int iValueMin=Deuce, int iValueMax=Ace );
	int							CountUniqueValuesInRange( int *piValues, int iValueMin, int iValueMax );

	size_t						MakeKicker(bool wild = false);
	size_t						MakeHighHand( int *piValues );
    size_t                      MakeFlush(int iSuit);

    int							CheckPair(int ignoreValue = 0);
	int							CheckThreeKind();

	size_t						CheckOnePair( );
    size_t						CheckStraight(const int *piValues);
	size_t						CheckFlush( int cRequired=5 );
	size_t						CheckStraightFlush( int cRequired=5, int iGapMax=-1 );
	size_t						CheckRoyalFlush( int cRequired=5 );

	size_t						GetHighCard(  );
	size_t						GetOnePair( );
	size_t						GetTwoPair(  );
	size_t						GetThreeKind(  );
	size_t						GetStraight(  );
	size_t						GetFlush( );
	size_t						GetFullHouse(  );
	size_t						GetFourKind(  );
	size_t						GetStraightFlush(  );
	size_t						GetRoyalFlush(  );
    size_t                      GetFourDeuces();
    size_t                      GetFiveOfAKind();

    //
	size_t						GetFourOpenStraightFlush(  );
	size_t						GetFourStraightFlush(  );
	size_t						GetThreeStraightFlush( );
	size_t						GetFourFlush(  );
	size_t						GetThreeFlush(  );
	size_t						GetFourRoyalFlush(  );
	size_t						GetThreeRoyalFlush(  );
	size_t						GetTwoRoyalFlush(  );
	size_t						GetFourOpenStraight(  );
	size_t						GetFourOpenStraightKingHigh(  );
	size_t						GetFourOpenStraightThreeHighCards(  );
	size_t						GetFourStraightThreeHighCards( );
	size_t						GetFourStraight(  );
	size_t						GetJacksOrBetter(  );
	size_t						GetHighCardsJackOrBetter(  );

private:

    const Analyzer_t& operator=(const Analyzer_t&);
};

} // hand