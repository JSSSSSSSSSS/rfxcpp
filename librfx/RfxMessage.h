//
// Created by Administrator on 2024/1/14.
//

#ifndef RFXCPP_RFXMESSAGE_H
#define RFXCPP_RFXMESSAGE_H
#include <cstdint>
#include <vector>
#include "Stream.h"

constexpr uint32_t WF_MAGIC {0xCACCACCA};
constexpr uint16_t WF_VERSION_1_0 {0x0100};
constexpr uint16_t CT_TILE_64x64 {0x0040};
constexpr uint8_t CODEC_MODE {0x02};
constexpr uint8_t COL_CONV_ICT {0x1};
constexpr uint8_t CLW_XFORM_DWT_53_A {0x1};
constexpr uint8_t SCALAR_QUANTIZATION {0x1};

enum TS_RFX_BLOCK_TYPE
{
    CBT_REGION = 0xCAC1,
    CBT_TILESET = 0xCAC2,
    CBT_TILE = 0xCAC3,
    CBY_CAPS = 0xCBC0,
    CBY_CAPSET = 0xCBC1,
    WBT_SYNC = 0xCCC0,
    WBT_CODEC_VERSIONS = 0xCCC1,
    WBT_CHANNELS = 0xCCC2,
    WBT_CONTEXT = 0xCCC3,
    WBT_FRAME_BEGIN = 0xCCC4,
    WBT_FRAME_END = 0xCCC5,
    WBT_REGION = 0xCCC6,
    WBT_EXTENSION = 0xCCC7
};

enum CLW_ENTROPY_RLGR
{
    CLW_ENTROPY_RLGR1 = 0x01,
    CLW_ENTROPY_RLGR3 = 0x04
};

/*
 * The TS_RFX_BLOCKT structure identifies the type of encodeTile message and specifies the size of the message.
 * */
struct TS_RFX_BLOCKT
{
    [[nodiscard]] bool Compose(OStream & s) const;
    [[nodiscard]] bool Decompose(IStream & s);
    [[nodiscard]] bool Decompose(IStream & s) const;

    uint16_t blockType;
    uint32_t blockLen;
};

/*
 * The TS_RFX_CODEC_CHANNELT structure is an extension of the TS_RFX_BLOCKT structure.
 * It is present as the first field in messages that are targeted for a specific combination of codec and channel.
 * */
struct TS_RFX_CODEC_CHANNELT
{
    [[nodiscard]] bool Compose(OStream & s) const;
    [[nodiscard]] bool Decompose(IStream & s);

    uint16_t blockType;
    uint32_t blockLen;
    uint8_t codecId;
    uint8_t channelId;
};

/* The TS_RFX_CHANNELT structure is used to specify the screen resolution of a channel. */
struct TS_RFX_CHANNELT
{
    [[nodiscard]] bool Compose(OStream & s) const;
    [[nodiscard]] bool Decompose(IStream & s);

    uint8_t channelId;
    uint16_t width;
    uint16_t height;
};

/*
 * The TS_RFX_CODEC_VERSIONT structure is used to specify support for a specific version of the RemoteFX codec.
 * */
struct TS_RFX_CODEC_VERSIONT
{
    [[nodiscard]] bool Compose(OStream & s) const;
    [[nodiscard]] bool Decompose(IStream & s) const;

    const uint8_t codecId{0x01}; /* Must be 0x01 */
    const uint16_t version{0x0100}; /* Must be 0x0100 */
};

/*
 * The TS_RFX_CODEC_QUANT structure holds the scalar quantization values for the ten sub-bands in the 3-level
 * Dwt decomposition. Each field in this structure MUST have a value in the range of 6 to 15.
 * */
struct TS_RFX_CODEC_QUANT
{
    [[nodiscard]] bool Compose(OStream & s) const;
    [[nodiscard]] bool Decompose(IStream & s);

    uint8_t LL3:4;
    uint8_t LH3:4;
    uint8_t HL3:4;
    uint8_t HH3:4;
    uint8_t LH2:4;
    uint8_t HL2:4;
    uint8_t HH2:4;
    uint8_t LH1:4;
    uint8_t HL1:4;
    uint8_t HH1:4;
};

/* The TS_RFX_RECT structure is used to specify a rectangle. */
struct TS_RFX_RECT
{
    [[nodiscard]] bool Compose(OStream & s) const;
    [[nodiscard]] bool Decompose(IStream & s);

    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
};

/*
 * The TS_RFX_SYNC message MUST be the first message in any encoded stream.
 * The decoder MUST examine this message to determine whether the protocol version is supported.
 * */
struct TS_RFX_SYNC
{
    [[nodiscard]] bool Compose(OStream & s) const;
    [[nodiscard]] bool Decompose(IStream & s) const;

    const TS_RFX_BLOCKT BlockT {TS_RFX_BLOCK_TYPE::WBT_SYNC,12};
    const uint32_t magic {WF_MAGIC}; /* Must be WF_MAGIC(0xCACCACCA) */
    const uint16_t version {WF_VERSION_1_0}; /* Must be WF_VERSION_1_0(0x0100) */
};

/* The TS_RFX_CODEC_VERSIONS message indicates the version of the RemoteFX codec that is being used. */
struct TS_RFX_CODEC_VERSIONS
{
    [[nodiscard]] bool Compose(OStream & s) const;
    [[nodiscard]] bool Decompose(IStream & s) const;

    const TS_RFX_BLOCKT BlockT {TS_RFX_BLOCK_TYPE::WBT_CODEC_VERSIONS, 10};
    const uint8_t numCodecs {0x01};
    const TS_RFX_CODEC_VERSIONT codecs;
};

/*
 * The TS_RFX_CHANNELS message contains the list of channels.
 * Each active monitor on the server must correspond to an entry in this list.
 * The list can have more entries than the number of active monitors.
 * The decoder endpoint MUST be able to support channels with different frame dimensions.
 * */
struct TS_RFX_CHANNELS
{
    [[nodiscard]] bool Compose(OStream & s) const;
    [[nodiscard]] bool Decompose(IStream & s);

    TS_RFX_BLOCKT BlockT {TS_RFX_BLOCK_TYPE::WBT_CHANNELS, 0};
    uint8_t numChannels;
    std::vector<TS_RFX_CHANNELT> Channels;
};

struct TS_RFX_CONTEXT_PROPERTIES
{
    [[nodiscard]] bool Compose(OStream & s) const;
    [[nodiscard]] bool Decompose(IStream & s);

    uint8_t flags;
    const uint8_t cct {COL_CONV_ICT};
    const uint8_t xft {CLW_XFORM_DWT_53_A};
    uint8_t et;
    const uint8_t qt {SCALAR_QUANTIZATION};
    uint8_t r;
};

/*
 * The TS_RFX_CONTEXT message contains information regarding the encoding properties being used.
 * */
struct TS_RFX_CONTEXT
{
    [[nodiscard]] bool Compose(OStream & s) const;
    [[nodiscard]] bool Decompose(IStream & s);

    TS_RFX_CODEC_CHANNELT CodecChannelT {TS_RFX_BLOCK_TYPE::WBT_CONTEXT, 13};
    const uint8_t ctxId {0x00};
    const uint16_t tileSize {CT_TILE_64x64};
    TS_RFX_CONTEXT_PROPERTIES properties;
};

/*
 * The TS_RFX_FRAME_BEGIN message indicates the start of a new frame for a specific channel in the encoded stream.
 * */
struct TS_RFX_FRAME_BEGIN
{
    [[nodiscard]] bool Compose(OStream & s) const;
    [[nodiscard]] bool Decompose(IStream & s);

    TS_RFX_CODEC_CHANNELT CodecChannelT {TS_RFX_BLOCK_TYPE::WBT_FRAME_BEGIN, 14};
    uint32_t frameIdx;
    uint16_t numRegions;
};

/*
 * The TS_RFX_FRAME_END message specifies the end of a frame for a specific channel in the encoded stream.
 * */
struct TS_RFX_FRAME_END
{
    [[nodiscard]] bool Compose(OStream & s) const;
    [[nodiscard]] bool Decompose(IStream & s);

    TS_RFX_CODEC_CHANNELT CodecChannelT {TS_RFX_BLOCK_TYPE::WBT_FRAME_END};
};

/*
 * The TS_RFX_REGION message contains information about the list of change rectangles on the screen
 * for a specific channel.
 * */
struct TS_RFX_REGION
{
    [[nodiscard]] bool Compose(OStream & s) const;
    [[nodiscard]] bool Decompose(IStream & s);

    TS_RFX_CODEC_CHANNELT CodecChannelT {TS_RFX_BLOCK_TYPE::WBT_REGION};
    uint8_t regionFlags;
    uint16_t numRects;
    std::vector<TS_RFX_RECT> rects;
    const uint16_t regionType {TS_RFX_BLOCK_TYPE::CBT_REGION};
    const uint16_t numTilesets {0x0001};
};

struct TS_RFX_TILESET_PROPERTIES
{
    [[nodiscard]] bool Compose(OStream & s) const;
    [[nodiscard]] bool Decompose(IStream & s);
    const uint8_t A {0x01};
    uint8_t flags;
    const uint8_t cct {COL_CONV_ICT};
    const uint8_t xft {CLW_XFORM_DWT_53_A};
    uint8_t et;
    const uint8_t qt {SCALAR_QUANTIZATION};
};

/*
 * The TS_RFX_TILE structure specifies the position of the tile on the frame and contains the encoded data
 * for the three tile components of Y, Cb, and Cr.
 * */
struct TS_RFX_TILE
{
    [[nodiscard]] bool Compose(OStream & s) const;
    [[nodiscard]] bool Decompose(IStream & s);

    TS_RFX_BLOCKT BlockT {CBT_TILE};
    uint8_t quantIdxY;
    uint8_t quantIdxCb;
    uint8_t quantIdxCr;
    uint16_t xIdx;
    uint16_t yIdx;
    uint16_t YLen;
    uint16_t CbLen;
    uint16_t CrLen;
    std::vector<uint8_t> YData;
    std::vector<uint8_t> CbData;
    std::vector<uint8_t> CrData;
};

/*
 * The TS_RFX_TILESET message contains encoding parameters and data for an arbitrary number of encoded tiles.
 * */
struct TS_RFX_TILESET
{
    [[nodiscard]] bool Compose(OStream & s) const;
    [[nodiscard]] bool Decompose(IStream & s);

    TS_RFX_CODEC_CHANNELT CodecChannelT {TS_RFX_BLOCK_TYPE::WBT_EXTENSION};
    const uint16_t subtype {TS_RFX_BLOCK_TYPE::CBT_TILESET};
    const uint16_t idx {0x0000};
    TS_RFX_TILESET_PROPERTIES properties;
    uint8_t numQuant;
    uint8_t tileSize {CT_TILE_64x64};
    uint16_t numTiles;
    uint32_t tilesDataSize;
    std::vector<TS_RFX_CODEC_QUANT> quantVals;
    std::vector<TS_RFX_TILE> tiles;
};

class RfxMessage
{
public:
    explicit RfxMessage(uint32_t frameIdx)
        :frameIdx(frameIdx)
    {

    }
public:
    void Compose(OStream & s);
    void Decompose(IStream & s);
private:


private:
    uint32_t frameIdx;

    /**
     * The rects array represents the updated region of the frame. The UI
     * requires to clip drawing destination base on the union of the rects.
     */
    std::vector<TS_RFX_RECT> rects;

    /**
     * The tiles array represents the actual frame data. Each tile is always
     * 64x64. Note that only pixels inside the updated region (represented as
     * rects described above) are valid. Pixels outside of the region may
     * contain arbitrary data.
     */
    std::vector<TS_RFX_TILE> tiles;

    std::vector<TS_RFX_CODEC_QUANT> quantVals;
};


#endif //RFXCPP_RFXMESSAGE_H
