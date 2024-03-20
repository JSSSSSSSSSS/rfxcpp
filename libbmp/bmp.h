//
// Created by Administrator on 2024/1/14.
//

#ifndef RFXCPP_BMP_H
#define RFXCPP_BMP_H
#include <span>
#include <cstdint>
#include <vector>

class bmp
{
public:
    [[nodiscard]] std::span<const uint32_t> GetRGBData() const
    {
        return m_rgbDate;
    }
private:
    std::vector<uint32_t> m_rgbDate;
};


#endif //RFXCPP_BMP_H
