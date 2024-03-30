//
// Created by Administrator on 2024/3/31.
//

#ifndef RFXCPP_DWTTEST_H
#define RFXCPP_DWTTEST_H
#include <Rfx.h>
#include <unordered_map>
#include <iostream>

class DwtTest:public Rfx
{
public:
    DwtTest(uint32_t width, uint32_t height, uint32_t bpp)
    :Rfx(width, height, bpp)
    {}

    bool testDwt(const Bitmap & bmp)
    {
        bool ret  = true;

        std::vector<Tile_rgb> tiles_rgb;
        auto raw_rgb_data = bmp.getRgbData();

        splitTiles(tiles_rgb, raw_rgb_data);

        std::vector<Tile_dwt> tiles_dwt;
        for(auto t : tiles_rgb)
        {
            Tile_dwt t_dwt{};
            tileRgbToDwt(t_dwt, t);

            t_dwt.xIdx = t.xIdx;
            t_dwt.yIdx = t.yIdx;
            tiles_dwt.push_back(t_dwt);
        }

        std::vector<Tile_rgb> decode_rgbs;
        for(auto t:tiles_dwt)
        {
            DwtDecomposedTile dwt_decomposed_tile{};
            dwtEncode(dwt_decomposed_tile, t);

            Tile_dwt t_dwt{};
            dwtDecode(t_dwt, dwt_decomposed_tile);
            Tile_rgb decode_rgb{};
            tileDwtToRgb(decode_rgb, t_dwt);
            decode_rgb.xIdx = t.xIdx;
            decode_rgb.yIdx = t.yIdx;
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

        std::vector<uint32_t> decode_data;
        rebuildFrame(decode_data, decode_rgbs);

        Bitmap out;
        out.setRgbDate(decode_data, bmp.getWidth(), bmp.getHeight(), 0, bmp.getBytesPerPixel() * 8);
        out.dump("test_decoded.bmp");

        return ret;
    }
};


#endif //RFXCPP_DWTTEST_H
