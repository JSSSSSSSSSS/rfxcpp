//
// Created by Administrator on 2024/1/14.
//

#ifndef RFXCPP_QUANTIZATION_H
#define RFXCPP_QUANTIZATION_H
#include "Tile.h"

class Quantization
{
public:
    void encode(DwtSubBands & sub_bands, std::span<const int> quantization_values);
    void decode(DwtSubBands & sub_bands, std::span<const int> quantization_values);
private:
    static void encodeSubBands(DwtSubBands & sub_bands, std::span<const int> quantization_values);
    static void decodeSubBands(DwtSubBands & sub_bands, std::span<const int> quantization_values);
    static void divideAndRound(int16_t & value, int factor);
    static void multipleAndRound(int16_t & value, int factor);
};


#endif //RFXCPP_QUANTIZATION_H
