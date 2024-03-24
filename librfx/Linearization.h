//
// Created by Administrator on 2024/3/21.
//

#ifndef RFXCPP_LINEARIZATION_H
#define RFXCPP_LINEARIZATION_H
#include <Tile.h>

class Linearization
{
public:
    static void encode(Tile_dwt & output, const DwtDecomposedTile & input);
    static void decode(DwtDecomposedTile & output, const Tile_dwt & input);
private:
    static void encodePlane(DwtTileData & plane, const DwtSubBands & sub_bands);
    static void decodePlane(DwtSubBands & plane, const DwtTileData & sub_bands);
    static void encodeDifferential(std::span<int16_t> coefficients, std::span<const int16_t> input);
    static void decodeDifferential(std::span<int16_t> coefficients, std::span<const int16_t> input);
};


#endif //RFXCPP_LINEARIZATION_H
