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

    return true;
}

bool TS_RFX_RECT::Compose(OStream &s) const {
    return false;
}

bool TS_RFX_RECT::Decompose(IStream &s) {
    return false;
}

bool TS_RFX_SYNC::Compose(OStream &s) const {
    return false;
}

bool TS_RFX_SYNC::Decompose(IStream &s) {
    return false;
}

bool TS_RFX_CODEC_VERSIONS::Compose(OStream &s) const {
    return false;
}

bool TS_RFX_CODEC_VERSIONS::Decompose(IStream &s) {
    return false;
}

bool TS_RFX_CHANNELS::Compose(OStream &s) const {
    return false;
}

bool TS_RFX_CHANNELS::Decompose(IStream &s) {
    return false;
}

bool TS_RFX_CONTEXT_PROPERTIES::Compose(OStream &s) const {
    return false;
}

bool TS_RFX_CONTEXT_PROPERTIES::Decompose(IStream &s) {
    return false;
}

bool TS_RFX_CONTEXT::Compose(OStream &s) const {
    return false;
}

bool TS_RFX_CONTEXT::Decompose(IStream &s) {
    return false;
}

bool TS_RFX_FRAME_BEGIN::Compose(OStream &s) const {
    return false;
}

bool TS_RFX_FRAME_BEGIN::Decompose(IStream &s) {
    return false;
}

bool TS_RFX_FRAME_END::Compose(OStream &s) const {
    return false;
}

bool TS_RFX_FRAME_END::Decompose(IStream &s) {
    return false;
}

bool TS_RFX_REGION::Compose(OStream &s) const {
    return false;
}

bool TS_RFX_REGION::Decompose(IStream &s) {
    return false;
}

bool TS_RFX_TILESET_PROPERTIES::Compose(OStream &s) const {
    return false;
}

bool TS_RFX_TILESET_PROPERTIES::Decompose(IStream &s) {
    return false;
}

bool TS_RFX_TILE::Compose(OStream &s) const {
    return false;
}

bool TS_RFX_TILE::Decompose(IStream &s) {
    return false;
}

bool TS_RFX_TILESET::Compose(OStream &s) const {
    return false;
}

bool TS_RFX_TILESET::Decompose(IStream &s) {
    return false;
}

void RfxMessage::Compose() {

}

void RfxMessage::Decompose() {

}
