//
// Created by Administrator on 2024/3/21.
//

#ifndef RFXCPP_LINEARIZATION_H
#define RFXCPP_LINEARIZATION_H
#include <Tile.h>

class Linearization
{
public:
    void encode(TileYCbCr & output, const DwtDecomposedTile & input);
    void Decode(Tile_dwt & output, const std::vector<uint32_t>& quantization_values);
};


#endif //RFXCPP_LINEARIZATION_H
