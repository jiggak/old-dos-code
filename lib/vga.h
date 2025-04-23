/*****************************************************************************
                          Graphic Video Modes ver3
                            By: Joshua Kropf

changes: 1. (v2)object oriented
            (v2.5)class Draw controls all
            (v3)class draw gets an address to a valid vga class

MCGA (320x200x256) chained [13h]
UN_MCGA (320x200x256) un-chained [13h]
MODEX (320x240x256) un-chained [13h]
VESA (640x400x256) un-chained [100h]
TEXT standard text mode [3h]

Virtual Screen based functions, (memory buffer)

*****************************************************************************/
#ifndef __VGA_H
#define __VGA_H

#include <dos.h>
#include <mem.h>
#include <alloc.h>

#define VGA_MEMORY (char far *)0xA0000000UL

typedef unsigned short int Coord_t;
typedef unsigned char Color_t;

typedef struct {
   unsigned char r;
   unsigned char g;
   unsigned char b;
} PalletReg;

class VgaMode {
   public:
      VgaMode(Coord_t width, Coord_t height, char unchained, unsigned long buffSize);

      // destructor returns to text mode
      ~VgaMode(void);

      void PutPixel(Coord_t x, Coord_t y, Color_t color);
      Color_t GetPixel(Coord_t x, Coord_t y);
      void PutHLine(Coord_t x1, Coord_t x2, Coord_t y, Color_t color);
      void ShowScreen(void);
      void Cls(Color_t color);

      // Pallet manipulating functions
      void SetPalletReg(Color_t colorID, char red ,char green, char blue);
      void GetPalletReg(Color_t colorID, char &red, char &green, char &blue);
      void SetPallet(PalletReg pallet[256]);

      inline Coord_t ScreenWidth() { return screenWidth; }
      inline Coord_t ScreenHeight() { return screenHeight; }

   private:
      // size of screen in pixels
      Coord_t screenWidth;
      Coord_t screenHeight;

      // MCGA,MODEY,MODEX,VESA Buffers
      // MCGA only uses Buffer[0]
      char far *buffer[4];

      char unchained;
      unsigned long buffSize;
};

VgaMode* ModeMCGA();
VgaMode* ModeY();
VgaMode* ModeX();
VgaMode* ModeVESA();

#endif