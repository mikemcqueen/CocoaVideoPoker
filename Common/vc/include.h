/*
 *  include.h
 */

#include "targetver.h"

//#define _SECURE_SCL 0
//#define _CRT_SECURE_NO_WARNINGS

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <windows.h>

#include <unordered_map>

//#include <boost/foreach.hpp>
//#include <boost/tokenizer.hpp>

#pragma warning(disable:4512) // assignment operator cannot be generated (due to const ivar usually)

using std::tr1::unordered_map;

#define hash_map_t unordered_map

typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;
typedef __int64        int64_t;
