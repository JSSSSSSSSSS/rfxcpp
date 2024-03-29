//
// Created by Administrator on 2024/3/29.
//

#ifndef RFXCPP_RFXTEST_H
#define RFXCPP_RFXTEST_H
#include <Rfx.h>
#include <iostream>
#include <unordered_map>

class RfxTest:public Rfx
{
public:
    RfxTest(uint32_t width, uint32_t height, uint32_t bpp)
    :Rfx(width, height, bpp)
    {}

    bool testTileSplitAndRebuildFrames(const Bitmap &in)
    {
        bool ret = true;
        std::vector<Tile_rgb> tiles_rgb;
        auto raw_rgb_data = in.getRgbData();

        splitTiles(tiles_rgb, raw_rgb_data);

        std::vector<uint32_t> rgb_data;
        rebuildFrame(rgb_data, tiles_rgb);

        for(auto i = 0; i < rgb_data.size(); ++i)
        {
            if (rgb_data[i] != raw_rgb_data[i])
            {
                std::cerr << "difference rgb data at index " << i << std::endl;
                ret = false;
                break;
            }
        }

        return ret;
    }

    bool testColorConvert(const Bitmap & in)
    {
        bool ret = true;
        std::vector<Tile_rgb> tiles_rgb;
        auto raw_rgb_data = in.getRgbData();

        splitTiles(tiles_rgb, raw_rgb_data);

        std::vector<Tile_dwt> tiles_dwt;
        for(auto t : tiles_rgb)
        {
            Tile_dwt t_dwt{};
            tileRgbToDwt(t_dwt, t);

            tiles_dwt.push_back(t_dwt);
        }

        std::vector<Tile_rgb> decode_rgbs;
        for(auto t:tiles_dwt)
        {
            Tile_rgb decode_rgb{};
            tileDwtToRgb(decode_rgb, t);
            decode_rgbs.push_back(decode_rgb);
        }

        std::unordered_map<int, int> counter_r;
        std::unordered_map<int, int> counter_g;
        std::unordered_map<int, int> counter_b;
        for(int i = 0; i < tiles_rgb.size(); ++i)
        {
            auto t1 = tiles_rgb[i];
            auto t2 = decode_rgbs[i];
            for(int j = 0; j < t1.r_data.size(); ++j)
            {
                auto diff_r = t1.r_data[j] - t2.r_data[j];
                auto diff_g = t1.g_data[j] - t2.g_data[j];
                auto diff_b = t1.b_data[j] - t2.b_data[j];
                ;
                counter_r[diff_r] =static_cast<int>(counter_r.contains(diff_r) ? counter_r[diff_r] + 1: 1);
                counter_g[diff_g] =static_cast<int>(counter_g.contains(diff_g) ? counter_g[diff_g] + 1: 1);
                counter_b[diff_b] =static_cast<int>(counter_b.contains(diff_b) ? counter_b[diff_b] + 1: 1);
            }
        }

        for(auto &[k , v]:counter_r)
        {
            if (k > 10)
            {
                ret = false;
                std::cerr << "counter_r:" << k << ", " << v << " is too big." << std::endl;
            }
            std::cout << "counter_r:" << k << ", " << v << std::endl;
        }
        for(auto &[k , v]:counter_g)
        {
            if (k > 10)
            {
                ret = false;
                std::cerr << "counter_g:" << k << ", " << v << " is too big." << std::endl;
            }
            std::cout << "counter_g:" << k << ", " << v << std::endl;
        }
        for(auto &[k , v]:counter_b)
        {
            if (k > 10)
            {
                ret = false;
                std::cerr << "counter_b:" << k << ", " << v << " is too big." << std::endl;
            }
            std::cout << "counter_b:" << k << ", " << v << std::endl;
        }

        return ret;
    }
};


#endif //RFXCPP_RFXTEST_H
