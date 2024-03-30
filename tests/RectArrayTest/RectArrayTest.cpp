//
// Created by Administrator on 2024/3/28.
//

#include "RectArrayTest.h"
#include <iostream>
#include <RectArray.h>

int main()
{
    RectArray<int, 8, 6> rect_array = {0,1,2,3,4,5,6,7,
                                           10,11,12,13,14,15,16,17,
                                           20,21,22,23,24,25,26,27,
                                           30,31,32,33,34,35,36,37,
                                           40,41,42,43,44,45,46,47,
                                           50,51,52,53,54,55,56,57};
    RectArrayRef<int, 8> row_ref{};

    rect_array.row(row_ref, 1);

    std::cout << "get row index 1:" << std::endl;
    for(int i = 0; i < row_ref.size(); ++i)
    {
        std::cout << row_ref[i] << ",";
    }

    std::cout << std::endl;

    std::cout << "add 10:" << std::endl;
    for(int i = 0; i < row_ref.size(); ++i)
    {
        row_ref[i] += 10;
    }

    for(int i = 0; i < rect_array.size(); ++i)
    {
        if (i % 8 == 0)
            std::cout << std::endl;
        std::cout << rect_array[i] << ",";
    }

    std::cout << std::endl;

    RectArrayRef<int, 6> col_ref{};
    rect_array.col(col_ref, 2);

    std::cout << "get col index 2:" << std::endl;
    for(int i = 0; i < col_ref.size(); ++i)
    {
        std::cout << col_ref[i] << ",";
    }

    std::cout << std::endl;

    std::cout << "add 10:" << std::endl;
    for(int i = 0; i < col_ref.size(); ++i)
    {
        col_ref[i] += 10;
    }

    for(int i = 0; i < rect_array.size(); ++i)
    {
        if (i % 8 == 0)
            std::cout << std::endl;
        std::cout << rect_array[i] << ",";
    }

    return 0;
}
