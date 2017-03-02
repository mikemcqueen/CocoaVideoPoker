
#pragma once
#ifndef Include_COMBINATIONSCUDA_H
#define Include_COMBINATIONSCUDA_H

#include "Combinations.h"

namespace Combinations
{
namespace Cuda
{

    size_t
    Calculate(
        const size_t N,
        const size_t K,
        const Data_t* data = NULL);


    bool 
    InitData(Data_t* data, size_t cardCount);

} // namespace Cuda
} // namespace Combinations

#endif // Include_COMBINATIONSCUDA_H
