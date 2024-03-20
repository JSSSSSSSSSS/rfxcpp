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
    uint16_t width;
    uint16_t height;
    RgbTileData m_r_data;
    RgbTileData m_g_data;
    RgbTileData m_b_data;
};

using DwtTileData = RectArray<int16_t, tile_width, tile_height>;
struct Tile_dwt
{
    uint32_t num;
    uint16_t width;
    uint16_t height;
    DwtTileData m_y_data;
    DwtTileData m_cb_data;
    DwtTileData m_cr_data;
};

using YCbCrTileData = RectArray<int8_t, tile_width, tile_height>;
struct TileYCbCr
{
    uint32_t num;
    uint16_t width;
    uint16_t height;
    YCbCrTileData m_y_data;
    YCbCrTileData m_cb_data;
    YCbCrTileData m_cr_data;
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
    uint32_t num;
    DwtSubBands y;
    DwtSubBands cb;
    DwtSubBands cr;
};

void tileRgbToDwt(Tile_dwt & output, const Tile_rgb & input);
void tileDwtToRgb(Tile_rgb & output, const Tile_dwt & input);

#endif //RFXCPP_TILE_H
