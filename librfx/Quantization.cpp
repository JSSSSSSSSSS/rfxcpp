//
// Created by Administrator on 2024/1/14.
//

#include "Quantization.h"
#include <cstdint>
#include <limits>

static const std::array<int, 10> rfx_default_quantization_values =
        { 6, 6, 6, 6, 7, 7, 8, 8, 8, 9 };

void Quantization::encode(DwtDecomposedTile & output, std::span<const int> quantization_values)
{
    // There may be multiple quantization values given by TS_RFX_TILESET quantVals field.
    // Each tile can use different quantization values which specified by
    //          TS_RFX_TILE quantIdxY\quantIdxCb\quantIdxCr field.

    // Simply, we use one group quantization values in all tiles. And specifies quantIdxY\quantIdxCb\quantIdxCr 0.
    encodeSubBands(output.y, rfx_default_quantization_values);
    encodeSubBands(output.cb, rfx_default_quantization_values);
    encodeSubBands(output.cr, rfx_default_quantization_values);
}

void Quantization::decode(DwtDecomposedTile & output, std::span<const int> quantization_values)
{
    // There may be multiple quantization values given by TS_RFX_TILESET quantVals field.
    // Each tile can use different quantization values which specified by
    //          TS_RFX_TILE quantIdxY\quantIdxCb\quantIdxCr field.

    // Simply, we use one group quantization values in all tiles. And specifies quantIdxY\quantIdxCb\quantIdxCr 0.
    decodeSubBands(output.y, rfx_default_quantization_values);
    decodeSubBands(output.cb, rfx_default_quantization_values);
    decodeSubBands(output.cr, rfx_default_quantization_values);
}

void Quantization::encodeSubBands(DwtSubBands &sub_bands, std::span<const int> quantization_values)
{
    int factor = 1 << (quantization_values[0] - 6);
    for(auto & value:sub_bands.LL3)
    {
        divideAndRound(value, factor);
    }

    factor = 1 << (quantization_values[1] - 6);
    for(auto & value:sub_bands.LH3)
    {
        divideAndRound(value, factor);
    }

    factor = 1 << (quantization_values[2] - 6);
    for(auto & value:sub_bands.HL3)
    {
        divideAndRound(value, factor);
    }

    factor = 1 << (quantization_values[3] - 6);
    for(auto & value:sub_bands.HH3)
    {
        divideAndRound(value, factor);
    }

    factor = 1 << (quantization_values[4] - 6);
    for(auto & value:sub_bands.LH2)
    {
        divideAndRound(value, factor);
    }
    factor = 1 << (quantization_values[5] - 6);
    for(auto & value:sub_bands.HL2)
    {
        divideAndRound(value, factor);
    }
    factor = 1 << (quantization_values[6] - 6);
    for(auto & value:sub_bands.HH2)
    {
        divideAndRound(value, factor);
    }
    factor = 1 << (quantization_values[7] - 6);
    for(auto & value:sub_bands.LH1)
    {
        divideAndRound(value, factor);
    }
    factor = 1 << (quantization_values[8] - 6);
    for(auto & value:sub_bands.HL1)
    {
        divideAndRound(value, factor);
    }
    factor = 1 << (quantization_values[9] - 6);
    for(auto & value:sub_bands.HH1)
    {
        divideAndRound(value, factor);
    }
}

void Quantization::divideAndRound(int16_t & value, int factor)
{
    if (factor == 0)
        return;

    double divided_value_f = static_cast<double>(value) / factor;
    int divided_value = value / factor;

    if (divided_value_f - divided_value > 0.5f)
    {
        divided_value += 1;
    }

    value = static_cast<int16_t>(divided_value);
}

void Quantization::decodeSubBands(DwtSubBands &sub_bands, std::span<const int> quantization_values)
{
    int factor = 1 << (quantization_values[0] - 6);
    for(auto & value:sub_bands.LL3)
    {
        multipleAndRound(value, factor);
    }

    factor = 1 << (quantization_values[1] - 6);
    for(auto & value:sub_bands.LH3)
    {
        multipleAndRound(value, factor);
    }

    factor = 1 << (quantization_values[2] - 6);
    for(auto & value:sub_bands.HL3)
    {
        multipleAndRound(value, factor);
    }

    factor = 1 << (quantization_values[3] - 6);
    for(auto & value:sub_bands.HH3)
    {
        multipleAndRound(value, factor);
    }

    factor = 1 << (quantization_values[4] - 6);
    for(auto & value:sub_bands.LH2)
    {
        multipleAndRound(value, factor);
    }
    factor = 1 << (quantization_values[5] - 6);
    for(auto & value:sub_bands.HL2)
    {
        multipleAndRound(value, factor);
    }
    factor = 1 << (quantization_values[6] - 6);
    for(auto & value:sub_bands.HH2)
    {
        multipleAndRound(value, factor);
    }
    factor = 1 << (quantization_values[7] - 6);
    for(auto & value:sub_bands.LH1)
    {
        multipleAndRound(value, factor);
    }
    factor = 1 << (quantization_values[8] - 6);
    for(auto & value:sub_bands.HL1)
    {
        multipleAndRound(value, factor);
    }
    factor = 1 << (quantization_values[9] - 6);
    for(auto & value:sub_bands.HH1)
    {
        multipleAndRound(value, factor);
    }
}

void Quantization::multipleAndRound(int16_t &value, int factor)
{
    if (factor == 0)
        return;

    int multiple_value = value * factor;

    multiple_value = std::min(
            std::max(multiple_value, static_cast<int>(std::numeric_limits<int16_t>::min())),
            static_cast<int>(std::numeric_limits<int16_t>::max()));

    value = static_cast<int16_t>(multiple_value);
}
