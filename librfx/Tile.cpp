//
// Created by Administrator on 2024/2/26.
//
#include "Tile.h"

void tileRgbToDwt(Tile_dwt & output, const Tile_rgb & input)
{
    //TODO: Convert rgb to ycbcr
    //TODO: left-shift y from [0, 255] to [-128, 127]
}

void tileDwtToRgb(Tile_rgb & output, const Tile_dwt & input)
{
    //TODO: Convert ycbcr to rgb
    //TODO: right-shift y from [-128, 127] to [0, 255]
}
