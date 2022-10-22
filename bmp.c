#include <stdlib.h>
#include <stdio.h>
#include "color.h"
#include "bmp.h"


typedef struct {
    int file_size, reserved, offset, DIB_size, width, height, compression, data_size, X_resolution, Y_resolution, Number_of_colors, Number_of_important_colors;
    short plane;
    short depth;
} heading_t;


heading_t heading(size_t w,size_t h,int d) {
    int R = 4 * (d * w + 31) / 32, M = h * R, C;
    if (d == 1 || d == 8) C = 2^d;
    else if (d == 24) C = 0;
    int P = 4 * C;
    heading_t head;
    head.file_size = 54 + P + M;
    head.reserved = 0;
    head.offset = 54 + P;
    head.DIB_size = 40;
    head.width = w;
    head.height = h;
    head.plane = 1;
    head.depth = d;
    head.compression = 0;
    head.data_size = M;
    head.X_resolution = 2835;
    head.Y_resolution = 2835;
    head.Number_of_colors = C;
    head.Number_of_important_colors = 0;
    return head;
}


void bmp_save(const char* fname, size_t width, size_t height, size_t depth, color_t (*pixel)(size_t i, size_t j, void** args), void** args) {
    FILE *fd = fopen(fname,"w+");
    /*if (depth == 1) {
        char tab[4][2];
    } else if (depth == 8) {
        char tab[4][256];        
    }*/
    //color_t c = 
    pixel(width, height, args);//for not having a warning, because function not finished
    pixel(width, depth, args);//for not having a warning, because function not finished
    fclose(fd);
}
