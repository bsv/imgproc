#include "dsp.h"
#include <math.h>
#include <stdio.h>

void getCore(Image src, Pixel * core, int px, int py, int size) {
    Pixel * ptr = core;
    Pixel val;
    for(int y = py-(size-1)/2; y <= py+(size-1)/2; y++) {
        for(int x = px-(size-1)/2; x <= px+(size-1)/2; x++) {
            if( (y < 0) || (y >= src.height) ||
                (x < 0) || (x >= src.width)) {
                val = src.data[py*src.width + px];
            } else {
                val = src.data[y*src.width + x];
            }
            *ptr++ = val;
        }
    }
}

void sobel(Image src, Image dst)
{
    int gx[] = {-1, 0, 1,
			    -2, 0, 2,
			    -1, 0, 1 };
    int gy[] = {-1, -2, -1,
                0,  0,  0,
                1,  2,  1 };

    int x_part = 0;
    int y_part = 0;
    int gray_val = 0;

    const int CORE_SIZE = 3;
    Pixel core[CORE_SIZE*CORE_SIZE];

    for(int y = 0; y < src.height; y++) {
        for(int x = 0; x < src.width; x++) {
            getCore(src, core, x, y, CORE_SIZE);

            x_part = 0;
            y_part = 0;

            for(int i = 0; i < CORE_SIZE*CORE_SIZE; i++) {
                gray_val = (core[i].r + core[i].g + core[i].b)/3;
                x_part += gray_val*gx[i];
                y_part += gray_val*gy[i];
            }
            int res_val = sqrt(x_part*x_part + y_part*y_part);
            dst.data[y*dst.width + x].r = res_val;
            dst.data[y*dst.width + x].g = res_val;
            dst.data[y*dst.width + x].b = res_val;
        }
    }
}

void smoothing(Image src, Image dst) {
    int filt[] = {16, 8, 16,
			      8, 4, 8,
			     16, 8, 16 };

    int x_part = 0;
    int res_r = 0;
    int res_g = 0;
    int res_b = 0;

    const int CORE_SIZE = 3;
    Pixel core[CORE_SIZE*CORE_SIZE];

    for(int y = 0; y < src.height; y++) {
        for(int x = 0; x < src.width; x++) {
            getCore(src, core, x, y, CORE_SIZE);

            res_r = 0;
            res_g = 0;
            res_b = 0;

            for(int i = 0; i < CORE_SIZE*CORE_SIZE; i++) {
                res_r += core[i].r/filt[i];
                res_g += core[i].g/filt[i];
                res_b += core[i].b/filt[i];
            }

            dst.data[y*dst.width + x].r = res_r;
            dst.data[y*dst.width + x].g = res_g;
            dst.data[y*dst.width + x].b = res_b;
        }
    }
}