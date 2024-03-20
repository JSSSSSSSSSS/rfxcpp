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
    :m_rgb(rgb)
    {
    }

    [[nodiscard]] uint8_t r() const
    {
        return (m_rgb >> 16) & 0xFF;
    }

    [[nodiscard]] uint8_t g() const
    {
        return (m_rgb >> 8) & 0xFF;
    }

    [[nodiscard]] uint8_t b() const
    {
        return (m_rgb) & 0xFF;
    }

private:
    uint32_t m_rgb;
};

class Color
{
public:
    enum color{ARGB, XRGB, RGB, BGRA, BGRX, BGR};
public:
    static ColorYUV RGBtoYUV(ColorRGB rgb);

};


#endif //RFXCPP_COLOR_H
