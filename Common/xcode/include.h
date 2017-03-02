/*
 *  include.h
 *  CardLib
 *
 *  Created by Mike McQueen on 6/5/10.
 *  Copyright 2010 Mike McQueen. All rights reserved.
 *
 */

//#define EXT_HASH_MAP

#ifdef EXT_HASH_MAP
    #include <ext/hash_map>
    #define hash_map_t __gnu_cxx::hash_map
#else
    #include <map>
    #define hash_map_t std::unordered_map
#endif

#define _countof( array ) ( sizeof( array )/sizeof( array[0] ) )


#define strcpy_s(dst, len, src) strlcpy(dst, src, len)
#define sprintf_s snprintf

#define FILETIME time_t


typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;

#define UIColorFromRGB(r,g,b) [UIColor colorWithRed: r / 255.0 green: g / 255.0 blue: b / 255.0 alpha: 1.0]
