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
#include <iostream>

Rfx::Rfx(uint32_t width, uint32_t height, uint32_t bpp)
    : width_(width), height_(height), bit_per_pixel_(bpp)
{
    dwt_codec_ = std::make_unique<Dwt>();
    quantization_codec_ = std::make_unique<Quantization>();
    linearization_codec_ = std::make_unique<Linearization>();
    rlgr_codec_ = std::make_unique<Rlgr>(RLGR_MODE::RLGR1);
}

Rfx::~Rfx()
{

}

bool Rfx::encode(std::vector<uint8_t> &out, const Bitmap &in, uint32_t frameIndex)
{
    std::vector<Tile_rgb> tiles;
    splitTiles(tiles, in.getRgbData());
    std::vector<TileYCbCr> tiles_ycbcr;

    for(auto &t: tiles)
    {
        DwtDecomposedTile dwtDecomposedTile{};
        Tile_dwt t_dwt{};
        tileRgbToDwt(t_dwt, t);

        dwtEncode(dwtDecomposedTile, t_dwt);

        quantization_codec_->encode(dwtDecomposedTile.y, {});
        quantization_codec_->encode(dwtDecomposedTile.cb, {});
        quantization_codec_->encode(dwtDecomposedTile.cr, {});

        linearization_codec_->encode(t_dwt.y_data, dwtDecomposedTile.y);
        linearization_codec_->encode(t_dwt.cb_data, dwtDecomposedTile.cb);
        linearization_codec_->encode(t_dwt.cr_data, dwtDecomposedTile.cr);

        TileYCbCr t_yCbCr_rlgr{};
        rlgr_codec_->encode(t_yCbCr_rlgr.y_data, t_dwt.y_data);
        rlgr_codec_->encode(t_yCbCr_rlgr.cb_data, t_dwt.cb_data);
        rlgr_codec_->encode(t_yCbCr_rlgr.cr_data, t_dwt.cr_data);

        t_yCbCr_rlgr.xIdx = t.xIdx;
        t_yCbCr_rlgr.yIdx = t.yIdx;
        tiles_ycbcr.push_back(t_yCbCr_rlgr);
    }

    composeToStream(out, tiles_ycbcr);

    return true;
}

bool Rfx::decode(Bitmap &out, const std::span<uint8_t> &in, uint32_t &frameIndex)
{
    std::vector<TileYCbCr> tiles_ycbcr;
    decomposeFromStream(tiles_ycbcr, in);
    std::vector<Tile_rgb> tiles_rgb;

    for(auto & t: tiles_ycbcr)
    {
        Tile_dwt t_dwt{};
        std::vector<int16_t> decoded_data;
        decoded_data.resize(tile_size);
        rlgr_codec_->decode(decoded_data, t.y_data);
        t_dwt.y_data.assign(0, decoded_data.begin(), decoded_data.end());
        rlgr_codec_->decode(decoded_data, t.cb_data);
        t_dwt.cb_data.assign(0, decoded_data.begin(), decoded_data.end());
        rlgr_codec_->decode(decoded_data, t.cr_data);
        t_dwt.cr_data.assign(0, decoded_data.begin(), decoded_data.end());

        DwtDecomposedTile t_decomposed{};
        linearization_codec_->decode(t_decomposed.y, t_dwt.y_data);
        linearization_codec_->decode(t_decomposed.cb, t_dwt.cb_data);
        linearization_codec_->decode(t_decomposed.cr, t_dwt.cr_data);

        quantization_codec_->decode(t_decomposed.y, {});
        quantization_codec_->decode(t_decomposed.cb, {});
        quantization_codec_->decode(t_decomposed.cr, {});

        Tile_dwt t_dwt_dwt{};
        dwtDecode(t_dwt_dwt, t_decomposed);

        Tile_rgb t_rgb{};
        tileDwtToRgb(t_rgb, t_dwt_dwt);

        t_rgb.xIdx = t.xIdx;
        t_rgb.yIdx = t.yIdx;
        tiles_rgb.push_back(t_rgb);
    }

    std::vector<uint32_t> rgb_data;
    rebuildFrame(rgb_data, tiles_rgb);
    out.setRgbDate(rgb_data, width_, height_, 0, 32);

    return true;
}

void Rfx::splitTiles(std::vector<Tile_rgb> & tiles, const std::span<const uint32_t>& rgb_data) const
{
    uint32_t x_num = Alignment::AlignedFactor(width_, static_cast<decltype(width_)>(64));
    uint32_t y_num = Alignment::AlignedFactor(height_, static_cast<decltype(width_)>(64));

    for(uint32_t yIdx = 0; yIdx < y_num; ++yIdx)
    {
        for(uint32_t xIdx = 0; xIdx < x_num; ++xIdx)
        {
            Tile_rgb t{};
            t.xIdx = xIdx;
            t.yIdx = yIdx;

            uint32_t top_left_x = xIdx * 64;
            uint32_t top_left_y = yIdx * 64;
            uint32_t right_bottom_x = std::min(top_left_x + 64, width_);
            uint32_t right_bottom_y = std::min(top_left_y + 64, height_);

            for (uint32_t data_y = top_left_y, tile_y = 0; data_y < right_bottom_y; ++data_y, ++tile_y)
            {
                for (uint32_t data_x = top_left_x, tile_x = 0; data_x < right_bottom_x; ++data_x, ++tile_x)
                {
                    uint32_t tile_index = tile_y * 64 + tile_x;
                    uint32_t data_index = data_y * width_ + data_x;
                    ColorARGB argb {rgb_data[data_index]};
                    t.r_data[tile_index] = argb.r();
                    t.g_data[tile_index] = argb.g();
                    t.b_data[tile_index] = argb.b();
                }
            }

            tiles.push_back(t);
        }
    }
}

void Rfx::rebuildFrame(std::vector<uint32_t> &rgb_data, const std::vector<Tile_rgb> &tiles) const
{
    rgb_data.resize(width_ * height_);

    for(auto t:tiles)
    {
        uint32_t top_left_x = t.xIdx * 64;
        uint32_t top_left_y = t.yIdx * 64;
        uint32_t right_bottom_x = std::min(top_left_x + 64, width_);
        uint32_t right_bottom_y = std::min(top_left_y + 64, height_);

        for (uint32_t data_y = top_left_y, tile_y = 0; data_y < right_bottom_y; ++data_y, ++tile_y)
        {
            for (uint32_t data_x = top_left_x, tile_x = 0; data_x < right_bottom_x; ++data_x, ++tile_x)
            {
                uint32_t tile_index = tile_y * 64 + tile_x;
                uint32_t data_index = data_y * width_ + data_x;

                uint8_t a = 0xff;
                uint8_t r = t.r_data[tile_index];
                uint8_t g = t.g_data[tile_index];
                uint8_t b = t.b_data[tile_index];

                rgb_data[data_index] = static_cast<uint32_t>(ColorARGB(a, r, g, b));
            }
        }
    }
}

void Rfx::composeToStream(std::vector<uint8_t> &output, const std::vector<TileYCbCr> &input)
{
    OStream os(8192);
    size_t count = 0;

    for(auto & t:input)
    {
        os.write(t.xIdx);
        os.write(t.yIdx);
        os.write(t.y_data.size());
        os.write(t.cb_data.size());
        os.write(t.cr_data.size());
        os.write(t.y_data);
        os.write(t.cb_data);
        os.write(t.cr_data);

        count += t.y_data.size();
        count += t.cb_data.size();
        count += t.cr_data.size();
    }

    auto stream = os.data();
    output.assign(stream.begin(), stream.end());
}

void Rfx::decomposeFromStream(std::vector<TileYCbCr> &output, const std::span<const uint8_t> & input)
{
    IStream is(input);

    while(!is.eof())
    {
        TileYCbCr t_ycbcr{};
        is.read(t_ycbcr.xIdx);
        is.read(t_ycbcr.yIdx);

        size_t y_len, cb_len, cr_len;
        is.read(y_len);
        is.read(cb_len);
        is.read(cr_len);

        if (is.ensureRemain(y_len))
        {
            t_ycbcr.y_data.resize(y_len);
            is.read(t_ycbcr.y_data.begin(), y_len);
        }
        if (is.ensureRemain(cb_len))
        {
            t_ycbcr.cb_data.resize(cb_len);
            is.read(t_ycbcr.cb_data.begin(), cb_len);
        }
        if (is.ensureRemain(cr_len))
        {
            t_ycbcr.cr_data.resize(cr_len);
            is.read(t_ycbcr.cr_data.begin(), cr_len);
        }

        output.push_back(t_ycbcr);
    }
}

void Rfx::dwtEncode(DwtDecomposedTile & dwt_decomposed_tile, const Tile_dwt &tile_dwt)
{
    dwt_codec_->encode(dwt_decomposed_tile.y, tile_dwt.y_data);
    dwt_codec_->encode(dwt_decomposed_tile.cb, tile_dwt.cb_data);
    dwt_codec_->encode(dwt_decomposed_tile.cr, tile_dwt.cr_data);
}

void Rfx::dwtDecode(Tile_dwt &tile_dwt, const DwtDecomposedTile &dwt_decomposed_tile)
{
    dwt_codec_->decode(tile_dwt.y_data, dwt_decomposed_tile.y);
    dwt_codec_->decode(tile_dwt.cb_data, dwt_decomposed_tile.cb);
    dwt_codec_->decode(tile_dwt.cr_data, dwt_decomposed_tile.cr);
}
