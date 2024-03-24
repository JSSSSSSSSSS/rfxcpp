//
// Created by Administrator on 2024/1/14.
//

#include "Dwt.h"

void Dwt::encodeTile(DwtDecomposedTile & output, const Tile_rgb & input)
{
    Tile_dwt tileDwt{};
    tileRgbToDwt(tileDwt, input);

    encodePlane(output.y, tileDwt.y_data);
    encodePlane(output.cb, tileDwt.cb_data);
    encodePlane(output.cr, tileDwt.cr_data);
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

void Dwt::decode(Tile_rgb &output, const DwtDecomposedTile &input)
{
    Tile_dwt t_dwt{};
    decodePlane(t_dwt.y_data, input.y);
    decodePlane(t_dwt.cb_data, input.cb);
    decodePlane(t_dwt.cr_data, input.cr);

    tileDwtToRgb(output, t_dwt);
}

void Dwt::decodePlane(DwtTileData &output, const DwtSubBands &input)
{
    DwtTileData tileData;

    // Level 3
    DwtHorizontal<8> level_3_horizontal{};
    level_3_horizontal.HL = input.HL3;
    level_3_horizontal.LH = input.LH3;
    level_3_horizontal.HH = input.HH3;
    level_3_horizontal.LL = input.LL3;

    DwtVertical<8> level_3_vertical{};
    decode2DHorizontal<8>(level_3_vertical, level_3_horizontal);
    RectArray<int16_t, 16, 16> output_3;
    decode2DVertical<8>(output_3, level_3_vertical);

    // Level 2
    DwtHorizontal<16> level_2_horizontal{};
    level_2_horizontal.HL = input.HL2;
    level_2_horizontal.LH = input.LH2;
    level_2_horizontal.HH = input.HH2;
    level_2_horizontal.LL = output_3;

    DwtVertical<16> level_2_vertical{};
    decode2DHorizontal<16>(level_2_vertical, level_2_horizontal);
    RectArray<int16_t, 32, 32> output_2;
    decode2DVertical<16>(output_2, level_2_vertical);

    // Level 1
    DwtHorizontal<32> level_1_horizontal{};
    level_1_horizontal.HL = input.HL1;
    level_1_horizontal.LH = input.LH1;
    level_1_horizontal.HH = input.HH1;
    level_1_horizontal.LL = output_2;

    DwtVertical<32> level_1_vertical{};
    decode2DHorizontal<32>(level_1_vertical, level_1_horizontal);
    decode2DVertical<32>(output, level_1_vertical);
}
