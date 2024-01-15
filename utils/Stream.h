//
// Created by Administrator on 2023/11/12.
//

#ifndef NET_BYTESTREAM_H
#define NET_BYTESTREAM_H

#include <string>
#include <cstring>
#include <span>
#include <cstdint>

class Stream
{
public:
    explicit Stream(const std::span<uint8_t> &in)
        : m_data(in), m_iter(in.begin())
    {
    }

    ~Stream() = default;
public:
    [[nodiscard]] bool EnsureRemain(size_t size) const
    {
        return m_iter + (long long)size <= m_data.end();
    }

    [[nodiscard]] bool Eof() const
    {
        return m_iter == m_data.end();
    }

    [[nodiscard]]std::span<uint8_t> RawData() const
    {
        return m_data;
    }

    [[nodiscard]] virtual size_t SizeBytes() const
    {
        return m_data.size_bytes();
    }

protected:
    const std::span<uint8_t> m_data;
    std::span<uint8_t>::iterator m_iter;
};

class IStream:public Stream
{
public:
    explicit IStream(const std::span<uint8_t> &in)
            : Stream(in)
    {
    }

    ~IStream() = default;
public:
    template<typename T>
    size_t Skip(T & v)
    {
        constexpr size_t valueSize = sizeof(T);

        if (EnsureRemain(valueSize))
        {
            return 0;
        }

        m_iter += valueSize;
        return valueSize;
    }

    template<typename T>
    size_t Peek(T & v, bool is_remove = false)
    {
        constexpr size_t valueSize = sizeof(T);
        if (EnsureRemain(valueSize))
        {
            // no enough data
            return 0;
        }

        memcpy(&v, &(*m_iter), valueSize);
        if (is_remove)
        {
            m_iter += valueSize;
        }
        return valueSize;
    }

    template<typename T>
    size_t Read(T & v)
    {
        constexpr size_t valueSize = sizeof(T);
        if (EnsureRemain(valueSize))
        {
            // no enough data
            return 0;
        }

        memcpy(&v, &(*m_iter), valueSize);
        m_iter += valueSize;
        return valueSize;
    }
};

class OStream:public Stream
{
public:
    explicit OStream(const std::span<uint8_t> &in)
            : Stream(in)
    {
    }

    ~OStream() = default;
public:
    template<typename T>
    size_t FillWith(const T & v, size_t nums)
    {
        constexpr size_t valueSize = sizeof(T);

        for(size_t i = 0; i < nums; ++i)
        {
            memcpy(&(*m_iter), &v, valueSize);
            m_iter += (long long)valueSize;
        }
        return valueSize * nums;
    }

    template<typename T>
    size_t Write(const T & v)
    {
        constexpr size_t valueSize = sizeof(T);

        memcpy(&(*m_iter), &v, valueSize);
        m_iter += (long long)valueSize;
        return valueSize;
    }

    size_t Write(const char* & v)
    {
        if (!v)
        {
            return 0;
        }

        size_t valueSize = strlen(v);

        memcpy(&(*m_iter), &v, valueSize);
        m_iter += (long long)valueSize;
        return valueSize;
    }

    [[nodiscard]] size_t SizeBytes() const override
    {
        return m_iter - m_data.begin();
    }
};

#endif //NET_BYTESTREAM_H
