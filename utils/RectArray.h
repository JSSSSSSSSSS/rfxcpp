//
// Created by Administrator on 2024/3/13.
//

#ifndef RFXCPP_RECTARRAY_H
#define RFXCPP_RECTARRAY_H

#include <cstdint>
#include <array>
#include <span>
#include <cassert>
#include <stdexcept>

struct ViewRect
{
    uint32_t view_left;
    uint32_t view_top;
    uint32_t view_width;
    uint32_t view_height;
};

template<typename T, size_t width, size_t height>
class RectArray:public std::array<T, width * height>
{
    static_assert(width > 0 && width % 2 == 0, "Template parameter <width> must > 0 and be an even number");
    static_assert(height > 0 && height % 2 == 0, "Template parameter <height> must > 0 and be an even number");
    static constexpr size_t half_width = width / 2;
    static constexpr size_t half_height = height / 2;
public:
    using size_type = size_t;

public:
    void verticalBisect(RectArray<T, half_width, height> & left, RectArray<T, half_width, height> & right) const
    {
        constexpr size_t half_size = half_width * height;

        for(int i = 0; i < half_size; ++i)
        {
            size_t index_left = (i / half_width) * width + i % half_width;
            size_t index_right = index_left + half_width;
            left[i] = this->at(index_left);
            right[i] = this->at(index_right);
        }
    }

    void horizontalBisect(RectArray<T, width, half_height> & up, RectArray<T, width, half_height> & down) const
    {
        constexpr size_t half_size = width * half_height;
        for(int i = 0; i < half_size; ++i)
        {
            up[i] = this->at(i);
            down[i] = this->at(i + half_size);
        }
    }

    template<typename InputIterator,
            typename = std::_RequireInputIter<InputIterator>>
    void assign(size_t index, InputIterator first, InputIterator last)
    {
        assert(index + (last - first) <= this->size());
        for(auto it = first; it != last; ++it, ++index)
        {
            try
            {
                this->at(index) = *it;
            }
            catch(std::out_of_range & e)
            {
                e.what();
            }
        }
    }

    T getByPos(size_t x, size_t y) const
    {
        size_t index = y * width + x;
        T value;
        try
        {
            value = this->at(index);
        }
        catch(std::out_of_range & e)
        {
            e.what();
        }
        return value;
    }

    void setByPos(T value, size_t x, size_t y)
    {
        size_t index = y * width + x;
        try
        {
            this->at(index) = value;
        }
        catch(std::out_of_range & e)
        {
            e.what();
        }
    }
};


#endif //RFXCPP_RECTARRAY_H
