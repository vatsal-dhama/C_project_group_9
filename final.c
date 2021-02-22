#include <stdio.h>
#include <stdlib.h>
#pragma pack(1)

struct bmp_header {
    char identify[2];
    unsigned int size;
    int reserved;
    unsigned int image_offset;
};

struct dib_header {
    unsigned int dib_size;
    signed int width;
    signed int height;
    unsigned short int colour_planes;
    unsigned short int bits_per_pixel;
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
};

/*#########  BITMAP STRUCTURES DONE  #########*/

struct ICONDIR
{
    short int reserved;
    short int image_specification;
    short int num_images;
};
struct ICONDIRENTRY
{
    char image_width;  
    char image_height; 
    unsigned char num_colors;   //255
    unsigned char reserve;
    unsigned short int color_planes;
    unsigned short int bits_per_pixel;
    unsigned int image_size;
    unsigned int image_offset;
};

struct BitmapInfoHeader
{
    unsigned int header_size;        //sizeof(TBitmapInfoHeader)
    unsigned int width;              //width of bitmap
    unsigned int height;             //height of bitmap[biHeight field is set to 2 * ICONDIRENTRY.image_height]
    unsigned short int colourpanels; //1
    unsigned short int bitsperpixel; //24
    unsigned int compresion;         //0
    unsigned int imagesize;          //(ICONDIRENTRY.image_width * ICONDIRENTRY.image_height * 24) / 8 +
    unsigned int temp[4];            //(ICONDIRENTRY.image_width * ICONDIRENTRY.image_height) / 8
    
};
void writeico(struct dib_header dibheader, struct RGB picture[dibheader.height][dibheader.width]) {
    FILE *fpw;
    fpw = fopen("output.ico", "wb");
    
    struct ICONDIR ico_header;
    struct ICONDIRENTRY ico_dir_entry;
    struct BitmapInfoHeader bmp_ico_header;

    for(int x=0;x<4;x++){
    bmp_ico_header.temp[x]=0;}
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

    //printf("%d\n%d\n%d\n%d\n%d\n",ico_header.reserved, ico_header.image_specification, ico_header.num_images, ico_dir_entry.image_width, ico_dir_entry.image_height);

    ico_dir_entry.image_size = ico_dir_entry.image_width * ico_dir_entry.image_height * 3;
    ico_dir_entry.image_offset = 22;

    bmp_ico_header.header_size = 40;
    bmp_ico_header.width = dibheader.width;
    bmp_ico_header.height = dibheader.height * 2;
    bmp_ico_header.colourpanels = 1;
    bmp_ico_header.bitsperpixel = 32;

    int xor_mask = (ico_dir_entry.image_width * ico_dir_entry.image_height * bmp_ico_header.bitsperpixel) / 8;
    int and_mask = (ico_dir_entry.image_width * ico_dir_entry.image_height) / 8;

    //bmp_ico_header.imagesize = ico_dir_entry.image_size + ico_dir_entry.image_size / 32;
    bmp_ico_header.imagesize = xor_mask + and_mask;

    fwrite(&ico_dir_entry, sizeof(struct ICONDIRENTRY), 1, fpw);

    fwrite(&bmp_ico_header, sizeof(struct BitmapInfoHeader), 1, fpw);

    //fseek(fpw, ico_dir_entry.image_offset, SEEK_SET);

    //struct ICOImageData picture_ico = write_image(fpw, dibheader.height, dibheader.width);

    for (int i = dibheader.height - 1; i >= 0; i--) {
        for (int j = 0; j < dibheader.width; j ++) {
            fputc(picture[i][j].blue, fpw);
            fputc(picture[i][j].green, fpw);
            fputc(picture[i][j].red, fpw);
            fputc(255, fpw);
        }
        //fwrite(picture.rgb[i], sizeof(struct icoRGB), picture.width, fpw);
    }

    for (int i = 0; i < ico_dir_entry.image_size / 32; i++) {
        fputc(0, fpw);
    }
    fclose(fpw);
    //freeImageICO(picture_ico);
}

void read_bmp() {
    FILE *fp;
    fp = fopen("bitmapImage-2.bmp", "rb");

    struct bmp_header bmpheader;
    struct dib_header dibheader;

    fread(bmpheader.identify, 2, 1, fp);
    fread(&bmpheader.size, 12, 1, fp);
    
    fread(&dibheader, sizeof(struct dib_header), 1, fp);
    
    fseek(fp, bmpheader.image_offset, SEEK_SET);

    //read_image(fp, dibheader.height, dibheader.width);

    struct RGB picture[dibheader.height][dibheader.width];

    for (int i = dibheader.height - 1; i >= 0; i--) {
        for(int j = 0; j < dibheader.width; j++) {
            fread(&picture[i][j], sizeof(struct RGB), 1, fp);
        }
    }

    writeico(dibheader, picture);
    fclose(fp);
    //freeImage(picture);
}
int main() {
    read_bmp();
}
