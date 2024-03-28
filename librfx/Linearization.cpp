//
// Created by Administrator on 2024/3/21.
//

#include "Linearization.h"

void Linearization::encode(DwtTileData & plane, const DwtSubBands & sub_bands)
{
    size_t index = 0;

    plane.assign(index, sub_bands.HL1.begin(), sub_bands.HL1.end());
    index += sub_bands.HL1.size();
    plane.assign(index, sub_bands.LH1.begin(), sub_bands.LH1.end());
    index += sub_bands.LH1.size();
    plane.assign(index, sub_bands.HH1.begin(), sub_bands.HH1.end());
    index += sub_bands.HH1.size();
    plane.assign(index, sub_bands.HL2.begin(), sub_bands.HL2.end());
    index += sub_bands.HL2.size();
    plane.assign(index, sub_bands.LH2.begin(), sub_bands.LH2.end());
    index += sub_bands.LH2.size();
    plane.assign(index, sub_bands.HH2.begin(), sub_bands.HH2.end());
    index += sub_bands.HH2.size();
    plane.assign(index, sub_bands.HL3.begin(), sub_bands.HL3.end());
    index += sub_bands.HL3.size();
    plane.assign(index, sub_bands.LH3.begin(), sub_bands.LH3.end());
    index += sub_bands.LH3.size();
    plane.assign(index, sub_bands.HH3.begin(), sub_bands.HH3.end());
    index += sub_bands.HH3.size();

    decltype(sub_bands.LL3) LL3_y_diff;
    encodeDifferential(LL3_y_diff, sub_bands.LL3);
    plane.assign(index, LL3_y_diff.begin(), LL3_y_diff.end());
}

void Linearization::encodeDifferential(std::span<int16_t> coefficients, std::span<const int16_t> input)
{
    if (input.empty() || coefficients.empty())
    {
        return;
    }

    coefficients[0] = input[0];

    for(int i = 1; i < input.size(); ++i)
    {
        coefficients[i] = static_cast<int16_t>(input[i] - input[i - 1]);
    }
}

void Linearization::decode(DwtSubBands &sub_bands, const DwtTileData & plane)
{
    size_t index = 0;
    auto iter = plane.begin();

    sub_bands.HL1.assign(0, iter, iter + sub_bands.HL1.size());
    iter += sub_bands.HL1.size();
    sub_bands.LH1.assign(0, iter, iter + sub_bands.LH1.size());
    iter += sub_bands.LH1.size();
    sub_bands.HH1.assign(0, iter, iter + sub_bands.HH1.size());
    iter += sub_bands.HH1.size();
    sub_bands.HL2.assign(0, iter, iter + sub_bands.HL2.size());
    iter += sub_bands.HL2.size();
    sub_bands.LH2.assign(0, iter, iter + sub_bands.LH2.size());
    iter += sub_bands.LH2.size();
    sub_bands.HH2.assign(0, iter, iter + sub_bands.HH2.size());
    iter += sub_bands.HH2.size();
    sub_bands.HL3.assign(0, iter, iter + sub_bands.HL3.size());
    iter += sub_bands.HL3.size();
    sub_bands.LH3.assign(0, iter, iter + sub_bands.LH3.size());
    iter += sub_bands.LH3.size();
    sub_bands.HH3.assign(0, iter, iter + sub_bands.HH3.size());
    iter += sub_bands.HH3.size();

    decltype(sub_bands.LL3) LL3_y_diff;
    LL3_y_diff.assign(0, iter, iter + sub_bands.LL3.size());

    decodeDifferential(sub_bands.LL3, LL3_y_diff);
}

void Linearization::decodeDifferential(std::span<int16_t> coefficients, std::span<const int16_t> input)
{
    if (input.empty() || coefficients.empty())
    {
        return;
    }

    coefficients[0] = input[0];

    for(int i = 1; i < input.size(); ++i)
    {
        coefficients[i] = static_cast<int16_t>(input[i] + input[i - 1]);
    }
}
