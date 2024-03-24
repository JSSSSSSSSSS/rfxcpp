//
// Created by Administrator on 2024/3/25.
//

#ifndef RFXCPP_BITSTREAM_H
#define RFXCPP_BITSTREAM_H
#include <cstdint>
#include <cassert>

typedef struct
{
    const uint8_t* buffer;
    uint8_t* pointer;
    uint32_t position;
    uint32_t length;
    uint32_t capacity;
    uint32_t mask;
    uint32_t offset;
    uint32_t prefetch;
    uint32_t accumulator;
} wBitStream;

static inline void BitStream_Prefetch(wBitStream* _bs)
{
    assert(_bs);

    (_bs->prefetch) = 0;
    if (((uint32_t)(_bs->pointer - _bs->buffer) + 4) < (_bs->capacity))
        (_bs->prefetch) |= ((uint32_t) * (_bs->pointer + 4) << 24);
    if (((uint32_t)(_bs->pointer - _bs->buffer) + 5) < (_bs->capacity))
        (_bs->prefetch) |= ((uint32_t) * (_bs->pointer + 5) << 16);
    if (((uint32_t)(_bs->pointer - _bs->buffer) + 6) < (_bs->capacity))
        (_bs->prefetch) |= ((uint32_t) * (_bs->pointer + 6) << 8);
    if (((uint32_t)(_bs->pointer - _bs->buffer) + 7) < (_bs->capacity))
        (_bs->prefetch) |= ((uint32_t) * (_bs->pointer + 7) << 0);
}

static inline void BitStream_Fetch(wBitStream* _bs)
{
    assert(_bs);
    (_bs->accumulator) = 0;
    if (((uint32_t)(_bs->pointer - _bs->buffer) + 0) < (_bs->capacity))
        (_bs->accumulator) |= ((uint32_t) * (_bs->pointer + 0) << 24);
    if (((uint32_t)(_bs->pointer - _bs->buffer) + 1) < (_bs->capacity))
        (_bs->accumulator) |= ((uint32_t) * (_bs->pointer + 1) << 16);
    if (((uint32_t)(_bs->pointer - _bs->buffer) + 2) < (_bs->capacity))
        (_bs->accumulator) |= ((uint32_t) * (_bs->pointer + 2) << 8);
    if (((uint32_t)(_bs->pointer - _bs->buffer) + 3) < (_bs->capacity))
        (_bs->accumulator) |= ((uint32_t) * (_bs->pointer + 3) << 0);
    BitStream_Prefetch(_bs);
}

static inline void BitStream_Flush(wBitStream* _bs)
{
    assert(_bs);
    if (((uint32_t)(_bs->pointer - _bs->buffer) + 0) < (_bs->capacity))
        *(_bs->pointer + 0) = (uint8_t)((uint32_t)_bs->accumulator >> 24);
    if (((uint32_t)(_bs->pointer - _bs->buffer) + 1) < (_bs->capacity))
        *(_bs->pointer + 1) = (uint8_t)((uint32_t)_bs->accumulator >> 16);
    if (((uint32_t)(_bs->pointer - _bs->buffer) + 2) < (_bs->capacity))
        *(_bs->pointer + 2) = (uint8_t)((uint32_t)_bs->accumulator >> 8);
    if (((uint32_t)(_bs->pointer - _bs->buffer) + 3) < (_bs->capacity))
        *(_bs->pointer + 3) = (uint8_t)((uint32_t)_bs->accumulator >> 0);
}

static inline void BitStream_Shift(wBitStream* _bs, uint32_t _nbits)
{
    assert(_bs);
    if (_nbits == 0)
    {
    }
    else if ((_nbits > 0) && (_nbits < 32))
    {
        _bs->accumulator <<= _nbits;
        _bs->position += _nbits;
        _bs->offset += _nbits;
        if (_bs->offset < 32)
        {
            _bs->mask = (uint32_t)((1UL << _nbits) - 1UL);
            _bs->accumulator |= ((_bs->prefetch >> (32 - _nbits)) & _bs->mask);
            _bs->prefetch <<= _nbits;
        }
        else
        {
            _bs->mask = (uint32_t)((1UL << _nbits) - 1UL);
            _bs->accumulator |= ((_bs->prefetch >> (32 - _nbits)) & _bs->mask);
            _bs->prefetch <<= _nbits;
            _bs->offset -= 32;
            _bs->pointer += 4;
            BitStream_Prefetch(_bs);
            if (_bs->offset)
            {
                _bs->mask = (uint32_t)((1UL << _bs->offset) - 1UL);
                _bs->accumulator |= ((_bs->prefetch >> (32 - _bs->offset)) & _bs->mask);
                _bs->prefetch <<= _bs->offset;
            }
        }
    }
    else
    {
        // log here.
    }
}

static inline void BitStream_Shift32(wBitStream* _bs)
{
    assert(_bs);
    BitStream_Shift(_bs, 16);
    BitStream_Shift(_bs, 16);
}

static inline void BitStream_Write_Bits(wBitStream* _bs, uint32_t _bits, uint32_t _nbits)
{
    assert(_bs);
    _bs->position += _nbits;
    _bs->offset += _nbits;
    if (_bs->offset < 32)
    {
        _bs->accumulator |= (_bits << (32 - _bs->offset));
    }
    else
    {
        _bs->offset -= 32;
        _bs->mask = ((1 << (_nbits - _bs->offset)) - 1);
        _bs->accumulator |= ((_bits >> _bs->offset) & _bs->mask);
        BitStream_Flush(_bs);
        _bs->accumulator = 0;
        _bs->pointer += 4;
        if (_bs->offset)
        {
            _bs->mask = (uint32_t)((1UL << _bs->offset) - 1);
            _bs->accumulator |= ((_bits & _bs->mask) << (32 - _bs->offset));
        }
    }
}

static inline size_t BitStream_GetRemainingLength(wBitStream* _bs)
{
    assert(_bs);
    return (_bs->length - _bs->position);
}

void BitStream_Attach(wBitStream* bs, const uint8_t* buffer, uint32_t capacity);

#endif //RFXCPP_BITSTREAM_H
