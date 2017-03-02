////////////////////////////////////////////////////////////////////////////////
//
// EvCount.h
//
////////////////////////////////////////////////////////////////////////////////

#if _MSC_VER >= 1000
#pragma once
#endif

#ifndef Include_EVCOUNT_H_
#define Include_EVCOUNT_H_

typedef float Ev_t;
static const float EV_MULTIPLIER = 1000.0f;

struct EvCount_t
{
    Ev_t   ev;
    size_t count;

    EvCount_t() : ev(0.0), count(0) {}
    EvCount_t(Ev_t e, size_t c) : ev(e), count(c) { }
};

typedef std::tr1::unordered_map<Ev_t, size_t>      EvCountMap_t;
typedef std::map<Ev_t, size_t>                     SortedEvCountMap_t;
typedef std::vector<EvCount_t>                     EvCountVector_t;
typedef std::tr1::unordered_map<size_t, EvCount_t> SizeEvCountMap_t; // Row_t: map "suitNumber" to proper column in matrix

////////////////////////////////////////////////////////////////////////////////

#endif // Include_EVCOUNT_H_

////////////////////////////////////////////////////////////////////////////////
