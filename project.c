#include <stdio.h>
#include <stdlib.h>
#pragma pack(1)
#include "headers.h"

void writeico(FILE *fpw, struct dib_header dibheader, struct RGB picture[dibheader.height][dibheader.width]);

void read_bmp(FILE *fp, FILE *fpw) {
    struct bmp_header bmpheader;
    struct dib_header dibheader;

    fread(bmpheader.identify, 2, 1, fp);
    fread(&bmpheader.size, 12, 1, fp);
    
    fread(&dibheader, sizeof(struct dib_header), 1, fp);
    
    fseek(fp, bmpheader.image_offset, SEEK_SET);

    struct RGB picture[dibheader.height][dibheader.width];

    for (int i = dibheader.height - 1; i >= 0; i--) {
        for(int j = 0; j < dibheader.width; j++) {
            fread(&picture[i][j], sizeof(struct RGB), 1, fp);
        }
    }

    writeico(fpw, dibheader, picture);
    fclose(fp);
}

void writeico(FILE *fpw, struct dib_header dibheader, struct RGB picture[dibheader.height][dibheader.width]) {

    struct ICONDIR ico_header;
    struct ICONDIRENTRY ico_dir_entry;
    struct BitmapInfoHeader bmp_ico_header;


    ico_header.reserved = 0;
    ico_header.image_specification = 1;
    ico_header.num_images = 1;

    fwrite(&ico_header.reserved, 6, 1, fpw);

    ico_dir_entry.image_width = dibheader.width;
    ico_dir_entry.image_height = dibheader.height;
    ico_dir_entry.num_colors = 0;
    ico_dir_entry.reserve = 0;
    ico_dir_entry.color_planes = 1;
    ico_dir_entry.bits_per_pixel = 32;

    ico_dir_entry.image_size = ico_dir_entry.image_width * ico_dir_entry.image_height * 4;
    ico_dir_entry.image_offset = 22;

    bmp_ico_header.header_size = 40;
    bmp_ico_header.width = dibheader.width;
    bmp_ico_header.height = dibheader.height * 2;
    bmp_ico_header.colourpanels = 1;
    bmp_ico_header.bitsperpixel = 32;

    int xor_mask = (ico_dir_entry.image_width * ico_dir_entry.image_height * bmp_ico_header.bitsperpixel) / 8;
    int and_mask = (ico_dir_entry.image_width * ico_dir_entry.image_height) / 8;

    bmp_ico_header.imagesize = xor_mask + and_mask;

    fwrite(&ico_dir_entry, sizeof(struct ICONDIRENTRY), 1, fpw);

    fwrite(&bmp_ico_header, sizeof(struct BitmapInfoHeader), 1, fpw);

    for (int i = dibheader.height - 1; i >= 0; i--) {
        for (int j = 0; j < dibheader.width; j ++) {
            fputc(picture[i][j].blue, fpw);
            fputc(picture[i][j].green, fpw);
            fputc(picture[i][j].red, fpw);
            fputc(255, fpw);
        }
    }

    for (int i = 0; i < ico_dir_entry.image_size / 32; i++) {
        fputc(0, fpw);
    }
    fclose(fpw);
}

int main( int argc, char *argv[] ) {
    FILE *fp, *fpw;
    fp = fopen(argv[1], "rb");
    fpw = fopen(argv[2], "wb");
    read_bmp(fp, fpw);
}