//
// Created by Administrator on 2024/1/14.
//

#include "RfxMessage.h"

bool TS_RFX_BLOCKT::Compose(OStream & s) const
{
    if (!s.EnsureRemain(6))
    {
        return false;
    }
    s.Write(blockType);
    s.Write(blockLen);
    return true;
}

bool TS_RFX_BLOCKT::Decompose(IStream & s)
{
    if (!s.EnsureRemain(6))
    {
        return false;
    }
    s.Read(blockType);
    s.Read(blockLen);
    return true;
}

bool TS_RFX_BLOCKT::Decompose(IStream &s) const
{
    if (!s.EnsureRemain(6))
    {
        return false;
    }

    uint16_t blockType_tmp;
    uint32_t blockLen_tmp;

    s.Read(blockType_tmp);
    s.Read(blockLen_tmp);

    if (blockLen_tmp != blockType || blockLen_tmp != blockLen)
    {
        return false;
    }

    return true;
}

bool TS_RFX_CODEC_CHANNELT::Compose(OStream &s) const
{
    if (!s.EnsureRemain(8))
    {
        return false;
    }

    s.Write(blockType);
    s.Write(blockLen);
    s.Write(codecId);
    s.Write(channelId);

    return true;
}

bool TS_RFX_CODEC_CHANNELT::Decompose(IStream &s)
{
    if (!s.EnsureRemain(2))
    {
        return false;
    }

    s.Read(blockType);
    s.Read(blockLen);
    s.Read(codecId);
    s.Read(channelId);
    return true;
}

bool TS_RFX_CHANNELT::Compose(OStream &s) const
{
    if (!s.EnsureRemain(5))
    {
        return false;
    }

    s.Write(channelId);
    s.Write(width);
    s.Write(height);

    return true;
}

bool TS_RFX_CHANNELT::Decompose(IStream &s)
{
    if (!s.EnsureRemain(5))
    {
        return false;
    }
    s.Read(channelId);
    s.Read(width);
    s.Read(height);
    return true;
}

bool TS_RFX_CODEC_VERSIONT::Compose(OStream &s) const
{
    if(!s.EnsureRemain(3))
    {
        return false;
    }

    s.Write(codecId);
    s.Write(version);
    return true;
}

bool TS_RFX_CODEC_VERSIONT::Decompose(IStream &s) const
{
    uint8_t codecId_tmp;
    uint16_t version_tmp;
    if (!s.EnsureRemain(3))
    {
        return false;
    }

    s.Read(codecId_tmp);
    s.Read(version_tmp);

    if (codecId_tmp != codecId || version_tmp != version)
    {
        return false;
    }

    return true;
}

bool TS_RFX_CODEC_QUANT::Compose(OStream &s) const
{
    if (!s.EnsureRemain(5))
    {
        return false;
    }

    s.Write(*this);

    return true;
}

bool TS_RFX_CODEC_QUANT::Decompose(IStream &s)
{
    if (!s.EnsureRemain(5))
    {
        return false;
    }

    uint8_t vals[5];

    s.Read(vals);

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
    if (!s.EnsureRemain(8))
    {
        return false;
    }

    s.Write(x);
    s.Write(y);
    s.Write(width);
    s.Write(height);

    return true;
}

bool TS_RFX_RECT::Decompose(IStream &s)
{
    if (!s.EnsureRemain(8))
    {
        return false;
    }

    s.Read(x);
    s.Read(y);
    s.Read(width);
    s.Read(height);

    return true;
}

bool TS_RFX_SYNC::Compose(OStream &s) const
{
    if (!BlockT.Compose(s))
    {
        return false;
    }

    if (!s.EnsureRemain(6))
    {
        return false;
    }

    s.Write(magic);
    s.Write(version);

    return true;
}

bool TS_RFX_SYNC::Decompose(IStream &s) const
{
    if (!BlockT.Decompose(s))
    {
        return false;
    }

    if (!s.EnsureRemain(6))
    {
        return false;
    }

    uint32_t magic_tmp;
    uint16_t version_tmp;

    s.Read(magic_tmp); /* Must be WF_MAGIC(0xCACCACCA) */
    s.Read(version_tmp); /* Must be WF_VERSION_1_0(0x0100) */

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

    if (!s.EnsureRemain(1))
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

    if (!s.EnsureRemain(1))
    {
        return false;
    }

    uint8_t numCodecs_tmp;
    s.Read(numCodecs_tmp);

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

    if (!s.EnsureRemain(1))
    {
        return false;
    }
    s.Write(numChannels);

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

    if (!s.EnsureRemain(1))
    {
        return false;
    }

    s.Read(numChannels);

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
    if (!s.EnsureRemain(2))
    {
        return false;
    }

    uint16_t properties {0};
    properties |= (flags & 0x0007);       // 3 bit
    properties |= ((cct << 3) & 0x0018);  // 2 bit
    properties |= ((xft << 5) & 0x01E0);  // 4 bit
    properties |= ((et << 9) & 0x1E00);
    properties |= ((qt << 13) & 0x6000);

    s.Write(properties);

    return true;
}

bool TS_RFX_CONTEXT_PROPERTIES::Decompose(IStream &s)
{
    if (!s.EnsureRemain(2))
    {
        return false;
    }

    uint16_t  properties {0};
    s.Read(properties);

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

    if (!s.EnsureRemain(3))
    {
        return false;
    }

    s.Write(ctxId);
    s.Write(tileSize);

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

    if (!s.EnsureRemain(3))
    {
        return false;
    }

    uint8_t ctxId_tmp;
    uint16_t tileSize_tmp;

    s.Read(ctxId_tmp);
    s.Read(tileSize_tmp);

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

    if (!s.EnsureRemain(6))
    {
        return false;
    }

    s.Write(frameIdx);
    s.Write(numRegions);

    return true;
}

bool TS_RFX_FRAME_BEGIN::Decompose(IStream &s)
{
    if (!CodecChannelT.Decompose(s))
    {
        return false;
    }

    if (!s.EnsureRemain(6))
    {
        return false;
    }

    s.Read(frameIdx);
    s.Read(numRegions);

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

    if (!s.EnsureRemain(3))
    {
        return false;
    }

    s.Write(regionFlags);
    s.Write(numRects);

    for(auto rect:rects)
    {
        if (!rect.Compose(s))
        {
            return false;
        }
    }

    if (!s.EnsureRemain(4))
    {
        return false;
    }

    s.Write(regionType);
    s.Write(numTilesets);

    return true;
}

bool TS_RFX_REGION::Decompose(IStream &s)
{
    if (!CodecChannelT.Decompose(s))
    {
        return false;
    }

    if (!s.EnsureRemain(3))
    {
        return false;
    }

    s.Read(regionFlags);
    s.Read(numRects);

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

    if (!s.EnsureRemain(4))
    {
        return false;
    }

    uint16_t regionType_tmp;
    uint16_t numTilesets_tmp;

    s.Read(regionType_tmp);
    s.Read(numTilesets_tmp);

    if (regionType_tmp != regionType || numTilesets_tmp != numTilesets)
    {
        return false;
    }

    return true;
}

bool TS_RFX_TILESET_PROPERTIES::Compose(OStream &s) const
{
    if (!s.EnsureRemain(2))
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

    s.Write(properties);

    return true;
}

bool TS_RFX_TILESET_PROPERTIES::Decompose(IStream &s)
{
    if (!s.EnsureRemain(2))
    {
        return false;
    }

    uint16_t  properties {0};
    s.Read(properties);

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

    if (!s.EnsureRemain(13))
    {
        return false;
    }

    s.Write(quantIdxY);
    s.Write(quantIdxCb);
    s.Write(quantIdxCr);
    s.Write(xIdx);
    s.Write(yIdx);
    s.Write(YLen);
    s.Write(CbLen);
    s.Write(CrLen);

    if (!s.EnsureRemain(YLen + CbLen + CrLen))
    {
        return false;
    }

    for(auto y:YData)
    {
        s.Write(y);
    }

    for(auto cb:CbData)
    {
        s.Write(cb);
    }

    for(auto cr:CrData)
    {
        s.Write(cr);
    }

    return true;
}

bool TS_RFX_TILE::Decompose(IStream &s)
{
    if (!BlockT.Decompose(s))
    {
        return false;
    }

    if (!s.EnsureRemain(13))
    {
        return false;
    }

    s.Read(quantIdxY);
    s.Read(quantIdxCb);
    s.Read(quantIdxCr);
    s.Read(xIdx);
    s.Read(yIdx);
    s.Read(YLen);
    s.Read(CbLen);
    s.Read(CrLen);

    if (!s.EnsureRemain(YLen + CbLen + CrLen))
    {
        return false;
    }

    YData.clear();
    for(int i = 0; i < YLen;++i)
    {
        uint8_t y;
        s.Read(y);
        YData.push_back(y);
    }

    CbData.clear();
    for(int i = 0; i < CbLen;++i)
    {
        uint8_t cb;
        s.Read(cb);
        CbData.push_back(cb);
    }

    CrData.clear();
    for(int i = 0; i < CrLen;++i)
    {
        uint8_t cr;
        s.Read(cr);
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

    if (!s.EnsureRemain(14))
    {
        return false;
    }

    s.Write(subtype);
    s.Write(idx);

    if (!properties.Compose(s))
    {
        return false;
    }

    s.Write(numQuant);
    s.Write(tileSize);
    s.Write(numTiles);
    s.Write(tilesDataSize);

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

    if (!s.EnsureRemain(14))
    {
        return false;
    }

    s.Read(subtype);
    s.Read(idx);

    if (!properties.Decompose(s))
    {
        return false;
    }

    s.Read(numQuant);
    s.Read(tileSize);
    s.Read(numTiles);
    s.Read(tilesDataSize);

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

void RfxMessage::Compose() {

}

void RfxMessage::Decompose() {

}
