#ifndef __PCX_H
#define __PCX_H


typedef struct Pallet { // 256 color pallet structure
   unsigned char r[256];
   unsigned char g[256];
   unsigned char b[256];
} Pallet;


typedef struct PalletReg { // pallet register structure
   unsigned char r;
   unsigned char g;
   unsigned char b;
} PalletReg;


// structure to hold ALL image info
typedef struct PCXbuffer {
   unsigned char far *buffer;
   unsigned short int Width;
   unsigned short int Height;
   Pallet pallet;
} PCXbuffer;

// pcx file header
typedef struct PCXhead {
   unsigned char manuf; // manufacturer
   unsigned char version; // version number 0=v2.5 2=v2.8(pallet)
                          // 3=v2.8(no pallet) 4=paint(v5) 5=v3> (24 bit)
   unsigned char encoding; // rle <run length encoding>
   unsigned char bpp; // BitsPerPixel
   unsigned short left;   // window! image dimentions
   unsigned short top;    // 8unsigned chars, 2unsigned chars per dimension
   unsigned short right;  //
   unsigned short bottom; //
   unsigned short hres; // resolution DotsPerInch
   unsigned short vres; // 2 unsigned chars per dim
   unsigned char colormap[48]; // color palett
   unsigned char reserved; // reserved set to 0
   unsigned char nplanes; // number of color planes
   unsigned short bpl; // number of unsigned chars to allocate for a scan line
   unsigned short pinfo; // palett info
   unsigned char filler[58]; // just set to 0's
} PCXhead;


PCXbuffer* PCXLoadFile(char* filePath);
void PCXFree(PCXbuffer* pcx);


#endif