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
    template <uint32_t SIZE>
    using CoefficientRef = RectArrayRef<coefficient_type,SIZE>;
    template <uint32_t SIZE>
    using ConstCoefficientRef = RectArrayRef<const coefficient_type, SIZE>;
public:
    void encode(DwtSubBands & output, const DwtTileData & input);
    void decode(DwtTileData & output, const DwtSubBands & input);
private:
    template<uint32_t SIZE>
    void encode2DVertical(DwtVertical<SIZE> & out, const RectArray<coefficient_type , 2 * SIZE, 2 * SIZE> & in)
    {
        auto & [L_dst, H_dst] = out;

        // vertical filtering, for each col, generator two sub-bands.
        constexpr size_t width = 2 * SIZE;
        for(size_t x_index = 0; x_index < width; ++x_index)
        {
            ConstCoefficientRef<2 * SIZE> X{};
            in.col(X, x_index);

            CoefficientRef<SIZE> H{};
            CoefficientRef<SIZE> L{};
            H_dst.col(H, x_index);
            L_dst.col(L, x_index);

            // left boundary
            H[0] = (X[1] - ((X[0] + X[2]) >> 1 )) >> 1;
            L[0] = X[0] + H[0];

            size_t n = 1;
            for(n = 1; n < SIZE - 1; ++n)
            {
                size_t _2N = n << 1;
                H[n] = (X[_2N + 1] - ((X[_2N] + X[_2N + 2]) >> 1)) >> 1;
                L[n] = X[_2N] + ((H[n - 1] + H[n]) >> 1);
            }

            // right boundary
            size_t _2N = n << 1;
            H[n] = (X[_2N + 1] - ((X[_2N] + X[_2N]) >> 1)) >> 1;
            L[n] = X[_2N] + ((H[n - 1] + H[n]) >> 1);
        }
    }

    template<uint32_t SIZE>
    void encode2DHorizontal(DwtHorizontal<SIZE> & out, const DwtVertical<SIZE> & in)
    {
        auto & [LL_dst, HL_dst, LH_dst, HH_dst] = out;
        auto & [L_src, H_src] = in;

        // Horizontal filtering, for each row, generate two sub-bands.
        // There are two sub-bands input, generate four sub-bands.
        for(int y_index = 0; y_index < SIZE; ++y_index)
        {
            ConstCoefficientRef<2 * SIZE> L{};
            ConstCoefficientRef<2 * SIZE> H{};
            L_src.row(L, y_index);
            H_src.row(H, y_index);

            CoefficientRef<SIZE> HL{};
            CoefficientRef<SIZE> LL{};
            CoefficientRef<SIZE> HH{};
            CoefficientRef<SIZE> LH{};
            HL_dst.row(HL, y_index);
            LL_dst.row(LL, y_index);
            HH_dst.row(HH, y_index);
            LH_dst.row(LH, y_index);

            // left boundary, n = 0
            HL[0] = (L[1] - ((L[0] + L[2]) >> 1)) >> 1;
            LL[0] = L[0] + HL[0];

            HH[0] = (H[1] - ((H[0] + H[2]) >> 1)) >> 1;;
            LH[0] = H[0] + HH[0];

            size_t n = 1;
            for(n = 1; n < SIZE - 1; ++n)
            {
                size_t _2N = n << 1;
                HL[n] = (L[_2N + 1] - ((L[_2N] + L[_2N + 2]) >> 1)) >> 1;
                LL[n] = L[_2N] + ((HL[n - 1] + HL[n]) >> 1);

                HH[n] = (H[_2N + 1] - ((H[_2N] + H[_2N + 2]) >> 1)) >> 1;
                LH[n] = H[_2N] + ((HH[n - 1] + HH[n]) >> 1);
            }

            // right boundary, n = SIZE - 1
            size_t _2N = n << 1;
            HL[n] = (L[_2N + 1] - ((L[_2N] + L[_2N]) >> 1)) >> 1;
            LL[n] = L[_2N] + ((HL[n - 1] + HL[n]) >> 1);

            HH[n] = (H[_2N + 1] - ((H[_2N] + H[_2N]) >> 1)) >> 1;
            LH[n] = H[_2N] + ((HH[n - 1] + HH[n]) >> 1);
        }
    }

    template<uint32_t SIZE>
    void decode2DVertical(RectArray<coefficient_type , 2 * SIZE, 2 * SIZE> & out, const DwtVertical<SIZE> & in)
    {
        auto & [L_src, H_src] = in;

        // Inverse vertical filtering, combines 2 sub bands to 1.
        constexpr size_t width = 2 * SIZE;
        for(size_t x_index = 0; x_index < width; ++x_index)
        {
            ConstCoefficientRef<SIZE> L{};
            ConstCoefficientRef<SIZE> H{};
            L_src.col(L, x_index);
            H_src.col(H, x_index);

            CoefficientRef<2 * SIZE> X{};
            out.col(X, x_index);

            // left boundary
            /* Even coefficients */
            X[0] = L[0] - ((H[1] + H[0] + 1) >> 1);

            for(size_t n = 1; n < SIZE; ++n)
            {
                const size_t _2N = n << 1;
                /* Even coefficients */
                X[_2N] = L[n] - ((H[n - 1] + H[n] + 1) >> 1);

                /* Odd coefficients */
                X[_2N - 1] = (H[n - 1] << 1) + ((X[_2N - 2] + X[_2N]) >> 1);
            }

            // right boundary, n = SIZE
            /* Odd coefficients */
            size_t _2N = SIZE << 1;
            X[_2N - 1] = (H[SIZE - 1] << 1) + (X[_2N - 2]);
        }
    }

    template<uint32_t SIZE>
    void decode2DHorizontal(DwtVertical<SIZE> & out, const DwtHorizontal<SIZE> & in)
    {
        auto & [LL_src, HL_src, LH_src, HH_src] = in;
        auto & [L_dst, H_dst] = out;

        // Inverse horizontal filtering, combine 4 sub bands into 2 sub bands.
        for(size_t y_index = 0; y_index < SIZE; ++y_index)
        {
            CoefficientRef<2 * SIZE> L{};
            CoefficientRef<2 * SIZE> H{};
            L_dst.row(L, y_index);
            H_dst.row(H, y_index);

            ConstCoefficientRef<SIZE> HL{};
            ConstCoefficientRef<SIZE> LL{};
            ConstCoefficientRef<SIZE> HH{};
            ConstCoefficientRef<SIZE> LH{};
            HL_src.row(HL, y_index);
            LL_src.row(LL, y_index);
            HH_src.row(HH, y_index);
            LH_src.row(LH, y_index);

            // Even coefficients
            // left boundary, n = 0
            L[0] = LL[0] - ((HL[1] + HL[0] + 1) >> 1);
            H[0] = LH[0] - ((HH[1] + HH[0] + 1) >> 1);

            for(size_t n = 1; n < SIZE; ++n)
            {
                const size_t _2N = n << 1;
                L[_2N] = LL[n] - ((HL[n - 1] + HL[n] + 1) >> 1);
                H[_2N] = LH[n] - ((HH[n - 1] + HH[n] + 1) >> 1);
            }

            /* Odd coefficients */
            size_t n = 0;
            for (n = 0; n < SIZE - 1; n++)
            {
                const size_t _2N = n << 1;
                L[_2N + 1] = (HL[n] << 1) + ((L[_2N] + L[_2N + 2]) >> 1);
                H[_2N + 1] = (HH[n] << 1) + ((H[_2N] + H[_2N + 2]) >> 1);
            }

            // Odd coefficients
            // right boundary, n = SIZE - 1
            const size_t _2N = n << 1;
            L[_2N + 1] = (HL[n] << 1) + (L[_2N]);
            H[_2N + 1] = (HH[n] << 1) + (H[_2N]);
        }
    }
};


#endif //RFXCPP_DWT_H
