//
// Created by Administrator on 2024/1/14.
//

#include "Dwt.h"

void Dwt::encodeTile(DwtDecomposedTile & output, const Tile_rgb & input)
{
    Tile_dwt tileDwt{};
    tileRgbToDwt(tileDwt, input);

    encodePlane(output.y, tileDwt.m_y_data);
    encodePlane(output.cb, tileDwt.m_cb_data);
    encodePlane(output.cr, tileDwt.m_cr_data);
}

void Dwt::encodePlane(DwtSubBands & output, const DwtTileData & input)
{
    DwtTileData tileData;

    // Level 1
    DwtVertical<32> level_1_vertical{};
    encode2DVertical<32>(level_1_vertical, input);
    DwtHorizontal<32> level_1_horizontal{};
    encode2DHorizontal<32>(level_1_horizontal, level_1_vertical);

    // Level 2
    DwtVertical<16> level_2_vertical{};
    encode2DVertical<16>(level_2_vertical, level_1_horizontal.LL);
    DwtHorizontal<16> level_2_horizontal{};
    encode2DHorizontal<16>(level_2_horizontal, level_2_vertical);

    // Level 3
    DwtVertical<8> level_3_vertical{};
    encode2DVertical<8>(level_3_vertical, level_2_horizontal.LL);
    DwtHorizontal<8> level_3_horizontal{};
    encode2DHorizontal<8>(level_3_horizontal, level_3_vertical);

    output.HL1 = level_1_horizontal.HL;
    output.LH1 = level_1_horizontal.LH;
    output.HH1 = level_1_horizontal.HH;

    output.HL2 = level_2_horizontal.HL;
    output.LH2 = level_2_horizontal.LH;
    output.HH2 = level_2_horizontal.HH;

    output.HL3 = level_3_horizontal.HL;
    output.LH3 = level_3_horizontal.LH;
    output.HH3 = level_3_horizontal.HH;
    output.LL3 = level_3_horizontal.LL;
}

void Dwt::filterCoefficientsHighPass(std::span<coefficient_type> out, const std::span<const coefficient_type> &in)
{

}

void Dwt::filterCoefficientsLowPass(std::span<coefficient_type> out, const std::span<const coefficient_type> &in)
{

}
