//
// Created by Administrator on 2024/1/14.
//

#include "Quantization.h"
#include <cstdint>

static const uint32_t rfx_default_quantization_values[] =
        { 6, 6, 6, 6, 7, 7, 8, 8, 8, 9 };

void Quantization::Encode(DwtDecomposedTile & output, const std::vector<uint32_t> & quantization_values)
{

}

void Quantization::Decode(Tile_dwt & output, const std::vector<uint32_t> & quantization_values)
{

}
