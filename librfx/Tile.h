//
// Created by Administrator on 2024/2/26.
//

#ifndef RFXCPP_TILE_H
#define RFXCPP_TILE_H
#include <vector>
#include <array>
#include <cstdint>
#include <Stream.h>
#include "RectArray.h"

constexpr int tile_width = 64;
constexpr int tile_height = 64;
constexpr int tile_size = tile_width * tile_height;

using RgbTileData = std::array<uint8_t, tile_size>;
struct Tile_rgb
{
    uint16_t xIdx;
    uint16_t yIdx;
    RgbTileData r_data;
    RgbTileData g_data;
    RgbTileData b_data;
};

using DwtTileData = RectArray<int16_t, tile_width, tile_height>;
struct Tile_dwt
{
    uint16_t xIdx;
    uint16_t yIdx;
    DwtTileData y_data;
    DwtTileData cb_data;
    DwtTileData cr_data;
};

using YCbCrTileData = std::vector<uint8_t>;
struct TileYCbCr
{
    uint16_t xIdx;
    uint16_t yIdx;
    YCbCrTileData y_data;
    YCbCrTileData cb_data;
    YCbCrTileData cr_data;
};

struct DwtSubBands
{
    RectArray<int16_t, 32, 32> HL1;
    RectArray<int16_t, 32, 32> LH1;
    RectArray<int16_t, 32, 32> HH1;
    RectArray<int16_t, 16, 16> HL2;
    RectArray<int16_t, 16, 16> LH2;
    RectArray<int16_t, 16, 16> HH2;
    RectArray<int16_t, 8, 8> HL3;
    RectArray<int16_t, 8, 8> LH3;
    RectArray<int16_t, 8, 8> HH3;
    RectArray<int16_t, 8, 8> LL3;
};

struct DwtDecomposedTile
{
    uint16_t xIdx;
    uint16_t yIdx;
    DwtSubBands y;
    DwtSubBands cb;
    DwtSubBands cr;
};

void tileRgbToDwt(Tile_dwt & output, const Tile_rgb & input);
void tileDwtToRgb(Tile_rgb & output, const Tile_dwt & input);

#endif //RFXCPP_TILE_H
