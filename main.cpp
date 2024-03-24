#include <iostream>

#include <Rfx.h>
#include <Bitmap.h>

int main()
{
    Bitmap bmp;
    bmp.load(R"(D:\code\cpp\rfxcpp\test.bmp)");

    uint32_t raw_size = bmp.getRgbData().size() * sizeof(bmp.getRgbData()[0]);

    std::cout << "raw data: " << raw_size / 1024 << "KB" << std::endl;

    std::vector<uint8_t> encoded_data;
    Rfx rfx_codec(bmp.getWidth(), bmp.getWidth(), bmp.getBytesPerPixel() * 8);
    rfx_codec.encode(encoded_data, bmp, 0);

    uint32_t encoded_size = encoded_data.size();
    std::cout << "encoded_data: " << encoded_size / 1024 << "KB" << std::endl;

    Bitmap bmp_decoded;
    uint32_t frame_index;
    rfx_codec.decode(bmp_decoded, encoded_data, frame_index);
    bmp_decoded.dump(R"(D:\code\cpp\rfxcpp\test1.bmp)");

    return 0;
}
