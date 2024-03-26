//
// Created by Administrator on 2024/3/26.
//

#include "RlgrTest.h"
#include <iostream>

int main()
{
    Rlgr rlgr{RLGR_MODE::RLGR1};

    std::vector<uint8_t> encoded_data;
    encoded_data.resize(tile_size);
    rlgr.encode(encoded_data, test_r_data);

    std::cout << "encoded data size:" << encoded_data.size() << std::endl;

    std::vector<int16_t> decoded_data;
    decoded_data.resize(tile_size);
    rlgr.decode(decoded_data, encoded_data);

    for(int i = 0; i < decoded_data.size(); ++i)
    {
        if (test_r_data[i] != decoded_data[i])
        {
            std::cout << "error decode!" << std::endl;
            break;
        }
    }

    return 0;
}