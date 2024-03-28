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
    void encode(DwtSubBands & output, const DwtTileData & input);
    void decode(DwtTileData & output, const DwtSubBands & input);
private:
    template<uint32_t Size>
    void encode2DVertical(DwtVertical<Size> & out, const RectArray<coefficient_type ,2 * Size, 2 * Size> & in)
    {
        auto & [L_dst, H_dst] = out;

        // vertical filtering, for each col, generator two sub-bands.
        constexpr size_t width = 2 * Size;
        for(int x_index = 0; x_index < width; ++x_index)
        {
            RectArrayRef<const coefficient_type, 2 * Size> X{};
            in.col(X, x_index);

            RectArrayRef<coefficient_type, Size> H{};
            RectArrayRef<coefficient_type, Size> L{};
            H_dst.col(H, x_index);
            L_dst.col(L, x_index);

            // left boundary
            H[0] = (X[1] - ((X[0] + X[2]) >> 1 )) >> 1;
            L[0] = X[0] + H[0];

            int n = 1;
            int y = n << 1;
            for(n = 1; n < Size - 1; ++n)
            {
                y = n << 1;
                H[n] = (X[y + 1] - ((X[y] + X[y + 2]) >> 1)) >> 1;
                L[n] = X[y] + ((H[n - 1] + H[n]) >> 1);
            }

            // right boundary
            y = n << 1;
            H[n] = (X[y + 1] - ((X[y] + X[y]) >> 1)) >> 1;
            L[n] = X[y] + ((H[n - 1] + H[n]) >> 1);
        }
    }

    template<uint32_t Size>
    void encode2DHorizontal(DwtHorizontal<Size> & out, const DwtVertical<Size> & in)
    {
        auto & [LL_dst, HL_dst, LH_dst, HH_dst] = out;
        auto & [L_src, H_src] = in;

        // Horizontal filtering, for each row, generate two sub-bands.
        // There are two sub-bands input, generate four sub-bands.
        for(int y_index = 0; y_index < Size; ++y_index)
        {
            RectArrayRef<const coefficient_type, 2 * Size> L{};
            RectArrayRef<const coefficient_type, 2 * Size> H{};
            L_src.row(L, y_index);
            H_src.row(H, y_index);

            RectArrayRef<coefficient_type, Size> HL{};
            RectArrayRef<coefficient_type, Size> LL{};
            RectArrayRef<coefficient_type, Size> HH{};
            RectArrayRef<coefficient_type, Size> LH{};
            HL_dst.row(HL, y_index);
            LL_dst.row(LL, y_index);
            HH_dst.row(HH, y_index);
            LH_dst.row(LH, y_index);

            // left boundary, n = 0
            HL[0] = (L[1] - ((L[0] + L[2]) >> 1)) >> 1;
            LL[0] = L[0] + HL[0];

            HH[0] = (H[1] - ((H[0] + H[2]) >> 1)) >> 1;;
            LH[0] = H[0] + HH[0];

            int x = 0;
            int n = 1;
            for(n = 1; n < Size - 1; ++n)
            {
                x = n << 1;
                HL[n] = (L[x + 1] - ((L[x] + L[x + 2]) >> 1)) >> 1;
                LL[n] = L[x] + ((HL[n - 1] + HL[n]) >> 1);

                HH[n] = (H[x + 1] - ((H[x] + H[x + 2]) >> 1)) >> 1;
                LH[n] = H[x] + ((HH[n - 1] + HH[n]) >> 1);
            }

            // right boundary, n = Size - 1
            x = n << 1;
            HL[n] = (L[x + 1] - ((L[x] + L[x]) >> 1)) >> 1;
            LL[n] = L[x] + ((HL[n - 1] + HL[n]) >> 1);

            HH[n] = (H[x + 1] - ((H[x] + H[x]) >> 1)) >> 1;
            LH[n] = H[x] + ((HH[n - 1] + HH[n]) >> 1);
        }
    }

    template<uint32_t Size>
    void decode2DVertical(RectArray<coefficient_type ,2 * Size, 2 * Size> & out, const DwtVertical<Size> & in)
    {
        auto & [L_src, H_src] = in;

        // Inverse vertical filtering, combines 2 sub bands to 1.
        constexpr size_t width = 2 * Size;
        for(size_t x_index = 0; x_index < width; ++x_index)
        {
            RectArrayRef<const coefficient_type, Size> L{};
            RectArrayRef<const coefficient_type, Size> H{};
            L_src.col(L, x_index);
            H_src.col(H, x_index);

            RectArrayRef<coefficient_type, 2 * Size> X{};
            out.col(X, x_index);

            // left boundary
            X[0] = L[0] - ((H[0] * 2 + 1) >> 1);

            for(size_t n = 1; n < Size; ++n)
            {
                const size_t y = n << 1;
                /* Even coefficients */
                X[y] = L[n] - ((H[n - 1] + H[n] + 1) >> 1);

                /* Odd coefficients */
                X[y - 1] = (H[n] << 1) + ((X[y - 2] + X[y]) >> 1);
            }

            // right boundary
            size_t y = Size << 1;
            X[y - 1] = (H[Size - 2] << 1) + ((X[y - 2] * 2) >> 1);
        }
    }

    template<uint32_t Size>
    void decode2DHorizontal(DwtVertical<Size> & out, const DwtHorizontal<Size> & in)
    {
        auto & [LL_src, HL_src, LH_src, HH_src] = in;
        auto & [L_dst, H_dst] = out;

        // Inverse horizontal filtering, combine 4 sub bands into 2 sub bands.
        for(size_t y_index = 0; y_index < Size; ++y_index)
        {
            RectArrayRef<coefficient_type, 2 * Size> L{};
            RectArrayRef<coefficient_type, 2 * Size> H{};
            L_dst.row(L, y_index);
            H_dst.row(H, y_index);

            RectArrayRef<const coefficient_type, Size> HL{};
            RectArrayRef<const coefficient_type, Size> LL{};
            RectArrayRef<const coefficient_type, Size> HH{};
            RectArrayRef<const coefficient_type, Size> LH{};
            HL_src.row(HL, y_index);
            LL_src.row(LL, y_index);
            HH_src.row(HH, y_index);
            LH_src.row(LH, y_index);

            // Even coefficients
            // left boundary, n = 0
            L[0] = LL[0] - ((HL[0] + HL[0] + 1) >> 1);
            H[0] = LH[0] - ((HH[0] + HH[0] + 1) >> 1);

            for(size_t n = 1; n < Size; ++n)
            {
                const size_t x = n << 1;
                L[x] = LL[n] - ((HL[n - 1] + HL[n] + 1) >> 1);
                H[x] = LH[n] - ((HH[n - 1] + HH[n] + 1) >> 1);
            }

            /* Odd coefficients */
            size_t n = 0;
            for (n = 0; n < Size - 1; n++)
            {
                const size_t x = n << 1;
                L[x + 1] = (HL[n] << 1) + ((L[x] + L[x + 2]) >> 1);
                H[x + 1] = (HH[n] << 1) + ((H[x] + H[x + 2]) >> 1);
            }

            // right boundary, n = Size - 1
            const size_t x = n << 1;
            L[x + 1] = (HL[n] << 1) + (L[x]);
            H[x + 1] = (HH[n] << 1) + (H[x]);
        }
    }
};


#endif //RFXCPP_DWT_H
