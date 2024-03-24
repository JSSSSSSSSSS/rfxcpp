//
// Created by Administrator on 2024/3/10.
//

#ifndef RFXCPP_COLOR_H
#define RFXCPP_COLOR_H
#include <span>
#include <cstdint>

struct ColorYUV
{
    int8_t y;
    int8_t u;
    int8_t v;
};

struct ColorRGB
{
    explicit ColorRGB(uint32_t rgb)
    : rgb_(rgb)
    {
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

class Color
{
public:
    enum color{ARGB, XRGB, RGB, BGRA, BGRX, BGR};
public:
    static ColorYUV RGBtoYUV(ColorRGB rgb);

};


#endif //RFXCPP_COLOR_H
