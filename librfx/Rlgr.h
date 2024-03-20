//
// Created by Administrator on 2024/1/14.
//

#ifndef RFXCPP_RLGR_H
#define RFXCPP_RLGR_H

#include "Tile.h"

enum class RLGR_MODE {
    RLGR1,
    RLGR3
};

class Rlgr
{
public:
    explicit Rlgr(RLGR_MODE mod)
    :m_mod(mod)
    {}
public:
    void Encode(TileYCbCr & output, const TileYCbCr & input);
    void Decode(Tile_dwt & output, const Tile_dwt & input);
private:
    RLGR_MODE m_mod;
};


#endif //RFXCPP_RLGR_H
