//
// Created by Administrator on 2024/1/14.
//

#ifndef RFXCPP_RFX_H
#define RFXCPP_RFX_H
#include <span>
#include <cstdint>
#include <memory>

class RfxMessage;
class DWT;
class Quantization;
class RLGR;

class Rfx
{
public:
    Rfx();
    ~Rfx();
public:
    bool Encode(std::span<uint8_t> & out, const std::span<uint8_t> & in);
    bool Decode(std::span<uint8_t> & out, const std::span<uint8_t> & in);
private:
    void SetPixFormat(uint32_t pixFormat)
    {
        m_pixFormat = pixFormat;
        m_bpp = 32;
    }

    [[nodiscard]] uint32_t GetPixFormat() const
    {
        return m_pixFormat;
    }

private:
    std::unique_ptr<RfxMessage> m_rfxMessage;
    std::unique_ptr<DWT> m_dwt;
    std::unique_ptr<Quantization> m_quantization;
    std::unique_ptr<RLGR> m_rlgr;

    uint32_t m_pixFormat;
    uint32_t m_bpp;
    uint32_t width;
    uint32_t height;
    uint32_t frameIdx;

};


#endif //RFXCPP_RFX_H
