////////////////////////////////////////////////////////////////////////////////
//
// PayScheduleManager.hpp
//
// Copyright 2010-2011 Mike McQueen. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#if _MSC_VER >= 1000
#pragma once
#endif

#ifndef Include_PAYSCHEDULEMANAGER_HPP
#define Include_PAYSCHEDULEMANAGER_HPP

#include "PayScheduleTypes.hpp"

namespace PaySchedule
{
    class Manager_t //: public Singleton_t<Manager_t>
    {
        typedef Lookup::Data_t::Set_t Set_t;

        static const int DefaultDeckSize = 52;

    public: //hack

        static PaySchedule::Game::Map_t* s_gameMap;

    public:

        static const
        Game::Map_t&
        getMap()
        {
            return *s_gameMap;
        }

        static void
        registerGame(
            PaySchedule::Game::Id_t gameId,
            const string& name,
            const string& fileName,
                  size_t deckSize = DefaultDeckSize)
        {
            using namespace PaySchedule::Game;

//            printf("registering Game %s...", name.c_str());
            if (s_gameMap == NULL)
            {
                s_gameMap = new PaySchedule::Game::Map_t();
            }

            pair<Map_t::iterator, bool> ibPair =
                s_gameMap->insert(make_pair(gameId, Data_t(name, fileName, deckSize)));

            if (!ibPair.second)
            {
                Data_t& data = ibPair.first->second;
                if (data._name.empty())
                {
//                    printf("updating game name: %s\n", name.c_str());
                    data._name = name;
                    data._filename = fileName;
                    data._deckSize = deckSize;
                }
            }
            else
            {
//                puts("Done");
            }
        }

        static const
        string&
        getGameName(
            PaySchedule::Game::Id_t gameId)
        {
            return getGameData(gameId)._name;
        }

        static const
        string&
        getGameFilename(
            PaySchedule::Game::Id_t gameId)
        {
            return getGameData(gameId)._filename;
        }

        static const
        Game::Data_t& 
        getGameData(
            PaySchedule::Game::Id_t gameId)
        {
            Game::Map_t::const_iterator it = s_gameMap->find(gameId);
            if (s_gameMap->end() == it)
            {
                printf("getGameData(%d)\n", gameId);
                throw invalid_argument("getGameData(): bad gameId");
            }
            return it->second;
        }

        static
        PaySchedule::Game::Id_t
        getGameId(
            uint32_t index);

        static
        uint32_t
        getGameIndex(PaySchedule::Game::Id_t gameId);

        static const
        Lookup::Data_t&
        getPaySchedule(PaySchedule::Game::Id_t gameId, double payout);

        static const
        Lookup::Data_t&
        getPaySchedule(PaySchedule::Game::Id_t gameId, int index);

        static const
        Lookup::Data_t&
        getPaySchedule(int index);

        static void
        registerPaySchedule(
            PaySchedule::Game::Id_t gameId,
            const Lookup::Data_t*   paySchedule);

        static void
        registerAllPaySchedules();

        static void
        listAll();
    };

} // namespace PaySchedule

#define REGISTER_PAY_SCHEDULE(group, cls, estimatedReturn) \
    static const PaySchedule::Lookup::Data_t* paySchedule = \
        new PaySchedule::Lookup::Data_t(group, cls, _countof(cls), 5, estimatedReturn);

#define REGISTER_PAY_SCHEDULE2(group, base, cls, estimatedReturn, wildCard) \
    static const PaySchedule::Lookup::Data_t* paySchedule = \
        new PaySchedule::Lookup::Data_t(group, base, _countof(base), cls, _countof(cls), 5, estimatedReturn, wildCard);

#endif // Include_PAYSCHEDULEMANAGER_HPP
