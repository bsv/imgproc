#include <stdio.h>
#include "bmp.h"
#include "dsp.h"

void toGrayScale(Pixel img[], int mx, int my) {
    for(int y = 0; y < my/2; y++) {
        for(int x = 0; x < mx; x++) {
            img[y*mx + x].g = 0;
            img[y*mx + x].b = 0;
        }
    }
}

int main(int argc, char *argv[]) {
    
    Image img;
    Image res_img;
    
    if(argc != 2) {
        printf("Please specify the file name: lab2 <filename>\n");
        return 0;
    }
    
    img = loadBMP(argv[1]);
    
    if(img.data != NULL) {
        printf("The image %s %d x %d px is opened successfully\n", argv[1], img.width, img.height);
    } else {
        printf("Error: The image %s can not be opened\n", argv[1]);
    }

    res_img = img;
    res_img.data = new Pixel[res_img.width*res_img.height];

    sobel(img, res_img);
    printf("Save img\n");
    saveBMP("out.bmp", res_img);
    
    delete [] img.data;
    delete [] res_img.data;
    
    return 0;
}