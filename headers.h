
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
    unsigned char num_colors;
    unsigned char reserve;
    unsigned short int color_planes;
    unsigned short int bits_per_pixel;
    unsigned int image_size;
    unsigned int image_offset;
};

struct BitmapInfoHeader
{
    unsigned int header_size;
    unsigned int width;
    unsigned int height;
    unsigned short int colourpanels;
    unsigned short int bitsperpixel;
    unsigned int compresion;
    unsigned int imagesize;         
    unsigned int temp1;
    unsigned int temp2;
    unsigned int temp3;
    unsigned int temp4;
};
/*#########  ICO STRUCTURES DONE  #########*/


void read_bmp();

