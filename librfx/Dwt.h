//
// Created by Administrator on 2024/1/14.
//

#ifndef RFXCPP_DWT_H
#define RFXCPP_DWT_H
#include "Tile.h"
#include <RectArray.h>

template<size_t N>
struct DwtVertical
{
    RectArray<int16_t, 2 * N, N> L;
    RectArray<int16_t, 2 * N, N> H;
};

template<size_t N>
struct DwtHorizontal
{
    RectArray<int16_t, N, N> LL;
    RectArray<int16_t, N, N> HL;
    RectArray<int16_t, N, N> LH;
    RectArray<int16_t, N, N> HH;
};

class Dwt
{
    using coefficient_type = int16_t;
public:
    void encodeTile(DwtDecomposedTile & output, const Tile_rgb & input);
    void Decode(Tile_rgb & output, const Tile_dwt & input);
private:
    void encodePlane(DwtSubBands & output, const DwtTileData & input);
    void Decode_2d(Tile_dwt & output, const Tile_dwt & input);

    template<uint32_t Size>
    void encode2DVertical(DwtVertical<Size> & out, const RectArray<coefficient_type ,2 * Size, 2 * Size> & in)
    {
        DwtVertical<Size> tmp;
        in.horizontalBisect(tmp.L, tmp.H);

        filterCoefficientsHighPass(out.H, tmp.H);
        filterCoefficientsLowPass(out.L, tmp.L);
    }

    template<uint32_t Size>
    void encode2DHorizontal(DwtHorizontal<Size> & out, const DwtVertical<Size> & in)
    {
        DwtHorizontal<Size> tmp;
        in.L.verticalBisect(tmp.LL, tmp.HL);
        in.H.verticalBisect(tmp.LH, tmp.HH);

        filterCoefficientsHighPass(out.HL, tmp.HL);
        filterCoefficientsLowPass(out.LL, tmp.LL);
        filterCoefficientsHighPass(out.HH, tmp.HH);
        filterCoefficientsLowPass(out.LH, tmp.LH);
    }

    void filterCoefficientsHighPass(std::span<coefficient_type> out, const std::span<const coefficient_type> & in);
    void filterCoefficientsLowPass(std::span<coefficient_type> out, const std::span<const coefficient_type> & in);

    void inverseFilterCoefficientsEven(std::span<coefficient_type> & out, const std::span<coefficient_type> & in);
    void inverseFilterCoefficientsOdd(std::span<coefficient_type> & out, const std::span<coefficient_type> & in);
};


#endif //RFXCPP_DWT_H
