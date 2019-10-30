#ifndef _DSP_H
#define _DSP_H

#include "bmp.h"

void getCore(Image src, Pixel * core, int px, int py, int size);
Pixel getMedian(Pixel * core, int size);

void sobel(Image src, Image dst);
void smoothing(Image src, Image dst);
void medianFilt(Image src, Image dst);
void laplacian(Image src, Image dst);

#endif // _DSP_H
