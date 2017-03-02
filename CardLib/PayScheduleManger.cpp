////////////////////////////////////////////////////////////////////////////////
//
// PayScheduleManager.cpp
//
// Copyright 2010-2011 Mike McQueen. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Poker.h"
#include "PaySchedule.hpp"
//#include "PayScheduleManager.hpp"
#include "GameRegister.hpp"

using namespace std;

namespace 
PaySchedule
{

Game::Map_t* Manager_t::s_gameMap = NULL;

/* static */
void
Manager_t::
registerPaySchedule(
    PaySchedule::Game::Id_t gameId,
    const Lookup::Data_t*   paySchedule)
{
    using namespace PaySchedule::Game;

//    printf("registered paySchedule (%d) (%s:%4.2f)\n", gameId, L"x", paySchedule->getReturn());

    if (s_gameMap == NULL)
    {
//        puts("  new game map");
        s_gameMap = new PaySchedule::Game::Map_t();
    }

    Map_t::iterator it = s_gameMap->find(gameId);
    if (it == s_gameMap->end())
    {
//        printf("new game entry (%d)\n", gameId);
        pair<Map_t::iterator, bool> ibPair =
            s_gameMap->insert(make_pair(gameId, Data_t()));

        it = ibPair.first;
    }
    
    it->second._scheduleSet.insert(paySchedule);
}

/* static*/
uint32_t
Manager_t::
getGameIndex(
    PaySchedule::Game::Id_t gameId)
{
    using namespace PaySchedule::Game;

    uint32_t index = 0;
    for (Map_t::const_iterator it = getMap().begin(); it != getMap().end(); ++it, ++index)
    {
        if (it->first == gameId)
        {
            return index;
        }
    }
    throw invalid_argument("getGameIndex(): bad gameId");
}

/*static*/
PaySchedule::Game::Id_t
Manager_t::
getGameId(
    uint32_t index)
{
    if (index >= getMap().size())
    {
        throw invalid_argument("getGameId(): bad index");
    }

    using namespace PaySchedule::Game;
    Map_t::const_iterator it = getMap().begin();
    for (; index-- != 0; ++it)
    {
        // do nothing
    }
    return it->first;
}

/*static*/
const Lookup::Data_t& 
Manager_t::
getPaySchedule(
    PaySchedule::Game::Id_t gameId, 
    double payout)
{
//    using namespace PaySchedule::Game;
    const Lookup::Data_t::Set_t& set = getMap().find(gameId)->second._scheduleSet;
    Lookup::Data_t::Set_t::const_iterator it;
    for (it = set.begin(); it != set.end(); ++it)
    {
        if ((*it)->getReturn() == payout)
        {
            return **it;
        }
    }
    throw invalid_argument("getPaySchedule() bad payout");
}

/*static*/
const Lookup::Data_t& 
Manager_t::
getPaySchedule(
    PaySchedule::Game::Id_t gameId, 
    int index)
{
    const Lookup::Data_t::Set_t& set = getMap().find(gameId)->second._scheduleSet;
    Lookup::Data_t::Set_t::const_iterator it = set.begin();
    for (int i = 0; i < index; ++i, ++it)
    {
    }

    return **it;
}

/*static*/
const Lookup::Data_t& 
Manager_t::
getPaySchedule(
    int index)
{
    using namespace PaySchedule::Game;

    const Map_t& map = getMap();

    Map_t::const_iterator itMap = map.begin();
    for (; map.end() != itMap; ++itMap)
    {
        const Lookup::Data_t::Set_t& set = itMap->second._scheduleSet;
        Lookup::Data_t::Set_t::const_iterator itSet = set.begin();
        for (; set.end() != itSet; ++itSet, --index)
        {
            const Lookup::Data_t& ps = **itSet;
            if (index == 0)
            {
                return ps;
            }
        }
    }
    throw invalid_argument("getPaySchedule(): bad index");
}

/*static*/
void
Manager_t::
listAll()
{
    using namespace PaySchedule::Game;

    const Map_t& map = PaySchedule::Manager_t::getMap();

    uint32_t index = 0;
    Map_t::const_iterator itMap = map.begin();
    for (; map.end() != itMap; ++itMap)
    {
        const Game::Id_t gameId = itMap->first;
        string gameName(getGameName(gameId));
        const Set_t& set = itMap->second._scheduleSet;
        Set_t::const_iterator itSet = set.begin();
        for (; set.end() != itSet; ++itSet)
        {
            const Lookup::Data_t& ps = **itSet;
            printf("%2d: %s %.2f\n", index++, gameName.c_str(), ps.getReturn());
        }
    }
}

/*static*/
void
Manager_t::
registerAllPaySchedules()
{
    PaySchedule::AllAmerican::registerPaySchedules();
//    PaySchedule::BonusDeuces::registerPaySchedules();
    PaySchedule::BonusPoker::registerPaySchedules();
    PaySchedule::BonusPokerDeluxe::registerPaySchedules();
    PaySchedule::DeucesWild::registerPaySchedules();
    PaySchedule::DoubleBonus::registerPaySchedules();
    PaySchedule::DoubleDblBonus::registerPaySchedules();
    PaySchedule::DoubleDeuces::registerPaySchedules();
    PaySchedule::JacksOrBetter::registerPaySchedules();
    PaySchedule::JokersWild::registerPaySchedules();
    PaySchedule::LooseDeuces::registerPaySchedules();
    PaySchedule::SuperAces::registerPaySchedules();
    PaySchedule::TripleDblBonus::registerPaySchedules();
    PaySchedule::WhiteHotAces::registerPaySchedules();
}

} // namespace PaySchedule
