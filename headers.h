
struct bmp_header {
    char identify[2];                   // This is for the identification of BMP file.
    unsigned int size;                  // The size of the BMP file in bytes.
    int reserved;                       // Reserved; actual value depends on the application that creates the image, if created manually can be 0
    unsigned int image_offset;          // The offset, i.e. starting address, of the byte where the bitmap image data (pixel array) can be found.
};

struct dib_header {
    unsigned int dib_size;              // The size of this header, in bytes (40)
    signed int width;                   // The bitmap width in pixels
    signed int height;                  // The bitmap height in pixels
    unsigned short int colour_planes;   // The number of color planes (must be 1)
    unsigned short int bits_per_pixel;  // The number of bits per pixel, which is the color depth of the image. 
    unsigned int compression;           // The compression method being used.
    unsigned int image_size;            // The image size. This is the size of the raw bitmap data
    unsigned int res[4];                //
};

struct RGB {                            //structure of pixel array
    unsigned char blue;
    unsigned char green;
    unsigned char red;

};

struct ImageData {                      // Structure of image data
    int height;
    int width;
};

/*#########  BITMAP STRUCTURES DONE  #########*/

struct ICONDIR
{
    short int reserved;                 // Always set to 0 for .ico files
    short int image_specification;      // Always set to 1 for .ico files
    short int num_images;               // Number of icon images
};
struct ICONDIRENTRY
{
    char image_width;                   // Width
    char image_height;                  // Height
    unsigned char num_colors;           // Number of colours used
    unsigned char reserve;              // Reserved
    unsigned short int color_planes;    // Reserved
    unsigned short int bits_per_pixel;  // Reserved
    unsigned int image_size;            // Total number of bytes in images
    unsigned int image_offset;          // Location of image from the beginning of file
};

struct BitmapInfoHeader
{
    unsigned int header_size;           // Size of this header in Bytes
    unsigned int width;                 // Width of bitmap
    unsigned int height;                // Height of bitmap = 2*ICONDIRENTRY.image_height
    unsigned short int colourpanels;    // Always 1
    unsigned short int bitsperpixel;    // The number of bits per pixel, which is the color depth of the image. 
    unsigned int compresion;            // The compression method being used(0)
    unsigned int imagesize;             // Size of the pixel data = sum of XOR mask and AND mask
    //XOR mask: (ICONDIRENTRY.image_width * ICONDIRENTRY.image_height * bitsperpixel) / 8
    //AND mask: (ICONDIRENTRY.image_width * ICONDIRENTRY.image_height) / 8
    
    // Not important
    unsigned int temp1;                 
    unsigned int temp2;
    unsigned int temp3;
    unsigned int temp4;
};
/*#########  ICO STRUCTURES DONE  #########*/


void read_bmp();

