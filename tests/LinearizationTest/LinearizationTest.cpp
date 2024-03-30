//
// Created by Administrator on 2024/3/30.
//

#include "LinearizationTest.h"
#include <iostream>

int main()
{
    DwtTileData plane{};
    Linearization::encode(plane, g_dwt_sub_bands);
    DwtSubBands dwt_sub_bands{};
    Linearization::decode(dwt_sub_bands, plane);

    if (!std::equal(g_dwt_sub_bands.LL3.begin(), g_dwt_sub_bands.LL3.end(),
                    dwt_sub_bands.LL3.begin(), dwt_sub_bands.LL3.end()))
    {
        std::cerr << "LL3 not equal!"<<std::endl;
    }

    if (!std::equal(g_dwt_sub_bands.LH3.begin(), g_dwt_sub_bands.LH3.end(),
                    dwt_sub_bands.LH3.begin(), dwt_sub_bands.LH3.end()))
    {
        std::cerr << "LH3 not equal!"<<std::endl;
    }

    if (!std::equal(g_dwt_sub_bands.HL3.begin(), g_dwt_sub_bands.HL3.end(),
                    dwt_sub_bands.HL3.begin(), dwt_sub_bands.HL3.end()))
    {
        std::cerr << "HL3 not equal!"<<std::endl;
    }
    if (!std::equal(g_dwt_sub_bands.HH3.begin(), g_dwt_sub_bands.HH3.end(),
                    dwt_sub_bands.HH3.begin(), dwt_sub_bands.HH3.end()))
    {
        std::cerr << "HH3 not equal!"<<std::endl;
    }
    if (!std::equal(g_dwt_sub_bands.LH2.begin(), g_dwt_sub_bands.LH2.end(),
                    dwt_sub_bands.LH2.begin(), dwt_sub_bands.LH2.end()))
    {
        std::cerr << "LH2 not equal!"<<std::endl;
    }
    if (!std::equal(g_dwt_sub_bands.HL2.begin(), g_dwt_sub_bands.HL2.end(),
                    dwt_sub_bands.HL2.begin(), dwt_sub_bands.HL2.end()))
    {
        std::cerr << "HL2 not equal!"<<std::endl;
    }
    if (!std::equal(g_dwt_sub_bands.HH2.begin(), g_dwt_sub_bands.HH2.end(),
                    dwt_sub_bands.HH2.begin(), dwt_sub_bands.HH2.end()))
    {
        std::cerr << "HH2 not equal!"<<std::endl;
    }
    if (!std::equal(g_dwt_sub_bands.LH1.begin(), g_dwt_sub_bands.LH1.end(),
                    dwt_sub_bands.LH1.begin(), dwt_sub_bands.LH1.end()))
    {
        std::cerr << "LH1 not equal!"<<std::endl;
    }
    if (!std::equal(g_dwt_sub_bands.HL1.begin(), g_dwt_sub_bands.HL1.end(),
                    dwt_sub_bands.HL1.begin(), dwt_sub_bands.HL1.end()))
    {
        std::cerr << "HL1 not equal!"<<std::endl;
    }
    if (!std::equal(g_dwt_sub_bands.HH1.begin(), g_dwt_sub_bands.HH1.end(),
                    dwt_sub_bands.HH1.begin(), dwt_sub_bands.HH1.end()))
    {
        std::cerr << "LL3 not equal!"<<std::endl;
    }

    return 0;
}