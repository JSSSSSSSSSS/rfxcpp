//
// Created by Administrator on 2024/1/14.
//

#include "Bitmap.h"
#include <cassert>
#include <fstream>
#include <iostream>

#pragma pack(push, 1)

struct BMPFileHeader
{
    char header[2];
    int size;
    short reserved1;
    short reserved2;
    int offset;
};

struct BMPInfoHeader
{
    int size;
    int width;
    int height;
    short planes;
    short bitCount;
    int compression;
    int imageSize;
    int xPixelsPerM;
    int yPixelsPerM;
    int colorsUsed;
    int importantColors;
};

#pragma pack(pop)

Bitmap::Bitmap()
    : width_(0), height_(0), format_(0), bytes_per_pixel_(0)
{

}

Bitmap::Bitmap(std::span<const uint32_t> rgbData, uint32_t width, uint32_t height, uint32_t format, uint32_t bpp)
{
    setRgbDate(rgbData, width, height, format, bpp);
}

void Bitmap::setRgbDate(std::span<const uint32_t> rgbData, uint32_t width, uint32_t height, uint32_t format, uint32_t bpp)
{
    assert(rgbData.size() == width * height);

    rgb_data_.clear();
    rgb_data_.assign(rgbData.begin(), rgbData.end());
    width_ = width;
    height_ = height;
    format_ = format;
    bytes_per_pixel_ = bpp / 8;
}

bool Bitmap::dump(const std::string &filename)
{
    bool ret = true;

    std::ofstream file(filename, std::ios::out | std::ios::binary);
    if (file.is_open())
    {
        int head_size = static_cast<int>(sizeof(BMPFileHeader) + sizeof(BMPInfoHeader));
        int body_size = static_cast<int>(width_ * height_ * sizeof(bytes_per_pixel_));
        BMPFileHeader fileHeader = {{'B', 'M'},
                                    head_size + body_size,
                                    0,
                                    0,
                                    head_size};
        BMPInfoHeader infoHeader = {sizeof(BMPInfoHeader),
                                    static_cast<int>(width_),
                                    static_cast<int>(height_),
                                    1,
                                    static_cast<short>(bytes_per_pixel_ * 8),
                                    0,
                                    body_size, 0, 0, 0, 0};

        file.write((const char*)&fileHeader, sizeof(BMPFileHeader));
        file.write((const char*)&infoHeader, sizeof(BMPInfoHeader));

        for (auto color: rgb_data_)
        {
            file.write((const char*)&color, bytes_per_pixel_);
        }

        file.close();
    }
    else
    {
        std::cerr << "Error: Unable to open file for writing." << std::endl;
        ret = false;
    }

    return ret;
}

bool Bitmap::load(const std::string &filename)
{
    bool ret = true;

    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (file.is_open())
    {
        rgb_data_.clear();
        BMPFileHeader fileHeader{};
        BMPInfoHeader infoHeader{};

        file.read((char*)&fileHeader, sizeof(BMPFileHeader));
        file.read((char*)&infoHeader, sizeof(BMPInfoHeader));

        int width = infoHeader.width;
        int height = infoHeader.height;
        int pixelSize = infoHeader.bitCount / 8;

        uint32_t pixel{};
        uint32_t size = width * height;
        for (int i = 0; i < size ; ++i)
        {
            pixel = 0;
            file.read((char*)&pixel, pixelSize);
            rgb_data_.push_back(pixel);
        }

        width_ = width;
        height_ = height;
        bytes_per_pixel_ = pixelSize;

        file.close();
    }
    else
    {
        std::cerr << "Error: Unable to open file for reading." << std::endl;
        ret = false;
    }

    return ret;
}
