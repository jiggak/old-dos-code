#ifndef __PCX_H
#define __PCX_H

#include "vga.h"


// structure to hold ALL image info
typedef struct {
   unsigned char far *buffer;
   unsigned short int Width;
   unsigned short int Height;
   PalletReg pallet[256];
} PCXbuffer;

// pcx file header
typedef struct {
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