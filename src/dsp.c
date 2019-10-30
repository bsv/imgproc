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

void laplacian(Image src, Image dst) {
    int filt[] = {0, 1, 0,
			      1, -4, 1,
			      0, 1, 0 };

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
                res_r += core[i].r * filt[i];
                res_g += core[i].g * filt[i];
                res_b += core[i].b * filt[i];
            }

            if(abs(res_r) > 255) res_r = 255;
            if(abs(res_g) > 255) res_g = 255;
            if(abs(res_b) > 255) res_b = 255;

            int px_index = y*dst.width + x;
            dst.data[px_index].r = src.data[px_index].r + abs(res_r);
            dst.data[px_index].g = src.data[px_index].g + abs(res_g);
            dst.data[px_index].b = src.data[px_index].b + abs(res_b);
        }
    }
}

Pixel getMedian(Pixel * core, int size) {

    int mas[size*size];

    Pixel median_px;

    // initialization
    for(int i = 0; i < size*size; i++) {
        mas[i] = (core[i].r + core[i].g + core[i].b)/3;
    }

    // sorting
    for(int i = 0; i < size*size-1; i++) {

        int max_i = i;
        for(int j = i+1; j < size*size; j++) {
            if(mas[j] > mas[max_i]) max_i = j;
        }
        int buf = 0;
        buf = mas[i];
        mas[i] = mas[max_i];
        mas[max_i] = buf;

    }

    int median_index = size*size/2-1;

    median_px.r = mas[median_index];
    median_px.g = mas[median_index];
    median_px.b = mas[median_index];

    return median_px;
}

void medianFilt(Image src, Image dst) {

    int res_r = 0;
    int res_g = 0;
    int res_b = 0;

    const int CORE_SIZE = 3;
    Pixel core[CORE_SIZE*CORE_SIZE];
    Pixel median_px;

    for(int y = 0; y < src.height; y++) {
        for(int x = 0; x < src.width; x++) {
            getCore(src, core, x, y, CORE_SIZE);
            median_px = getMedian(core, CORE_SIZE);

            dst.data[y*dst.width + x].r = median_px.r;
            dst.data[y*dst.width + x].g = median_px.g;
            dst.data[y*dst.width + x].b = median_px.b;
        }
    }
}