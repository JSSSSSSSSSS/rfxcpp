//
// Created by Administrator on 2024/1/14.
//

#include "RfxMessage.h"

bool TS_RFX_BLOCKT::Compose(OStream & s) const
{
    if (!s.ensureRemain(6))
    {
        return false;
    }
    s.write(blockType);
    s.write(blockLen);
    return true;
}

bool TS_RFX_BLOCKT::Decompose(IStream & s)
{
    if (!s.ensureRemain(6))
    {
        return false;
    }
    s.read(blockType);
    s.read(blockLen);
    return true;
}

bool TS_RFX_BLOCKT::Decompose(IStream &s) const
{
    if (!s.ensureRemain(6))
    {
        return false;
    }

    uint16_t blockType_tmp;
    uint32_t blockLen_tmp;

    s.read(blockType_tmp);
    s.read(blockLen_tmp);

    if (blockLen_tmp != blockType || blockLen_tmp != blockLen)
    {
        return false;
    }

    return true;
}

bool TS_RFX_CODEC_CHANNELT::Compose(OStream &s) const
{
    if (!s.ensureRemain(8))
    {
        return false;
    }

    s.write(blockType);
    s.write(blockLen);
    s.write(codecId);
    s.write(channelId);

    return true;
}

bool TS_RFX_CODEC_CHANNELT::Decompose(IStream &s)
{
    if (!s.ensureRemain(2))
    {
        return false;
    }

    s.read(blockType);
    s.read(blockLen);
    s.read(codecId);
    s.read(channelId);
    return true;
}

bool TS_RFX_CHANNELT::Compose(OStream &s) const
{
    if (!s.ensureRemain(5))
    {
        return false;
    }

    s.write(channelId);
    s.write(width);
    s.write(height);

    return true;
}

bool TS_RFX_CHANNELT::Decompose(IStream &s)
{
    if (!s.ensureRemain(5))
    {
        return false;
    }
    s.read(channelId);
    s.read(width);
    s.read(height);
    return true;
}

bool TS_RFX_CODEC_VERSIONT::Compose(OStream &s) const
{
    if(!s.ensureRemain(3))
    {
        return false;
    }

    s.write(codecId);
    s.write(version);
    return true;
}

bool TS_RFX_CODEC_VERSIONT::Decompose(IStream &s) const
{
    uint8_t codecId_tmp;
    uint16_t version_tmp;
    if (!s.ensureRemain(3))
    {
        return false;
    }

    s.read(codecId_tmp);
    s.read(version_tmp);

    if (codecId_tmp != codecId || version_tmp != version)
    {
        return false;
    }

    return true;
}

bool TS_RFX_CODEC_QUANT::Compose(OStream &s) const
{
    if (!s.ensureRemain(5))
    {
        return false;
    }

    s.write(*this);

    return true;
}

bool TS_RFX_CODEC_QUANT::Decompose(IStream &s)
{
    if (!s.ensureRemain(5))
    {
        return false;
    }

    uint8_t vals[5];

    s.read(vals);

    LL3 = vals[0] >> 4 & 0x0F;
    LH3 = vals[0] & 0x0F;
    HL3 = vals[1] >> 4 & 0x0F;
    HH3 = vals[1] & 0x0F;
    LH2 = vals[2] >> 4 & 0x0F;
    HL2 = vals[2] & 0x0F;
    HH2 = vals[3] >> 4 & 0x0F;
    LH1 = vals[3] & 0x0F;
    HL1 = vals[4] >> 4 & 0x0F;
    HH1 = vals[4] & 0x0F;

    return true;
}

bool TS_RFX_RECT::Compose(OStream &s) const
{
    if (!s.ensureRemain(8))
    {
        return false;
    }

    s.write(x);
    s.write(y);
    s.write(width);
    s.write(height);

    return true;
}

bool TS_RFX_RECT::Decompose(IStream &s)
{
    if (!s.ensureRemain(8))
    {
        return false;
    }

    s.read(x);
    s.read(y);
    s.read(width);
    s.read(height);

    return true;
}

bool TS_RFX_SYNC::Compose(OStream &s) const
{
    if (!BlockT.Compose(s))
    {
        return false;
    }

    if (!s.ensureRemain(6))
    {
        return false;
    }

    s.write(magic);
    s.write(version);

    return true;
}

bool TS_RFX_SYNC::Decompose(IStream &s) const
{
    if (!BlockT.Decompose(s))
    {
        return false;
    }

    if (!s.ensureRemain(6))
    {
        return false;
    }

    uint32_t magic_tmp;
    uint16_t version_tmp;

    s.read(magic_tmp); /* Must be WF_MAGIC(0xCACCACCA) */
    s.read(version_tmp); /* Must be WF_VERSION_1_0(0x0100) */

    if (magic_tmp != magic || version_tmp != version)
    {
        return false;
    }

    return true;
}

bool TS_RFX_CODEC_VERSIONS::Compose(OStream &s) const
{
    if (!BlockT.Compose(s))
    {
        return false;
    }

    if (!s.ensureRemain(1))
    {
        return  false;
    }

    if (!codecs.Compose(s))
    {
        return false;
    }

    return true;
}

bool TS_RFX_CODEC_VERSIONS::Decompose(IStream &s) const
{
    if (!BlockT.Decompose(s))
    {
        return false;
    }

    if (!s.ensureRemain(1))
    {
        return false;
    }

    uint8_t numCodecs_tmp;
    s.read(numCodecs_tmp);

    if (numCodecs_tmp != numCodecs)
    {
        return false;
    }

    if (!codecs.Decompose(s))
    {
        return false;
    }

    return true;
}

bool TS_RFX_CHANNELS::Compose(OStream &s) const
{
    if (!BlockT.Compose(s))
    {
        return false;
    }

    if (!s.ensureRemain(1))
    {
        return false;
    }
    s.write(numChannels);

    for(auto channel:Channels)
    {
        if(!channel.Compose(s))
        {
            return false;
        }
    }

    return true;
}

bool TS_RFX_CHANNELS::Decompose(IStream &s)
{
    if (!BlockT.Decompose(s))
    {
        return false;
    }

    if (!s.ensureRemain(1))
    {
        return false;
    }

    s.read(numChannels);

    for(int i = 0; i < numChannels; ++i)
    {
        TS_RFX_CHANNELT channelt {};
        if (!channelt.Decompose(s))
        {
            return false;
        }
        Channels.push_back(channelt);
    }

    return true;
}

bool TS_RFX_CONTEXT_PROPERTIES::Compose(OStream &s) const
{
    if (!s.ensureRemain(2))
    {
        return false;
    }

    uint16_t properties {0};
    properties |= (flags & 0x0007);       // 3 bit
    properties |= ((cct << 3) & 0x0018);  // 2 bit
    properties |= ((xft << 5) & 0x01E0);  // 4 bit
    properties |= ((et << 9) & 0x1E00);
    properties |= ((qt << 13) & 0x6000);

    s.write(properties);

    return true;
}

bool TS_RFX_CONTEXT_PROPERTIES::Decompose(IStream &s)
{
    if (!s.ensureRemain(2))
    {
        return false;
    }

    uint16_t  properties {0};
    s.read(properties);

    flags = properties & 0x0007;
    et = (properties & 0x1E00) >> 9;

    return true;
}

bool TS_RFX_CONTEXT::Compose(OStream &s) const
{
    if (!CodecChannelT.Compose(s))
    {
        return false;
    }

    if (!s.ensureRemain(3))
    {
        return false;
    }

    s.write(ctxId);
    s.write(tileSize);

    if(!properties.Compose(s))
    {
        return  false;
    }

    return true;
}

bool TS_RFX_CONTEXT::Decompose(IStream &s)
{
    if (!CodecChannelT.Decompose(s))
    {
        return false;
    }

    if (!s.ensureRemain(3))
    {
        return false;
    }

    uint8_t ctxId_tmp;
    uint16_t tileSize_tmp;

    s.read(ctxId_tmp);
    s.read(tileSize_tmp);

    if (ctxId_tmp != ctxId || tileSize_tmp != tileSize)
    {
        return false;
    }

    if (!properties.Decompose(s))
    {
        return false;
    }

    return true;
}

bool TS_RFX_FRAME_BEGIN::Compose(OStream &s) const
{
    if (!CodecChannelT.Compose(s))
    {
        return false;
    }

    if (!s.ensureRemain(6))
    {
        return false;
    }

    s.write(frameIdx);
    s.write(numRegions);

    return true;
}

bool TS_RFX_FRAME_BEGIN::Decompose(IStream &s)
{
    if (!CodecChannelT.Decompose(s))
    {
        return false;
    }

    if (!s.ensureRemain(6))
    {
        return false;
    }

    s.read(frameIdx);
    s.read(numRegions);

    return true;
}

bool TS_RFX_FRAME_END::Compose(OStream &s) const
{
    if (!CodecChannelT.Compose(s))
    {
        return false;
    }

    return true;
}

bool TS_RFX_FRAME_END::Decompose(IStream &s)
{
    if (!CodecChannelT.Decompose(s))
    {
        return false;
    }

    return true;
}

bool TS_RFX_REGION::Compose(OStream &s) const
{
    if (!CodecChannelT.Compose(s))
    {
        return false;
    }

    if (!s.ensureRemain(3))
    {
        return false;
    }

    s.write(regionFlags);
    s.write(numRects);

    for(auto rect:rects)
    {
        if (!rect.Compose(s))
        {
            return false;
        }
    }

    if (!s.ensureRemain(4))
    {
        return false;
    }

    s.write(regionType);
    s.write(numTilesets);

    return true;
}

bool TS_RFX_REGION::Decompose(IStream &s)
{
    if (!CodecChannelT.Decompose(s))
    {
        return false;
    }

    if (!s.ensureRemain(3))
    {
        return false;
    }

    s.read(regionFlags);
    s.read(numRects);

    rects.clear();
    for(int i = 0; i < numRects; ++i)
    {
        TS_RFX_RECT rect {0};
        if (!rect.Decompose(s))
        {
            return false;
        }
        rects.push_back(rect);
    }

    if (!s.ensureRemain(4))
    {
        return false;
    }

    uint16_t regionType_tmp;
    uint16_t numTilesets_tmp;

    s.read(regionType_tmp);
    s.read(numTilesets_tmp);

    if (regionType_tmp != regionType || numTilesets_tmp != numTilesets)
    {
        return false;
    }

    return true;
}

bool TS_RFX_TILESET_PROPERTIES::Compose(OStream &s) const
{
    if (!s.ensureRemain(2))
    {
        return false;
    }

    uint16_t  properties {0};
    properties = A;
    properties |= ((flags << 1) & 0x000E);
    properties |= ((cct << 4) & 0x0030);
    properties |= ((xft << 6) & 0x03C0);
    properties |= ((et << 10) & 0x3C00);
    properties |= ((qt << 14) & 0xC000);

    s.write(properties);

    return true;
}

bool TS_RFX_TILESET_PROPERTIES::Decompose(IStream &s)
{
    if (!s.ensureRemain(2))
    {
        return false;
    }

    uint16_t  properties {0};
    s.read(properties);

    flags = (properties & 0x000E) >> 1;
    et = (properties & 0x3C00) >> 10;

    return true;
}

bool TS_RFX_TILE::Compose(OStream &s) const
{
    if (!BlockT.Compose(s))
    {
        return false;
    }

    if (!s.ensureRemain(13))
    {
        return false;
    }

    s.write(quantIdxY);
    s.write(quantIdxCb);
    s.write(quantIdxCr);
    s.write(xIdx);
    s.write(yIdx);
    s.write(YLen);
    s.write(CbLen);
    s.write(CrLen);

    if (!s.ensureRemain(YLen + CbLen + CrLen))
    {
        return false;
    }

    for(auto y:YData)
    {
        s.write(y);
    }

    for(auto cb:CbData)
    {
        s.write(cb);
    }

    for(auto cr:CrData)
    {
        s.write(cr);
    }

    return true;
}

bool TS_RFX_TILE::Decompose(IStream &s)
{
    if (!BlockT.Decompose(s))
    {
        return false;
    }

    if (!s.ensureRemain(13))
    {
        return false;
    }

    s.read(quantIdxY);
    s.read(quantIdxCb);
    s.read(quantIdxCr);
    s.read(xIdx);
    s.read(yIdx);
    s.read(YLen);
    s.read(CbLen);
    s.read(CrLen);

    if (!s.ensureRemain(YLen + CbLen + CrLen))
    {
        return false;
    }

    YData.clear();
    for(int i = 0; i < YLen;++i)
    {
        uint8_t y;
        s.read(y);
        YData.push_back(y);
    }

    CbData.clear();
    for(int i = 0; i < CbLen;++i)
    {
        uint8_t cb;
        s.read(cb);
        CbData.push_back(cb);
    }

    CrData.clear();
    for(int i = 0; i < CrLen;++i)
    {
        uint8_t cr;
        s.read(cr);
        YData.push_back(cr);
    }

    return true;
}

bool TS_RFX_TILESET::Compose(OStream &s) const
{
    if (!CodecChannelT.Compose(s))
    {
        return false;
    }

    if (!s.ensureRemain(14))
    {
        return false;
    }

    s.write(subtype);
    s.write(idx);

    if (!properties.Compose(s))
    {
        return false;
    }

    s.write(numQuant);
    s.write(tileSize);
    s.write(numTiles);
    s.write(tilesDataSize);

    for(auto vals:quantVals)
    {
        if (!vals.Compose(s))
        {
            return false;
        }
    }

    for(const auto& tile:tiles)
    {
        if (!tile.Compose(s))
        {
            return false;
        }
    }

    return true;
}

bool TS_RFX_TILESET::Decompose(IStream &s)
{
    if (!CodecChannelT.Decompose(s))
    {
        return false;
    }

    if (!s.ensureRemain(14))
    {
        return false;
    }

    s.skip(subtype);
    s.skip(idx);

    if (!properties.Decompose(s))
    {
        return false;
    }

    s.read(numQuant);
    s.read(tileSize);
    s.read(numTiles);
    s.read(tilesDataSize);

    quantVals.clear();
    for(int i = 0; i < numQuant; ++i)
    {
        TS_RFX_CODEC_QUANT quant {0};
        if (!quant.Decompose(s))
        {
            return false;
        }
        quantVals.push_back(quant);
    }

    tiles.clear();
    for(int i = 0; i < numTiles; ++i)
    {
        TS_RFX_TILE tile {0};
        if (!tile.Decompose(s))
        {
            return false;
        }
        tiles.push_back(tile);
    }

    return true;
}

void RfxMessage::Compose(OStream & s) {

}

void RfxMessage::Decompose(IStream & s) {

}
