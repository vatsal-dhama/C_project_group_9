#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#pragma pack(1)
struct bmp_header {
    char identify[2];
    unsigned int size;
    int reserved;
    unsigned int offset;
};

struct dib_header {
    unsigned int dib_size;
    signed int width;
    signed int height;
    unsigned short int colour;
    unsigned short int bpp;
    unsigned int compression;
    unsigned int image_size;
    unsigned int res[4];


};

struct RGB {
    unsigned char blue;
    unsigned char green;
    unsigned char red;

};

struct ImageData {
    int height;
    int width;
    unsigned char *blue;
    unsigned char *green;
    unsigned char *red;
};

struct ImageData read_image(FILE *fp, int height, int width) {
    struct ImageData picture;
    picture.blue = (unsigned char *) malloc(width / 4 * sizeof(char*));
    picture.green = (unsigned char *) malloc(width / 4 * sizeof(char*));
    picture.red = (unsigned char *) malloc(width / 4 * sizeof(char*));
    picture.height = height;
    picture.width = width;

    for (int i = picture.height - 1; i >= 0; i--) {
        for (int j = 0; j < 3 * picture.width; j++) {
            if (j % 3 == 0)
                fread(*(picture.blue + j / 3), 1, 1, fp);
            else if (j % 3 == 1)
                fread(*(picture.green + j / 3), 1, 1, fp);
            else
                fread(*(picture.red + j / 3), 1, 1, fp);
        }
        /*
        picture.rgb[i] = (struct RGB*) malloc(width * sizeof(struct RGB));
        fread(picture.rgb[i], sizeof(struct RGB), width, fp);
        */
    }
    return picture;
}

void freeImage(struct ImageData picture) {
    for (int i = picture.height - 1; i >= 0; i--) {
        for (int j = 0; j < 3 * picture.width; j++) {
            if (j % 3 == 0)
                free(*(picture.blue + j / 3));
            else if (j % 3 == 1)
                free(*(picture.green + j / 3));
            else
                free(*(picture.red + j / 3));
        }
    }
}
void writebmp(struct bmp_header, struct dib_header, struct ImageData);

void read_bmp() {
    FILE *fp;
    fp = fopen("New-Bitmap-Image.bmp", "rb");

    struct bmp_header bmpheader;
    struct dib_header dibheader;

    fread(bmpheader.identify, 2, 1, fp);
    fread(&bmpheader.size, 12, 1, fp);

    //printf("First 2 chars: %c%c\n", bmpheader.identify[0], bmpheader.identify[1]);
    //printf("Size: %d\n", bmpheader.size);
    //printf("Offset: %d\n",bmpheader.offset);
    
    fread(&dibheader, sizeof(struct dib_header), 1, fp);
    
    //printf("Header Size: %d\nWidth: %d\nHeight: %d\nColour Planes: %d\nBits Per Pixel: %d\nCompression: %d\nImage Size: %d\n",dibheader.dib_size, dibheader.width, dibheader.height, dibheader.colour, dibheader.bpp, dibheader.compression, dibheader.image_size);
    
    fseek(fp, bmpheader.offset, SEEK_SET);

    struct ImageData picture = read_image(fp, dibheader.height, dibheader.width);

    writebmp(bmpheader, dibheader, picture);
    fclose(fp);
    freeImage(picture);
}

void writebmp(struct bmp_header bmpheader, struct dib_header dibheader, struct ImageData picture) {
    FILE *fpw;
    fpw = fopen("write3.bmp", "wb");

    fwrite(bmpheader.identify, 2, 1, fpw);

    fwrite(&bmpheader.size, 12, 1, fpw);

    fwrite(&dibheader, sizeof(struct dib_header), 1, fpw);

    for (int i = picture.height - 1; i >= 0; i--) {
        for (int j = 0; j < picture.width; j += 4) {
            fputc(picture.blue[j], fpw);
            fputc(picture.green[j + 1], fpw);
            fputc(picture.red[j + 2], fpw);
            fputc(0, fpw);
        }              
        //fwrite(picture.rgb[i], sizeof(struct icoRGB), picture.width, fpw);
    }
    fclose(fpw);
}

int main() {
    read_bmp();
}
