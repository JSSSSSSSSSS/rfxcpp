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
    explicit Bitmap();
    Bitmap(std::span<const uint32_t> rgbData, uint32_t width, uint32_t height, uint32_t format, uint32_t bpp);

    bool load(const std::string & filename);
    bool dump(const std::string & filename);

    void setRgbDate(std::span<const uint32_t> rgbData, uint32_t width, uint32_t height, uint32_t format, uint32_t bpp);

    [[nodiscard]] std::span<const uint32_t> getRgbData() const
    {
        return rgb_data_;
    }

    [[nodiscard]] uint32_t getWidth() const
    {
        return width_;
    }

    [[nodiscard]] uint32_t getHeight() const
    {
        return height_;
    }

    [[nodiscard]] uint32_t getBytesPerPixel() const
    {
        return bytes_per_pixel_;
    }
private:
    std::vector<uint32_t> rgb_data_;
    uint32_t width_;
    uint32_t height_;
    uint32_t format_;
    uint32_t bytes_per_pixel_;
};


#endif //RFXCPP_BITMAP_H
