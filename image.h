#ifndef IMAGE_H 
#define IMAGE_H

#pragma pack(push, 1)
typedef struct {
  unsigned short bfType;
  unsigned int bfSize;
  unsigned short bfReserved1;
  unsigned short bfReserved2;
  unsigned int bfOffBits;
} BMPFILEHEADER;
 
typedef struct {
  unsigned int biSize;
  int biWidth;
  int biHeight;
  unsigned short biPlanes;
  unsigned short biBitCount;
  unsigned int   biCompression;
  unsigned int   biSizeImage;
  int biXPelsPerMeter;
  int biYPelsPerMeter;
  unsigned int   biClrUsed;
  unsigned int   biClrImportant;
} BMPINFOHEADER;

typedef struct {
  int R;
  int G;
  int B;      
} Pixel;

void leituraHeader(FILE *F, BMPFILEHEADER *H);

void leituraInfoHeader(FILE *F, BMPINFOHEADER *H);

void printFileHeader(BMPFILEHEADER *header);

void printInfoHeader(BMPINFOHEADER *header);

void loadBMPImage(FILE *input, BMPINFOHEADER InfoHeader, Pixel *Image, int compressionType);

#endif
