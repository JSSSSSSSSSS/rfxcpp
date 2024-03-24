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
    void decode(Tile_rgb & output, const DwtDecomposedTile & input);
private:
    void encodePlane(DwtSubBands & output, const DwtTileData & input);
    void decodePlane(DwtTileData & output, const DwtSubBands & input);

    template<uint32_t Size>
    void encode2DVertical(DwtVertical<Size> & out, const RectArray<coefficient_type ,2 * Size, 2 * Size> & in)
    {
        auto & [L, H] = out;

        // vertical filtering, for each col, generator two sub-bands.
        constexpr size_t width = 2 * Size;
        for(int x = 0; x < width; ++x)
        {
            // left boundary
            coefficient_type h = (in.getByPos(x,1) - ((in.getByPos(x, 0) + in.getByPos(x, 2)) >> 1 )) >> 1;
            coefficient_type l = in.getByPos(x, 0) + h;
            H.setByPos(h, x, 0);
            L.setByPos(l, x, 0);

            int n = 1;
            int y = n << 1;
            for(n = 1; n < Size - 1; ++n)
            {
                y = n << 1;
                h = (in.getByPos(x, y + 1) - ((in.getByPos(x, y) + in.getByPos(x, y + 2)) >> 1))
                        >> 1;
                H.setByPos(h, x, n);
                l = in.getByPos(x, y) + ((H.getByPos(x, n - 1) + H.getByPos(x, n)) >> 1);
                L.setByPos(l, x, n);
            }

            // right boundary
            y = n << 1;
            h = (in.getByPos(x, y + 1) - ((in.getByPos(x, y) + in.getByPos(x, y)) >> 1)) >> 1;
            H.setByPos(h, x, n);
            l = in.getByPos(x, 0) + ((H.getByPos(x, n - 1) + H.getByPos(x, n)) >> 1);
            L.setByPos(l, x, n);
        }
    }

    template<uint32_t Size>
    void encode2DHorizontal(DwtHorizontal<Size> & out, const DwtVertical<Size> & in)
    {
        auto & [LL, HL, LH, HH] = out;
        auto & [L, H] = in;

        // Horizontal filtering, for each row, generate two sub-bands.
        // There are two sub-bands input, generate four sub-bands.
        for(int y = 0; y < Size; ++y)
        {
            // left boundary, n = 0
            coefficient_type hl = (L.getByPos(1, y) - ((L.getByPos(0, y) + L.getByPos(2, y)) >> 1)) >> 1;
            coefficient_type ll = L.getByPos(0, y) + hl;
            HL.setByPos(hl, 0, y);
            LL.setByPos(ll, 0, y);
            coefficient_type hh = (H.getByPos(1, y) - ((H.getByPos(0, y) + H.getByPos(2, y)) >> 1)) >> 1;;
            coefficient_type lh = H.getByPos(0, y) + hh;
            HH.setByPos(hh, 0, y);
            LH.setByPos(lh, 0, y);

            int x = 0;
            int n = 1;
            for(n = 1; n < Size - 1; ++n)
            {
                x = n << 1;
                hl = (L.getByPos(x + 1, y) - ((L.getByPos(x, y) + L.getByPos(x + 2,y)) >> 1))
                        >> 1;
                HL.setByPos(hl, n, y);
                ll = L.getByPos(x, y) + ((HL.getByPos(n - 1, y) + HL.getByPos(n, y)) >> 1);
                LL.setByPos(ll, n, y);

                hh = (H.getByPos(x + 1, y) - ((H.getByPos(x, y) + H.getByPos(x + 2,y)) >> 1))
                        >> 1;
                HH.setByPos(hh, n, y);
                lh = H.getByPos(x, y) + ((HH.getByPos(n - 1, y) + HH.getByPos(n, y)) >> 1);
                LH.setByPos(lh, n, y);
            }

            // right boundary, n = Size - 1
            x = n << 1;
            hl = (L.getByPos(x + 1, y) - ((L.getByPos(x, y) + L.getByPos(x, y)) >> 1)) >> 1;
            HL.setByPos(hl, n, y);
            ll = L.getByPos(x, y) + ((HL.getByPos(n - 1, y) + HL.getByPos(n, y)) >> 1);
            LL.setByPos(ll, n, y);

            hh = (H.getByPos(x + 1, y) - ((H.getByPos(x, y) + H.getByPos(x, y)) >> 1)) >> 1;
            HH.setByPos(hh, n, y);
            lh = H.getByPos(x, y) + ((HH.getByPos(n - 1, y) + HH.getByPos(n, y)) >> 1);
            LH.setByPos(lh, n, y);
        }
    }

    template<uint32_t Size>
    void decode2DVertical(RectArray<coefficient_type ,2 * Size, 2 * Size> & out, const DwtVertical<Size> & in)
    {
        auto & [L, H] = in;

        // Inverse vertical filtering, combines 2 sub bands to 1.
        constexpr size_t width = 2 * Size;
        for(size_t x = 0; x < width; ++x)
        {
            // left boundary
            coefficient_type value = L.getByPos(x, 0) - ((H.getByPos(x, 0) * 2 + 1) >> 1);
            out.setByPos(value, x, 0);

            for(size_t n = 1; n < Size; ++n)
            {
                const size_t y = n << 1;
                /* Even coefficients */
                value = L.getByPos(x, n) - ((H.getByPos(x, n - 1) + H.getByPos(x, n) + 1) >> 1);
                out.setByPos(value, x, y);

                /* Odd coefficients */
                value = (H.getByPos(x, n) << 1) + ((out.getByPos(x, y - 2) + out.getByPos(x, y)) >> 1);
                out.setByPos(value, x, y - 1);
            }

            // right boundary
            size_t y = Size << 1;
            value = (H.getByPos(x, Size - 2) << 1) + ((out.getByPos(x, y - 2) * 2) >> 1);
            out.setByPos(value, x, y - 1);
        }
    }

    template<uint32_t Size>
    void decode2DHorizontal(DwtVertical<Size> & out, const DwtHorizontal<Size> & in)
    {
        auto & [LL, HL, LH, HH] = in;
        auto & [L, H] = out;

        // Inverse horizontal filtering, combine 4 sub bands into 2 sub bands.
        for(size_t y = 0; y < Size; ++y)
        {
            // Even coefficients
            // left boundary, n = 0
            coefficient_type l = LL.getByPos(0, y) - ((HL.getByPos(0, y) + HL.getByPos(0, y) + 1) >> 1);
            coefficient_type h = LH.getByPos(0, y) - ((HH.getByPos(0, y) + HH.getByPos(0, y) + 1) >> 1);
            L.setByPos(l, 0, y);
            H.setByPos(h, 0, y);

            for(size_t n = 1; n < Size; ++n)
            {
                const size_t x = n << 1;
                l = LL.getByPos(n, y) - ((HL.getByPos(n - 1, y) + HL.getByPos(n, y) + 1) >> 1);
                h = LH.getByPos(n, y) - ((HH.getByPos(n - 1, y) + HH.getByPos(n, y) + 1) >> 1);
                L.setByPos(l, x, y);
                H.setByPos(h, x, y);
            }

            /* Odd coefficients */
            size_t n = 0;
            for (n = 0; n < Size - 1; n++)
            {
                const size_t x = n << 1;
                l = (HL.getByPos(n, y) << 1) + ((L.getByPos(x, y) + L.getByPos(x + 2, y)) >> 1);
                h = (HH.getByPos(n, y) << 1) + ((H.getByPos(x, y) + H.getByPos(x + 2, y)) >> 1);
                L.setByPos(l, x + 1, y);
                H.setByPos(h, x + 1, y);
            }

            // right boundary, n = Size - 1
            const size_t x = n << 1;
            l = (HL.getByPos(n, y) << 1) + (L.getByPos(x, y));
            h = (HH.getByPos(n, y) << 1) + (H.getByPos(x, y));
            L.setByPos(l, x + 1, y);
            H.setByPos(h, x + 1, y);
        }
    }
};


#endif //RFXCPP_DWT_H
