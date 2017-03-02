////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CombinationsCuda.h"
#include "Log_t.h"

namespace Combinations
{
namespace Cuda
{

size_t
factorial(
    const size_t num,
    const size_t stopAt = 1)
{
    if (stopAt == num)
    {
        return 1;
    }
    else
    {
        return num * factorial(num - 1, stopAt);
    }
}

size_t
Calculate(
    const size_t N,
    const size_t K,
    const Data_t* data)
{

    if (data != NULL)
    {
#if 0
        return data->at(N).at(K);
#else
        return data[N].K[K];
#endif
    }

    return factorial(N, N - K) / factorial(K);
}

bool 
InitData(Data_t* data, size_t cardCount)
{
    for (size_t c = cardCount; c > 1; --c)
    {
        size_t k = 5 < (c - 1) ? 5 : c - 1;
        for (; k > 0; --k)
        {
            data[c].K[k] = Calculate(c, k);
        }
    }
    return true;
}

} // namespace Cuda
} // namespace Combinations