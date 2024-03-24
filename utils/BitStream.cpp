//
// Created by Administrator on 2024/3/25.
//

#include "BitStream.h"

void BitStream_Attach(wBitStream* bs, const uint8_t * buffer, uint32_t capacity)
{
    union
    {
        const uint8_t* cpv;
        uint8_t* pv;
    } cnv{};

    assert(bs);
    assert(buffer);

    cnv.cpv = buffer;

    bs->position = 0;
    bs->buffer = cnv.pv;
    bs->offset = 0;
    bs->accumulator = 0;
    bs->pointer = cnv.pv;
    bs->capacity = capacity;
    bs->length = bs->capacity * 8;
}
