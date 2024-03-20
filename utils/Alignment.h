//
// Created by Administrator on 2024/3/10.
//

#ifndef RFXCPP_ALIGNMENT_H
#define RFXCPP_ALIGNMENT_H
#include <concepts>

class Alignment
{
public:
    template <std::integral T>
    static T Align(T value, T offset)
    {
        auto factor = value / offset;
        auto valueAligned = factor * offset;
        if (valueAligned < value)
        {
            valueAligned += offset;
        }
        return valueAligned;
    }

    template <std::integral T>
    static T AlignedFactor(T value, T offset)
    {
        auto factor = value / offset;
        if (factor * offset < value)
        {
            factor += 1;
        }
        return factor;
    }
};


#endif //RFXCPP_ALIGNMENT_H
