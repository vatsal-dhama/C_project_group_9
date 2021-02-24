#include <stdio.h>
#include <stdlib.h>
#pragma pack(1)
#include "headers.h"

void write_ico(FILE *fpw, struct dib_header dibheader, struct RGB picture[dibheader.height][dibheader.width]);

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

    write_ico(fpw, dibheader, picture);
    fclose(fp);
}

// Converting data appropriately and writing into the ICO file.

void write_ico(FILE *fpw, struct dib_header dibheader, struct RGB picture[dibheader.height][dibheader.width]) {

    // Using User Defined ICO structures.

    struct ICONDIR ico_header;
    struct ICONDIRENTRY ico_dir_entry;
    struct BitmapInfoHeader bmp_ico_header;


    // Hard coding header values which are fixed for all ICO images.
    ico_header.reserved = 0;
    ico_header.image_specification = 1;
    ico_header.num_images = 1;

    fwrite(&ico_header.reserved, 6, 1, fpw);

    // The length and width of the ICO image is same as the BMP image.
    ico_dir_entry.image_width = dibheader.width;
    ico_dir_entry.image_height = dibheader.height;
    // fixed values for all ICO images.
    ico_dir_entry.num_colors = 0;
    ico_dir_entry.reserve = 0;
    ico_dir_entry.color_planes = 1;
    ico_dir_entry.bits_per_pixel = 32;
    // ICO images have a reserved channel after RGB values in each pixel, hence we multiply width * height by 4.
    ico_dir_entry.image_size = ico_dir_entry.image_width * ico_dir_entry.image_height * 4;
    ico_dir_entry.image_offset = 22;

    bmp_ico_header.header_size = 40;
    bmp_ico_header.width = dibheader.width;
    // We need to multiply the height by 2 as we need to apply two masks.
    bmp_ico_header.height = dibheader.height * 2;
    bmp_ico_header.colourpanels = 1;
    bmp_ico_header.bitsperpixel = 32;

    // We need to use XOR mask and AND mask to remove irregulaties on the screen which will compromise the image.
    int xor_mask = (ico_dir_entry.image_width * ico_dir_entry.image_height * bmp_ico_header.bitsperpixel) / 8;
    int and_mask = (ico_dir_entry.image_width * ico_dir_entry.image_height) / 8;

    bmp_ico_header.imagesize = xor_mask + and_mask;
    // Writing the values into the defined structures of the ICO file.
    fwrite(&ico_dir_entry, sizeof(struct ICONDIRENTRY), 1, fpw);

    fwrite(&bmp_ico_header, sizeof(struct BitmapInfoHeader), 1, fpw);

    // Writing the image data into the ICO file.
    for (int i = dibheader.height - 1; i >= 0; i--) {
        for (int j = 0; j < dibheader.width; j ++) {
            fputc(picture[i][j].blue, fpw);
            fputc(picture[i][j].green, fpw);
            fputc(picture[i][j].red, fpw);      // This is the Alpha channel which takes care of the transparency of the image.
            fputc(255, fpw);
        }
    }
    // These are zeroes added to the end of the image as a mask. The size of the mask is 1/32th of the size of the image.
    //These may not be required, but it is a good practice to use them.
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
