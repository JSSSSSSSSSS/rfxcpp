//
// Created by Administrator on 2024/1/14.
//

#include "Rlgr.h"
#include "BitStream.h"

void Rlgr::encode(std::vector<uint8_t> & output, std::span<const int16_t> input)
{
    switch(mod_)
    {
        case RLGR_MODE::RLGR1:
            encodeRlgr1(output, input);
            break;
        case RLGR_MODE::RLGR3:
            encodeRlgr3(output, input);
            break;
        default:
            encodeRlgr1(output, input);
    }
}

void Rlgr::decode(std::vector<int16_t> & output, std::span<const uint8_t> input)
{
    switch(mod_)
    {
        case RLGR_MODE::RLGR1:
            decodeRlgr1(output, input);
            break;
        case RLGR_MODE::RLGR3:
            decodeRlgr3(output, input);
            break;
        default:
            decodeRlgr1(output, input);
    }
}

class BitStream
{
public:
    explicit BitStream(std::span<uint8_t> data)
    {
        buffer_ = data.data();
        bytes_ = data.size();
        byte_pos_ = 0;
        bits_left_ = 8;
    }

    [[nodiscard]] size_t getProcessedBytes() const
    {
        return (bits_left_ > 0 ? byte_pos_ + 1 : byte_pos_);
    }

    void putBits(int bits, int size)
    {
        int b;
        while (byte_pos_ < bytes_ && size > 0)
        {
            b = std::min(size, bits_left_);

            // Why this ?
            buffer_[byte_pos_] &= ~(((1 << b) - 1) << (bits_left_ - b));
            buffer_[byte_pos_] |= ((bits >> (size - b)) & ((1 << b) - 1)) << (bits_left_ - b);

            bits_left_ -= b;
            size -= b;

            if (bits_left_ == 0)
            {
                bits_left_ = 8;
                byte_pos_++;
            }
        }
    }
private:
    uint8_t *buffer_{};
    size_t bytes_{};
    int byte_pos_{};
    int bits_left_{};
};

/* Constants used within the RLGR1/RLGR3 algorithm */
static constexpr int KPMAX = 80;  /* max value for kp or krp */
static constexpr int LSGR  = 3;   /* shift count to convert kp to k */
static constexpr int UP_GR = 4;   /* increase in kp after a zero run in RL mode */
static constexpr int DN_GR = 6;   /* decrease in kp after a nonzero symbol in RL mode */
static constexpr int UQ_GR = 3;   /* increase in kp after nonzero symbol in GR mode */
static constexpr int DQ_GR = 3;   /* decrease in kp after zero symbol in GR mode */

/*
 * Update the passed parameter and clamp it to the range [0, KPMAX]
 * Return the value of parameter right-shifted by LSGR
 */
void UpdateParam(int & param, int _deltaP, int & k)
{
    param += _deltaP;
    param = std::min(param, KPMAX);
    param = std::max(param, 0);
    k = (param >> LSGR);
}

/* Returns the next coefficient (a signed int) to encode, from the input stream */
template<std::input_iterator It, std::sized_sentinel_for<It> End>
int GetNextInput(It & input_iter, End input_iter_end)
{
    int n = 0;
    if (input_iter != input_iter_end)
    {
        n = *input_iter++;
    }

    return n;
}

/* Emit bitPattern to the output bitstream */
#define OutputBits(_numBits, _bitPattern) bs.putBits(_bitPattern, _numBits)

/* Emit a bit (0 or 1), count number of times, to the output bitstream */
#define OutputBit(_count, _bit) \
do \
{   \
    uint16_t b = ((_bit) ? 0xFFFF : 0); \
    int c = _count; \
    for (; c > 0; c -= 16) \
    { \
        bs.putBits(b, (c > 16 ? 16 : c)); \
    } \
} while (0)

/* Converts the input value to (2 * abs(input) - sign(input)), where sign(input) = (input < 0 ? 1 : 0) and returns it */
#define Get2MagSign(_input) ((_input) >= 0 ? 2 * (_input) : -2 * (_input) - 1)

/* Outputs the Golomb/Rice encoding of a non-negative integer */
#define CodeGR(_krp, _val) \
do { \
    int lkr = (_krp) >> LSGR; \
    int lval = _val; \
    /* unary part of GR code */ \
    uint32_t lvk = lval >> lkr; \
    OutputBit(lvk, 1); \
    OutputBit(1, 0); \
    /* remainder part of GR code, if needed */ \
    if (lkr) \
    { \
        OutputBits(lkr, lval & ((1 << lkr) - 1)); \
    } \
    /* update krp, only if it is not equal to 1 */ \
    if (lvk == 0) \
    { \
        UpdateParam(_krp, -2, lkr); \
    } \
    else if (lvk > 1) \
    { \
        UpdateParam(_krp, lvk, lkr); \
    } \
} while (0)

void Rlgr::encodeRlgr1(std::vector<uint8_t> & output, std::span<const int16_t> input_plane)
{
    int k;
    int kp;
    int krp;

    int input;
    int numZeros;
    int runmax;
    int mag;
    int sign;
    int lmag;
    uint32_t twoMs;

    output.resize(tile_size * 2);
    BitStream bs{output};

    auto input_iter = input_plane.begin();
    auto input_iter_end = input_plane.end();

    /* initialize the parameters */
    k = 1;
    kp = 1 << LSGR;
    krp = 1 << LSGR;

    /* process all the input coefficients */
    while (input_iter < input_iter_end)
    {
        if (k)
        {
            /* RUN-LENGTH MODE */

            /* collect the run of zeros in the input stream */
            numZeros = 0;
            input = GetNextInput(input_iter, input_iter_end);
            while (input == 0 && input_iter < input_iter_end)
            {
                numZeros++;
                input = GetNextInput(input_iter, input_iter_end);
            }

            /* emit output zeros */
            runmax = 1 << k;
            while (numZeros >= runmax)
            {
                OutputBit(1, 0); /* output a zero bit */
                numZeros -= runmax;
                UpdateParam(kp, UP_GR, k); /* update kp, k */
                runmax = 1 << k;
            }

            /* output a 1 to terminate runs */
            OutputBit(1, 1);

            /* output the remaining run length using k bits */
            OutputBits(k, numZeros);

            /* note: when we reach here and the last byte being encoded is 0, we still
               need to output the last two bits, otherwise mstsc will crash */

            /* encode the nonzero value using GR coding */
            mag = (input < 0 ? -input : input); /* absolute value of input coefficient */
            sign = (input < 0 ? 1 : 0);  /* sign of input coefficient */

            OutputBit(1, sign); /* output the sign bit */
            lmag = mag ? mag - 1 : 0;
            CodeGR(krp, lmag); /* output GR code for (mag - 1) */

            UpdateParam(kp, -DN_GR, k);
        }
        else
        {
            /* GOLOMB-RICE MODE */

            /* RLGR1 variant */

            /* convert input to (2*magnitude - sign), encode using GR code */
            input = GetNextInput(input_iter, input_iter_end);
            twoMs = Get2MagSign(input);
            CodeGR(krp, twoMs);

            /* update k, kp */
            /* NOTE: as of Aug 2011, the algorithm is still wrongly documented
               and the update direction is reversed */
            if (twoMs)
            {
                UpdateParam(kp, -DQ_GR, k);
            }
            else
            {
                UpdateParam(kp, UQ_GR, k);
            }
        }
    }

    output.resize(bs.getProcessedBytes());
}

static inline uint32_t lzcnt(uint32_t x)
{
    unsigned y;
    int n = 32;
    y = x >> 16;
    if (y != 0)
    {
        n = n - 16;
        x = y;
    }
    y = x >> 8;
    if (y != 0)
    {
        n = n - 8;
        x = y;
    }
    y = x >> 4;
    if (y != 0)
    {
        n = n - 4;
        x = y;
    }
    y = x >> 2;
    if (y != 0)
    {
        n = n - 2;
        x = y;
    }
    y = x >> 1;
    if (y != 0)
        return n - 2;
    return n - x;
}

static inline uint32_t lzcnt_s(uint32_t x)
{
    if (!x)
        return 32;
    
    return lzcnt(x);
}

void Rlgr::decodeRlgr1(std::vector<int16_t> & output, std::span<const uint8_t> input)
{
    int vk = 0;
    size_t run = 0;
    int cnt = 0;
    size_t size = 0;
    int nbits = 0;
    size_t offset = 0;
    int16_t mag = 0;
    uint32_t k = 0;
    int32_t kp = 0;
    uint32_t kr = 0;
    int32_t krp = 0;
    uint16_t code = 0;
    uint32_t sign = 0;
    uint32_t nIdx = 0;
    uint32_t val1 = 0;
    uint32_t val2 = 0;
    int16_t * pOutput = nullptr;
    wBitStream* bs = nullptr;
    wBitStream s_bs = { nullptr };
    
    k = 1;
    kp = k << LSGR;

    kr = 1;
    krp = kr << LSGR;
    
    output.resize(tile_size);
    const size_t DstSize = tile_size;
    const size_t rDstSize = tile_size;
    
    int16_t * pDstData = output.data();
    pOutput = output.data();

    bs = &s_bs;

    BitStream_Attach(bs, input.data(), input.size());
    BitStream_Fetch(bs);

    while ((BitStream_GetRemainingLength(bs) > 0) && ((pOutput - pDstData) < DstSize))
    {
        if (k)
        {
            /* Run-Length (RL) Mode */

            run = 0;

            /* count number of leading 0s */

            cnt = lzcnt_s(bs->accumulator);

            nbits = BitStream_GetRemainingLength(bs);

            if (cnt > nbits)
                cnt = nbits;

            vk = cnt;

            while ((cnt == 32) && (BitStream_GetRemainingLength(bs) > 0))
            {
                BitStream_Shift32(bs);

                cnt = lzcnt_s(bs->accumulator);

                nbits = BitStream_GetRemainingLength(bs);

                if (cnt > nbits)
                    cnt = nbits;

                vk += cnt;
            }

            BitStream_Shift(bs, (vk % 32));

            if (BitStream_GetRemainingLength(bs) < 1)
                break;

            BitStream_Shift(bs, 1);

            while (vk--)
            {
                const uint32_t add = (1 << k); /* add (1 << k) to run length */
                run += add;

                /* update k, kp params */

                kp += UP_GR;

                if (kp > KPMAX)
                    kp = KPMAX;

                k = kp >> LSGR;
            }

            /* next k bits contain run length remainder */

            if (BitStream_GetRemainingLength(bs) < k)
                break;

            bs->mask = ((1 << k) - 1);
            run += ((bs->accumulator >> (32 - k)) & bs->mask);
            BitStream_Shift(bs, k);

            /* read sign bit */

            if (BitStream_GetRemainingLength(bs) < 1)
                break;

            sign = (bs->accumulator & 0x80000000) ? 1 : 0;
            BitStream_Shift(bs, 1);

            /* count number of leading 1s */

            cnt = lzcnt_s(~(bs->accumulator));

            nbits = BitStream_GetRemainingLength(bs);

            if (cnt > nbits)
                cnt = nbits;

            vk = cnt;

            while ((cnt == 32) && (BitStream_GetRemainingLength(bs) > 0))
            {
                BitStream_Shift32(bs);

                cnt = lzcnt_s(~(bs->accumulator));

                nbits = BitStream_GetRemainingLength(bs);

                if (cnt > nbits)
                    cnt = nbits;

                vk += cnt;
            }

            BitStream_Shift(bs, (vk % 32));

            if (BitStream_GetRemainingLength(bs) < 1)
                break;

            BitStream_Shift(bs, 1);

            /* next kr bits contain code remainder */

            if (BitStream_GetRemainingLength(bs) < kr)
                break;

            bs->mask = ((1 << kr) - 1);
            if (kr > 0)
                code = (uint16_t)((bs->accumulator >> (32 - kr)) & bs->mask);
            else
                code = 0;
            BitStream_Shift(bs, kr);

            /* add (vk << kr) to code */

            code |= (vk << kr);

            if (!vk)
            {
                /* update kr, krp params */

                krp -= 2;

                if (krp < 0)
                    krp = 0;

                kr = krp >> LSGR;
            }
            else if (vk != 1)
            {
                /* update kr, krp params */

                krp += vk;

                if (krp > KPMAX)
                    krp = KPMAX;

                kr = krp >> LSGR;
            }

            /* update k, kp params */

            kp -= DN_GR;

            if (kp < 0)
                kp = 0;

            k = kp >> LSGR;

            /* compute magnitude from code */

            if (sign)
                mag = ((int16_t)(code + 1)) * -1;
            else
                mag = (int16_t)(code + 1);

            /* write to output stream */

            offset = (pOutput - pDstData);
            size = run;

            if ((offset + size) > rDstSize)
                size = DstSize - offset;

            if (size)
            {
                memset(pOutput, 0, size * sizeof(int16_t));
                pOutput += size;
            }

            if ((pOutput - pDstData) < DstSize)
            {
                *pOutput = mag;
                pOutput++;
            }
        }
        else
        {
            /* Golomb-Rice (GR) Mode */

            /* count number of leading 1s */

            cnt = lzcnt_s(~(bs->accumulator));

            nbits = BitStream_GetRemainingLength(bs);

            if (cnt > nbits)
                cnt = nbits;

            vk = cnt;

            while ((cnt == 32) && (BitStream_GetRemainingLength(bs) > 0))
            {
                BitStream_Shift32(bs);

                cnt = lzcnt_s(~(bs->accumulator));

                nbits = BitStream_GetRemainingLength(bs);

                if (cnt > nbits)
                    cnt = nbits;

                vk += cnt;
            }

            BitStream_Shift(bs, (vk % 32));

            if (BitStream_GetRemainingLength(bs) < 1)
                break;

            BitStream_Shift(bs, 1);

            /* next kr bits contain code remainder */

            if (BitStream_GetRemainingLength(bs) < kr)
                break;

            bs->mask = ((1 << kr) - 1);
            if (kr > 0)
                code = (uint16_t)((bs->accumulator >> (32 - kr)) & bs->mask);
            else
                code = 0;
            BitStream_Shift(bs, kr);

            /* add (vk << kr) to code */

            code |= (vk << kr);

            if (!vk)
            {
                /* update kr, krp params */

                krp -= 2;

                if (krp < 0)
                    krp = 0;

                kr = krp >> LSGR;
            }
            else if (vk != 1)
            {
                /* update kr, krp params */

                krp += vk;

                if (krp > KPMAX)
                    krp = KPMAX;

                kr = krp >> LSGR;
            }

            if (!code)
            {
                /* update k, kp params */

                kp += UQ_GR;

                if (kp > KPMAX)
                    kp = KPMAX;

                k = kp >> LSGR;

                mag = 0;
            }
            else
            {
                /* update k, kp params */

                kp -= DQ_GR;

                if (kp < 0)
                    kp = 0;

                k = kp >> LSGR;

                /*
                 * code = 2 * mag - sign
                 * sign + code = 2 * mag
                 */

                if (code & 1)
                    mag = ((int16_t)((code + 1) >> 1)) * -1;
                else
                    mag = (int16_t)(code >> 1);
            }

            if ((pOutput - pDstData) < DstSize)
            {
                *pOutput = mag;
                pOutput++;
            }
        }
    }

    offset = (pOutput - pDstData);

    if (offset < rDstSize)
    {
        size = DstSize - offset;
        memset(pOutput, 0, size * 2);
        pOutput += size;
    }

    offset = (pOutput - pDstData);

    if (offset != DstSize)
        throw std::exception();
}

void Rlgr::encodeRlgr3(std::vector<uint8_t> &output, std::span<const int16_t> input)
{

}

void Rlgr::decodeRlgr3(std::vector<int16_t> & output, std::span<const uint8_t> input)
{

}

