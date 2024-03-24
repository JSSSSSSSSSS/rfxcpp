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
    : mod_(mod)
    {}
public:
    void encode(TileYCbCr & output, const Tile_dwt & input);
    void decode(Tile_dwt & output, const TileYCbCr & input);
private:
    void encodeRlgr1(TileYCbCr & output, const Tile_dwt & input);
    void decodeRlgr1(Tile_dwt & output, const TileYCbCr & input);
    void encodeRlgr3(TileYCbCr & output, const Tile_dwt & input);
    void decodeRlgr3(Tile_dwt & output, const TileYCbCr & input);
    static void encodePlaneRlgr1(std::vector<uint8_t> & output, std::span<const int16_t> input_plane);
    static void decodePlaneRlgr1(DwtTileData & output, std::span<const uint8_t> input_plane);
    void encodePlaneRlgr3(std::vector<uint8_t> & output, std::span<const int16_t> input_plane);
private:
    RLGR_MODE mod_;
};


#endif //RFXCPP_RLGR_H
