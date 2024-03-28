//
// Created by Administrator on 2024/3/29.
//

#ifndef RFXCPP_RFXTEST_H
#define RFXCPP_RFXTEST_H
#include <Rfx.h>
#include <iostream>

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
};


#endif //RFXCPP_RFXTEST_H
