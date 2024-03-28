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

template<typename T, size_t Size>
class RectArrayRef
{
public:
    T & operator[](size_t index)
    {
        return *array_.at(index);
    }

    T* & addressOf(size_t index)
    {
        return array_.at(index);
    }

    [[nodiscard]] constexpr size_t size() const
    {
        return Size;
    }
private:
    std::array<T*, Size> array_;
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

    void row(RectArrayRef<T, width> &row_ref, size_t index)
    {
        assert(index < height);
        for(size_t i = 0; i < width; ++i)
        {
            row_ref.addressOf(i) = &(this->at(index * width + i));
        }
    }

    void row(RectArrayRef<const T, width> &row_ref, size_t index) const
    {
        assert(index < height);
        for(size_t i = 0; i < width; ++i)
        {
            row_ref.addressOf(i) = &(this->at(index * width + i));
        }
    }

    void col(RectArrayRef<T, height> &col_ref, size_t index)
    {
        assert(index < width);
        for(size_t i = 0; i < height; ++i)
        {
            col_ref.addressOf(i) = &(this->at(i * width + index));
        }
    }

    void col(RectArrayRef<const T, height> &col_ref, size_t index) const
    {
        assert(index < width);
        for(size_t i = 0; i < height; ++i)
        {
            col_ref.addressOf(i) = &(this->at(i * width + index));
        }
    }
};


#endif //RFXCPP_RECTARRAY_H
