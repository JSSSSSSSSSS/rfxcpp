//
// Created by Administrator on 2024/1/14.
//

#include "Rfx.h"
#include "Dwt.h"
#include "Quantization.h"
#include "Rlgr.h"
#include "RfxMessage.h"
#include <Alignment.h>
#include <Color.h>

Rfx::Rfx(uint32_t width, uint32_t height, uint32_t bpp)
    :m_width(width), m_height(height), m_bpp(bpp)
{

}

Rfx::~Rfx()
{

}

bool Rfx::encode(std::vector<uint8_t> &out, const Bitmap &in, uint32_t frameIndex)
{
    std::vector<Tile_rgb> tiles;
    SplitTiles(tiles, in.GetRGBData());
    std::vector<TileYCbCr> tiles_dwt;

    for(auto &t: tiles)
    {
        DwtDecomposedTile dwtDecomposedTile{};
        m_dwt->encodeTile(dwtDecomposedTile, t);
        m_quantization->Encode(dwtDecomposedTile, {});

        TileYCbCr t_yCbCr{};
        linearization_->encode(t_yCbCr, dwtDecomposedTile);

        TileYCbCr t_yCbCr_rlgr{};
        m_rlgr->Encode(t_yCbCr_rlgr, t_yCbCr);

        tiles_dwt.push_back(t_yCbCr_rlgr);
    }

    OStream os(out);
    RfxMessage rfx_msg(frameIndex);
    // TODO:setting rfx_msg here. Given data/width/height/format etc.
    rfx_msg.Compose(os);

    return true;
}

bool Rfx::Decode(Bitmap &out, const std::span<uint8_t> &in, uint32_t &frameIndex)
{
    return false;
}

void Rfx::SplitTiles(std::vector<Tile_rgb> & tiles, const std::span<const uint32_t>& rgb_data) const
{
    uint32_t x_num = Alignment::AlignedFactor(m_width, static_cast<decltype(m_width)>(64));
    uint32_t y_num = Alignment::AlignedFactor(m_height, static_cast<decltype(m_width)>(64));

    for(uint32_t yIdx = 0; yIdx < y_num; ++yIdx)
    {
        for(uint32_t xIdx = 0; xIdx < x_num; ++xIdx)
        {
            Tile_rgb t{};
            t.xIdx = xIdx;
            t.yIdx = yIdx;

            uint32_t top_left_x = xIdx * 64;
            uint32_t top_left_y = yIdx * 64;
            uint32_t right_bottom_x = std::min(top_left_x + 64, m_width);
            uint32_t right_bottom_y = std::min(top_left_y + 64, m_height);

            for (uint32_t data_y = top_left_y, tile_y = 0; data_y < right_bottom_y; ++data_y, ++tile_y)
            {
                for (uint32_t data_x = top_left_x, tile_x = 0; data_x < right_bottom_x; ++data_x, ++tile_x)
                {
                    uint32_t tile_index = tile_y * 64 + tile_x;
                    uint32_t data_index = data_y * m_width + data_x;
                    ColorRGB rgb {rgb_data[data_index]};
                    t.m_r_data[tile_index] = rgb.r();
                    t.m_g_data[tile_index] = rgb.g();
                    t.m_b_data[tile_index] = rgb.b();
                }
            }

            t.width = right_bottom_x - top_left_x;
            t.height = right_bottom_y - top_left_y;

            tiles.push_back(t);
        }
    }
}
