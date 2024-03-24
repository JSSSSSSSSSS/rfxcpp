//
// Created by Administrator on 2024/1/14.
//

#ifndef RFXCPP_RFX_H
#define RFXCPP_RFX_H
#include <span>
#include <cstdint>
#include <memory>
#include <vector>
#include <Bitmap.h>

#include "Tile.h"
#include "Linearization.h"

class RfxMessage;
class Dwt;
class Quantization;
class Rlgr;

enum class OperationalMode {ImageMode, VideoMode};
enum class EntropyAlgorithm {RLGR1, RLGR3};

class Rfx
{
public:
    Rfx(uint32_t width, uint32_t height, uint32_t bpp);
    ~Rfx();
public:
    bool encode(std::vector<uint8_t> & out, const Bitmap & in, uint32_t frameIndex);
    bool decode(Bitmap & out, const std::span<uint8_t> & in, uint32_t &frameIndex);
private:
    void SplitTiles(std::vector<Tile_rgb> & tiles, const std::span<const uint32_t>& rgb_data) const;
    void rebuildFrame(std::vector<uint32_t> & rgb_data, const std::vector<Tile_rgb> & tiles) const;
    static void composeToStream(std::vector<uint8_t> & output, const std::vector<TileYCbCr> & input);
    static void decomposeFromStream(std::vector<TileYCbCr> & output, const std::span<const uint8_t> input);

    void SetPixFormat(uint32_t format)
    {
        pix_format_ = format;
        bit_per_pixel_ = 32;
    }

    [[nodiscard]] uint32_t GetPixFormat() const
    {
        return pix_format_;
    }

private:
    std::unique_ptr<RfxMessage> rfxMessage_;
    std::unique_ptr<Dwt> dwt_codec_;
    std::unique_ptr<Quantization> quantization_codec_;
    std::unique_ptr<Linearization> linearization_codec_;
    std::unique_ptr<Rlgr> rlgr_codec_;

    uint32_t pix_format_{};
    uint32_t bit_per_pixel_{};
    uint32_t width_{};
    uint32_t height_{};
    uint32_t frame_index_{};
    enum OperationalMode operational_mode_;
    enum EntropyAlgorithm entropy_algorithm_;
};


#endif //RFXCPP_RFX_H
