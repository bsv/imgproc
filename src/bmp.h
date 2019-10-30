#ifndef _BMP_H
#define _BMP_H

typedef unsigned short int WORD;

#pragma pack(push, 1)
typedef struct {
    WORD bfType;          // 0x4d42 | 0x4349 | 0x5450
    int  bfSize;          // размер файла
    int  bfReserved;      // 0
    int  bfOffBits;       // смещение до поля данных,
                          // обычно 54 = 16 + biSize
    int  biSize;          // размер струкуры в байтах:
                          // 40(BITMAPINFOHEADER) или 108(BITMAPV4HEADER)
                          // или 124(BITMAPV5HEADER)
    int  biWidth;         // ширина в точках
    int  biHeight;        // высота в точках
    WORD biPlanes;        // всегда должно быть 1
    WORD biBitCount;      // 0 | 1 | 4 | 8 | 16 | 24 | 32
    int  biCompression;   // BI_RGB | BI_RLE8 | BI_RLE4 |
                          // BI_BITFIELDS | BI_JPEG | BI_PNG
    int  biSizeImage;     // Количество байт в поле данных
                          // Обычно устанавливается в 0
    int  biXPelsPerMeter; // горизонтальное разрешение, точек на дюйм
    int  biYPelsPerMeter; // вертикальное разрешение, точек на дюйм
    int  biClrUsed;       // Количество используемых цветов
                          // (если есть таблица цветов)
    int  biClrImportant;  // Количество существенных цветов.
} BMPheader;
#pragma pack(pop)

typedef struct {
    int r;
    int g;
    int b;
} Pixel;

typedef struct {
    Pixel * data;
    int width;
    int height;
} Image;

Image loadBMP(const char *fname);
int saveBMP(const char *fname, Image img);

#endif // _BMP_H
