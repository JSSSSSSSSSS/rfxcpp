//
// Created by Administrator on 2024/1/14.
//

#ifndef RFXCPP_RFX_H
#define RFXCPP_RFX_H
#include <span>
#include <cstdint>
#include <memory>
#include <vector>
#include <bmp.h>

#include "Tile.h"
#include "Linearization.h"

class RfxMessage;
class Dwt;
class Quantization;
class Rlgr;

enum class OperationalMode {image, video};
enum class EntropyAlgorithm {RLGR1, RLGR3};

class Rfx
{
public:
    Rfx(uint32_t width, uint32_t height, uint32_t bpp);
    ~Rfx();
public:
    bool Encode(std::span<uint8_t> & out, const bmp & in, uint32_t frameIndex);
    bool Decode(bmp & out, const std::span<uint8_t> & in, uint32_t &frameIndex);
private:
    void SplitTiles(std::vector<Tile_rgb> & tiles, const std::span<const uint32_t>& rgb_data) const;

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
    std::unique_ptr<Dwt> m_dwt;
    std::unique_ptr<Quantization> m_quantization;
    std::unique_ptr<Linearization> linearization_;
    std::unique_ptr<Rlgr> m_rlgr;

    uint32_t m_pixFormat{};
    uint32_t m_bpp{};
    uint32_t m_width{};
    uint32_t m_height{};
    uint32_t m_frameIndex{};
    enum OperationalMode m_operationalMode;
    enum EntropyAlgorithm m_entropyAlgorithm;
};


#endif //RFXCPP_RFX_H
