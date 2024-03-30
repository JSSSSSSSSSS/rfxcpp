//
// Created by Administrator on 2024/3/10.
//

#ifndef RFXCPP_COLOR_H
#define RFXCPP_COLOR_H
#include <span>
#include <cstdint>

struct ColorARGB
{
    explicit ColorARGB(uint32_t rgb)
    : rgb_(rgb)
    {
    }

    ColorARGB(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
            : rgb_((a << 24) + (r << 16) + (g << 8) + b)
    {
    }

    explicit operator uint32_t() const
    {
        return rgb_;
    }

    [[nodiscard]] uint8_t a() const
    {
        return (rgb_ >> 24) & 0xFF;
    }

    [[nodiscard]] uint8_t r() const
    {
        return (rgb_ >> 16) & 0xFF;
    }

    [[nodiscard]] uint8_t g() const
    {
        return (rgb_ >> 8) & 0xFF;
    }

    [[nodiscard]] uint8_t b() const
    {
        return (rgb_) & 0xFF;
    }

private:
    uint32_t rgb_;
};


#endif //RFXCPP_COLOR_H
