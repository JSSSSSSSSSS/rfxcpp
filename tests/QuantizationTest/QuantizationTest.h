//
// Created by Administrator on 2024/3/30.
//

#ifndef RFXCPP_QUANTIZATIONTEST_H
#define RFXCPP_QUANTIZATIONTEST_H
#include <Quantization.h>
#include <limits>

class QuantizationTest
{

};

constexpr DwtSubBands generateDwtSubBands()
{
    DwtSubBands dwt_sub_bands{};

    for(auto i = 0; i < dwt_sub_bands.LL3.size(); ++i)
    {
        dwt_sub_bands.LL3[i] = static_cast<int16_t>((i*10 + 1) % std::numeric_limits<int16_t>::max());
    }
    for(auto i = 0; i < dwt_sub_bands.LL3.size(); ++i)
    {
        dwt_sub_bands.LH3[i] = static_cast<int16_t>((i*10 + 2) % std::numeric_limits<int16_t>::max());
    }
    for(auto i = 0; i < dwt_sub_bands.LL3.size(); ++i)
    {
        dwt_sub_bands.HL3[i] = static_cast<int16_t>((i*10 + 3) % std::numeric_limits<int16_t>::max());
    }
    for(auto i = 0; i < dwt_sub_bands.LL3.size(); ++i)
    {
        dwt_sub_bands.HH3[i] = static_cast<int16_t>((i*10 + 4) % std::numeric_limits<int16_t>::max());
    }
    for(auto i = 0; i < dwt_sub_bands.LL3.size(); ++i)
    {
        dwt_sub_bands.LH2[i] = static_cast<int16_t>((i*10 + 5) % std::numeric_limits<int16_t>::max());
    }
    for(auto i = 0; i < dwt_sub_bands.LL3.size(); ++i)
    {
        dwt_sub_bands.HL2[i] = static_cast<int16_t>((i*10 + 6) % std::numeric_limits<int16_t>::max());
    }
    for(auto i = 0; i < dwt_sub_bands.LL3.size(); ++i)
    {
        dwt_sub_bands.HH2[i] = static_cast<int16_t>((i*10 + 7) % std::numeric_limits<int16_t>::max());
    }
    for(auto i = 0; i < dwt_sub_bands.LL3.size(); ++i)
    {
        dwt_sub_bands.LH1[i] = static_cast<int16_t>((i*10 + 8) % std::numeric_limits<int16_t>::max());
    }
    for(auto i = 0; i < dwt_sub_bands.LL3.size(); ++i)
    {
        dwt_sub_bands.HL1[i] = static_cast<int16_t>((i*10 + 9) % std::numeric_limits<int16_t>::max());
    }
    for(auto i = 0; i < dwt_sub_bands.LL3.size(); ++i)
    {
        dwt_sub_bands.HH1[i] = static_cast<int16_t>((i*10 + 10) % std::numeric_limits<int16_t>::max());
    }

    return dwt_sub_bands;
}

constexpr inline DwtSubBands g_dwt_sub_bands = generateDwtSubBands();

#endif //RFXCPP_QUANTIZATIONTEST_H
