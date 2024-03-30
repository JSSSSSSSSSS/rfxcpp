//
// Created by Administrator on 2024/3/30.
//

#include "QuantizationTest.h"
#include <iostream>
#include <unordered_map>

void compare(const DwtSubBands & sub_bands, const DwtSubBands & sub_bands_other)
{
    std::array<std::unordered_map<int, int>, 10> counters;
    auto iter = counters.begin();

    for(auto i = 0; i < sub_bands.LL3.size(); ++i)
    {
        auto diff = sub_bands.LL3[i] - sub_bands_other.LL3[i];
        iter->operator[](diff) = static_cast<int>(iter->contains(diff) ? iter->at(diff) + 1: 1);
    }
    ++iter;
    for(auto i = 0; i < sub_bands.LH3.size(); ++i)
    {
        auto diff = sub_bands.LH3[i] - sub_bands_other.LH3[i];
        iter->operator[](diff) = static_cast<int>(iter->contains(diff) ? iter->at(diff) + 1: 1);
    }
    ++iter;
    for(auto i = 0; i < sub_bands.HL3.size(); ++i)
    {
        auto diff = sub_bands.HL3[i] - sub_bands_other.HL3[i];
        iter->operator[](diff) = static_cast<int>(iter->contains(diff) ? iter->at(diff) + 1: 1);
    }
    ++iter;
    for(auto i = 0; i < sub_bands.HH3.size(); ++i)
    {
        auto diff = sub_bands.HH3[i] - sub_bands_other.HH3[i];
        iter->operator[](diff) = static_cast<int>(iter->contains(diff) ? iter->at(diff) + 1: 1);
    }
    ++iter;
    for(auto i = 0; i < sub_bands.LH2.size(); ++i)
    {
        auto diff = sub_bands.LH2[i] - sub_bands_other.LH2[i];
        iter->operator[](diff) = static_cast<int>(iter->contains(diff) ? iter->at(diff) + 1: 1);
    }
    ++iter;
    for(auto i = 0; i < sub_bands.HL2.size(); ++i)
    {
        auto diff = sub_bands.HL2[i] - sub_bands_other.HL2[i];
        iter->operator[](diff) = static_cast<int>(iter->contains(diff) ? iter->at(diff) + 1: 1);
    }
    ++iter;
    for(auto i = 0; i < sub_bands.HH2.size(); ++i)
    {
        auto diff = sub_bands.HH2[i] - sub_bands_other.HH2[i];
        iter->operator[](diff) = static_cast<int>(iter->contains(diff) ? iter->at(diff) + 1: 1);
    }
    ++iter;
    for(auto i = 0; i < sub_bands.LH1.size(); ++i)
    {
        auto diff = sub_bands.LH1[i] - sub_bands_other.LH1[i];
        iter->operator[](diff) = static_cast<int>(iter->contains(diff) ? iter->at(diff) + 1: 1);
    }
    ++iter;
    for(auto i = 0; i < sub_bands.HL1.size(); ++i)
    {
        auto diff = sub_bands.HL1[i] - sub_bands_other.HL1[i];
        iter->operator[](diff) = static_cast<int>(iter->contains(diff) ? iter->at(diff) + 1: 1);
    }
    ++iter;
    for(auto i = 0; i < sub_bands.HH1.size(); ++i)
    {
        auto diff = sub_bands.HH1[i] - sub_bands_other.HH1[i];
        iter->operator[](diff) = static_cast<int>(iter->contains(diff) ? iter->at(diff) + 1: 1);
    }

    for(auto i = 0; i < counters.size(); ++i)
    {
        auto & c = counters[i];
        std::cout << "diff index " << i << " =================" << std::endl;
        for(auto &[k,v]:c)
        {
            std::cout << k << ", " << v << std::endl;
        }
    }
}

int main()
{
    DwtSubBands sub_bands = g_dwt_sub_bands;
    Quantization::encode(sub_bands, {});
    Quantization::decode(sub_bands, {});

    compare(g_dwt_sub_bands, sub_bands);

    return 0;
}