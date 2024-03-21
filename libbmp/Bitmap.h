//
// Created by Administrator on 2024/1/14.
//

#ifndef RFXCPP_BITMAP_H
#define RFXCPP_BITMAP_H
#include <span>
#include <cstdint>
#include <vector>
#include <string>

class Bitmap
{
public:
    explicit Bitmap(const std::string & filename);

    [[nodiscard]] std::span<const uint32_t> GetRGBData() const
    {
        return m_rgbDate;
    }

    [[nodiscard]] uint32_t getWidth() const
    {
        return width_;
    }

    [[nodiscard]] uint32_t getHeight() const
    {
        return height_;
    }
private:
    std::vector<uint32_t> m_rgbDate;
    uint32_t width_;
    uint32_t height_;
    uint32_t format_;
};


#endif //RFXCPP_BITMAP_H
