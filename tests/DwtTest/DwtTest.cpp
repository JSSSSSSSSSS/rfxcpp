//
// Created by Administrator on 2024/3/31.
//

#include "DwtTest.h"
#include <iostream>


int main()
{
    Bitmap bmp;
    bmp.load(R"(D:\code\cpp\rfxcpp\test.bmp)");

    DwtTest test(bmp.getWidth(), bmp.getHeight(), 32);
    if (!test.testDwt(bmp))
    {
        std::cerr << "testDwt failed!" << std::endl;
    }

    return 0;
}
