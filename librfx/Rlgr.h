//
// Created by Administrator on 2024/1/14.
//

#ifndef RFXCPP_RLGR_H
#define RFXCPP_RLGR_H

#include "Tile.h"

enum class RLGR_MODE {
    RLGR1,
    RLGR3
};

class Rlgr
{
public:
    explicit Rlgr(RLGR_MODE mod)
    : mod_(mod)
    {}
public:
    void encode(std::vector<uint8_t> & output, std::span<const int16_t> input);
    void decode(std::vector<int16_t> & output, std::span<const uint8_t> input);
private:
    static void encodeRlgr1(std::vector<uint8_t> & output, std::span<const int16_t> input);
    static void encodeRlgr3(std::vector<uint8_t> & output, std::span<const int16_t> input);
    static void decodeRlgr1(std::vector<int16_t> & output, std::span<const uint8_t> input);
    static void decodeRlgr3(std::vector<int16_t> & output, std::span<const uint8_t> input);
private:
    RLGR_MODE mod_;
};


#endif //RFXCPP_RLGR_H
