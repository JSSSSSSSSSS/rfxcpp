//
// Created by Administrator on 2024/3/30.
//

#ifndef RFXCPP_LINEARIZATIONTEST_H
#define RFXCPP_LINEARIZATIONTEST_H

#include <Linearization.h>
#include <limits>
#include <cstdint>
#include <random>

class LinearizationTest
{

};

constexpr DwtSubBands generateDwtSubBands()
{
    DwtSubBands dwt_sub_bands{};

    for(auto i = 0; i < dwt_sub_bands.LL3.size(); ++i)
    {
        dwt_sub_bands.LL3[i] = static_cast<int16_t>((i + 1) % std::numeric_limits<int16_t>::max());
    }
    for(auto i = 0; i < dwt_sub_bands.LL3.size(); ++i)
    {
        dwt_sub_bands.LH3[i] = static_cast<int16_t>((i + 2) % std::numeric_limits<int16_t>::max());
    }
    for(auto i = 0; i < dwt_sub_bands.LL3.size(); ++i)
    {
        dwt_sub_bands.HL3[i] = static_cast<int16_t>((i + 3) % std::numeric_limits<int16_t>::max());
    }
    for(auto i = 0; i < dwt_sub_bands.LL3.size(); ++i)
    {
        dwt_sub_bands.HH3[i] = static_cast<int16_t>((i + 4) % std::numeric_limits<int16_t>::max());
    }
    for(auto i = 0; i < dwt_sub_bands.LL3.size(); ++i)
    {
        dwt_sub_bands.LH2[i] = static_cast<int16_t>((i + 5) % std::numeric_limits<int16_t>::max());
    }
    for(auto i = 0; i < dwt_sub_bands.LL3.size(); ++i)
    {
        dwt_sub_bands.HL2[i] = static_cast<int16_t>((i + 6) % std::numeric_limits<int16_t>::max());
    }
    for(auto i = 0; i < dwt_sub_bands.LL3.size(); ++i)
    {
        dwt_sub_bands.HH2[i] = static_cast<int16_t>((i + 7) % std::numeric_limits<int16_t>::max());
    }
    for(auto i = 0; i < dwt_sub_bands.LL3.size(); ++i)
    {
        dwt_sub_bands.LH1[i] = static_cast<int16_t>((i + 8) % std::numeric_limits<int16_t>::max());
    }
    for(auto i = 0; i < dwt_sub_bands.LL3.size(); ++i)
    {
        dwt_sub_bands.HL1[i] = static_cast<int16_t>((i + 9) % std::numeric_limits<int16_t>::max());
    }
    for(auto i = 0; i < dwt_sub_bands.LL3.size(); ++i)
    {
        dwt_sub_bands.HH1[i] = static_cast<int16_t>((i + 10) % std::numeric_limits<int16_t>::max());
    }

    return dwt_sub_bands;
}

constexpr inline DwtSubBands g_dwt_sub_bands = generateDwtSubBands();



#endif //RFXCPP_LINEARIZATIONTEST_H
