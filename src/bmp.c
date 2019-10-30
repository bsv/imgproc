#include <stdio.h>
#include "bmp.h"
#include <cstring>

Image loadBMP(const char *fname)
{
    Image img;
    img.data = NULL;

    printf("Filename: %s\n", fname);
    FILE *f = fopen( fname, "rb" );
    if( !f ) return img;
    BMPheader bh;    // File header sizeof(BMPheader) = 56
    size_t res;

    printf("Header size: %d\n", sizeof(BMPheader));
    // читаем заголовок
    res = fread( &bh, 1, sizeof(BMPheader), f );
    
    printf("---\n");
    //printf("Filesize: %d\n", filesize);
    printf("bh.bfType: %x\n", bh.bfType);
    printf("bh.bfSize: %d\n", bh.bfSize);
    printf("bh.bfReserved: %d\n", bh.bfReserved);
    printf("bh.bfOffBits: %d\n", bh.bfOffBits);
    
    printf("bh.biSize: %d\n", bh.biSize);
    printf("bh.biWidth: %d\n", bh.biWidth);
    printf("bh.biHeight: %d\n", bh.biHeight);
    printf("bh.biPlanes: %d\n", bh.biPlanes);
    printf("bh.biBitCount: %d\n", bh.biBitCount);
    printf("bh.biCompression: %d\n", bh.biCompression);
    printf("bh.biSizeImage: %d\n", bh.biSizeImage);
    printf("---\n");
    
    if( res != sizeof(BMPheader) ) { fclose(f); return img; }
    printf("Read header\n");

    // проверяем сигнатуру
    if( bh.bfType!=0x4d42 && bh.bfType!=0x4349 && bh.bfType!=0x5450 ) { fclose(f); return img; }
    printf("Check signature\n");

    // проверка размера файла
    fseek( f, 0, SEEK_END);
    int filesize = ftell(f);
    // восстановим указатель в файле:
    fseek( f, sizeof(BMPheader), SEEK_SET);
    
    // проверим условия
    if( bh.bfSize != filesize ||
        bh.bfReserved != 0    ||
        bh.biPlanes   != 1    ||
       (bh.biSize!=40 && bh.biSize!=108 && bh.biSize!=124)||
        bh.bfOffBits != 14+bh.biSize ||

        bh.biWidth <1 || bh.biWidth >10000 ||
        bh.biHeight<1 || bh.biHeight>10000 ||
        bh.biBitCount    != 24 ||             // пока рассматриваем только полноцветные изображения
        bh.biCompression !=  0                // пока рассматриваем только несжатие изображения
        ) 
    { 
            fclose(f); 
            return img; 
    }
    
    printf("Check conditions\n");

    // выделим память для результата
    img.width  = bh.biWidth;
    img.height = bh.biHeight;
    img.data = new Pixel[img.width*img.height];

    int mx3 = (3*img.width+3) & (-4);    // Compute row width in file, including padding to 4-byte boundary
    unsigned char *tmp_buf = new unsigned char[mx3*img.height];    // читаем данные
    res = fread( tmp_buf, 1, mx3*img.height, f);
    if( (int)res != mx3*img.height ) { delete []tmp_buf; fclose(f); return img; }
    printf("Read data\n");
    fclose(f); 

    // Перенос данных (не забудем про BGR->RGB)
    //unsigned char *ptr = (unsigned char *) v;
    Pixel * ptr = img.data;
    for(int y = img.height-1; y >= 0; y--) {
        unsigned char *pRow = tmp_buf + mx3*y;
        for(int x=0; x < img.width; x++) {
            (*ptr).r = *(pRow + 2);
            (*ptr).g = *(pRow + 1);
            (*ptr).b = *pRow;
            pRow+=3;
            ptr ++;
        }
    }
    delete []tmp_buf;
    return img;
}

int saveBMP( const char *fname, Image img)	// В каждом элементе упаковано все три RGB-байта
{
	BMPheader bh;	// Заголовок файла, sizeof(BMPheader) = 56
	memset( &bh, 0, sizeof(bh) );
	bh.bfType =0x4d42;	// 'BM'
    // Найдем длину строки в файле, включая округление вверх до кратного 4:
    int mx3 = (3*img.width+3) & (-4);
	int filesize = 54 + img.height*mx3;
	bh.bfSize = filesize;
	bh.bfReserved =  0;
	bh.biPlanes   =  1;
	bh.biSize     = 40;
	bh.bfOffBits  = 14 + bh.biSize;
	bh.biWidth    = img.width;
	bh.biHeight   = img.height;
	bh.biBitCount = 24;
	bh.biCompression= 0;

	FILE *f = fopen( fname, "wb" );
	if( !f ) return -1;
	size_t res;

	// пишем заголовок
	res = fwrite( &bh, 1, sizeof(BMPheader), f );
	if( res != sizeof(BMPheader) ) { fclose(f); return -1; }

	// приготовим временный буфер
	unsigned char *tmp_buf = new unsigned char[mx3*img.height];
	// Перенос данных (не забудем про RGB->BGR)
	//unsigned char *ptr = (unsigned char *) v;
    Pixel * ptr = img.data;
	for(int y = img.height-1; y >= 0; y--) {
		unsigned char *pRow = tmp_buf + mx3*y;
		for(int x=0; x < img.width; x++) {
            *(pRow + 2) = (*ptr).r;
			*(pRow + 1) = (*ptr).g;
			*pRow       = (*ptr).b; 
			pRow += 3;
			ptr++;
		}
	}
	// сбросим в файл
	fwrite( tmp_buf, 1, mx3*img.height, f );
	fclose(f);
	delete []tmp_buf;
	return 0;	// OK
}
