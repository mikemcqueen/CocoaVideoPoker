#pragma once

#ifndef Include_PLATFORM_H
#define Include_PLATFORM_H

#ifdef _MSC_VER
#include "vc/include.h"
typedef int       int32_t;
typedef unsigned  uint32_t;


#else
#include "xcode/include.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include <sstream>
#include <iostream>
#include <fstream>

#define ASSERT(x) 

using namespace std;

#endif // Include_PLATFORM_H

