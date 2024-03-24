//
// Created by Administrator on 2024/2/26.
//
#include "Tile.h"
#include <cstdint>

/*
 *  * 0.299   -0.168935    0.499813
 * 0.587   -0.331665   -0.418531
 * 0.114    0.50059    -0.081282
   y = r *  0.299000 + g *  0.587000 + b *  0.114000;
   u = r * -0.168935 + g * -0.331665 + b *  0.500590;
   v = r *  0.499813 + g * -0.418531 + b * -0.081282;
*/

/* 19595  38470   7471
  -11071 -21736  32807
   32756 -27429  -5327
 */
void tileRgbToDwt(Tile_dwt & output, const Tile_rgb & input)
{
    int32_t r, g, b;
    int32_t y, u, v;
    for(int i = 0; i < input.b_data.size(); ++i)
    {
        r = input.r_data[i];
        g = input.g_data[i];
        b = input.b_data[i];

        y = (r *  19595 + g *  38470 + b *   7471) >> 16;
        u = (r * -11071 + g * -21736 + b *  32807) >> 16;
        v = (r *  32756 + g * -27429 + b *  -5327) >> 16;

        y = std::min(std::max(y,  000), 255);
        u = std::min(std::max(u, -128), 127);
        v = std::min(std::max(v, -128), 127);

        output.y_data[i] = static_cast<int16_t>(y - 128);
        output.cb_data[i] = static_cast<int16_t>(u);
        output.cr_data[i] = static_cast<int16_t>(v);
    }
}

/*
 * https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-rdprfx/2e1618ed-60d6-4a64-aa5d-0608884861bb
 * 1.0        1.0         1.0
 * 0.0       -0.343730    1.769905
 * 1.402525  -0.714401    0.000013
 *
   r = y * 1.0 + cb *  0.000000 + cr *  1.402525
   g = y * 1.0 + cb * -0.343730 + cr * -0.714401
   b = y * 1.0 + cb *  1.769905 + cr *  0.000013
*/
/* 65536            0              91916
 * 65536           -22527          -46819
 * 65536(>>16)     115992(>>16)    55835 (>>32)
 * */

void tileDwtToRgb(Tile_rgb & output, const Tile_dwt & input)
{
    int32_t r, g, b;
    int32_t y, u, v;
    for(int i = 0; i < input.y_data.size(); ++i)
    {
        y = input.y_data[i] + 128;
        u = input.cb_data[i];
        v = input.cr_data[i];

        r = (y * 65536 + u *  00000  + v *  91916) >> 16;
        g = (y * 65536 + u * -22527  + v * -46819) >> 16;
        b = ((y * 65536 + u *  115992) >> 16) + static_cast<int32_t>((v * static_cast<int64_t>(55835)) >> 32);

        r = std::min(std::max(r, 0), 255);
        g = std::min(std::max(g, 0), 255);
        b = std::min(std::max(b, 0), 255);

        output.r_data[i] = static_cast<uint8_t>(r);
        output.g_data[i] = static_cast<uint8_t>(g);
        output.b_data[i] = static_cast<uint8_t>(b);
    }
}
