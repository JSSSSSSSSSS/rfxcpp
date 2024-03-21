#include <iostream>

#include <Rfx.h>
#include <Bitmap.h>

int main()
{
    Bitmap bmp("test.Bitmap");
    Rfx rfx(bmp.getWidth(), bmp.getHeight(), 32);

    std::vector<uint8_t> encodedData;
    rfx.encode(encodedData, bmp, 0);

    std::cout << encodedData.size() << std::endl;

    return 0;
}
