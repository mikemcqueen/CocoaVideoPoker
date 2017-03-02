#pragma once
#ifndef Include_COMBINATIONS_H
#define Include_COMBINATIONS_H

namespace Combinations
{
    struct Data_t
    {
        size_t K[6];
    };

    size_t
    Calculate(
        const size_t N,
        const size_t K,
        const Data_t* data = NULL);


    bool 
    InitData(Data_t* data, size_t cardCount);

}

#endif // Include_COMBINATIONS_H
