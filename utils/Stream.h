//
// Created by Administrator on 2023/11/12.
//

#ifndef NET_BYTESTREAM_H
#define NET_BYTESTREAM_H

#include <string>
#include <cstring>
#include <span>
#include <cstdint>
#include <utility>
#include <vector>
#include <concepts>

class IStream
{
public:
    explicit IStream(std::span<const uint8_t> in)
        :data_(in), iter_(data_.begin())
    {
        iter_ = data_.begin();
    }

    ~IStream() = default;
public:
    template<typename T>
    size_t skip(T & v)
    {
        constexpr size_t valueSize = sizeof(T);

        if (ensureRemain(valueSize))
        {
            return 0;
        }

        iter_ += valueSize;
        return valueSize;
    }

    template<typename T>
    size_t peek(T & v, bool is_remove = false)
    {
        constexpr size_t valueSize = sizeof(T);
        if (ensureRemain(valueSize))
        {
            // no enough data
            return 0;
        }

        memcpy(&v, &(*iter_), valueSize);
        if (is_remove)
        {
            iter_ += valueSize;
        }
        return valueSize;
    }

    template<typename T>
    size_t read(T & v)
    {
        constexpr size_t valueSize = sizeof(T);
        if (!ensureRemain(valueSize))
        {
            // no enough data
            return 0;
        }

        memcpy(&v, &(*iter_), valueSize);
        iter_ += valueSize;
        return valueSize;
    }

    template<std::output_iterator<uint8_t> Out>
    size_t read(Out v, size_t n)
    {
        for(int i = 0; i < n; ++i)
        {
            *v = *iter_;
            ++v;
            ++iter_;
        }
        return n;
    }

    [[nodiscard]] bool ensureRemain(size_t size) const
    {
        return data_.end() - iter_ >= size;
    }

    [[nodiscard]] size_t remain() const
    {
        return data_.end() - iter_;
    }

    [[nodiscard]] size_t size() const
    {
        return data_.size();
    }

    [[nodiscard]] bool eof() const
    {
        return iter_ == data_.end();
    }
private:
    std::span<const uint8_t> data_;
    std::span<const uint8_t>::iterator iter_;
};

class OStream
{
public:
    explicit OStream(size_t init_size)
    {
        data_.resize(init_size);
        iter_ = data_.begin();
    }

    ~OStream() = default;
public:
    template<typename T>
    size_t fillWith(const T & v, size_t nums)
    {
        constexpr size_t valueSize = sizeof(T);

        for(size_t i = 0; i < nums; ++i)
        {
            memcpy(&(*iter_), &v, valueSize);
            iter_ += (long long)valueSize;
        }
        return valueSize * nums;
    }

    template<typename T>
    size_t write(const T & v)
    {
        constexpr size_t valueSize = sizeof(T);

        memcpy(&(*iter_), &v, valueSize);
        iter_ += (long long)valueSize;

        return valueSize;
    }

    size_t write(const char* & v)
    {
        if (!v)
        {
            return 0;
        }

        size_t valueSize = strlen(v);
        iter_ = data_.insert(iter_, v, v + valueSize);
        iter_+= static_cast<long long>(valueSize);

        return valueSize;
    }

    template<std::ranges::sized_range RNG>
    size_t write(const RNG & v) requires std::is_same_v<uint8_t, std::ranges::range_value_t<RNG>>
    {
        size_t size = v.size();
        iter_ = data_.insert(iter_, v.begin(), v.end());
        iter_+= static_cast<long long>(size);
        return size;
    }

    [[nodiscard]] size_t size() const
    {
        return iter_ - data_.begin();
    }

    [[nodiscard]] bool ensureRemain(size_t size)
    {
        if (iter_ + (long long)size >= data_.end())
        {
            data_.resize(data_.size() + size);
        }
        return true;
    }

    [[nodiscard]]std::span<const uint8_t> data() const
    {
        std::span<const uint8_t> data_raw(data_);
        return data_raw.subspan(0, size());
    }
private:
    std::vector<uint8_t> data_;
    std::vector<uint8_t>::iterator iter_;
};

#endif //NET_BYTESTREAM_H
