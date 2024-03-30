//
// Created by Administrator on 2024/3/29.
//

#include "RfxTest.h"
#include <iostream>

int main()
{
    Bitmap bmp;
    bmp.load(R"(D:\code\cpp\rfxcpp\test.bmp)");

    auto rfx_tester = RfxTest(bmp.getWidth(), bmp.getHeight(), bmp.getBytesPerPixel() * 8);

//    if (!rfx_tester.testTileSplitAndRebuildFrames(bmp))
//    {
//        std::cerr << "testTileSplitAndRebuildFrames failed! " << std::endl;
//    }

    if (!rfx_tester.testColorConvert(bmp))
    {
        std::cerr << "testColorConvert failed!" << std::endl;
    }

    return 0;
}