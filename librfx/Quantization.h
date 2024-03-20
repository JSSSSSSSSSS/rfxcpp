//
// Created by Administrator on 2024/1/14.
//

#ifndef RFXCPP_QUANTIZATION_H
#define RFXCPP_QUANTIZATION_H
#include "Tile.h"

class Quantization
{
public:
    void Encode(DwtDecomposedTile & output, const std::vector<uint32_t> & quantization_values);
    void Decode(Tile_dwt & output, const std::vector<uint32_t>& quantization_values);
};


#endif //RFXCPP_QUANTIZATION_H
