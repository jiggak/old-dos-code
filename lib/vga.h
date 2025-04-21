/*****************************************************************************
                          Graphic Video Modes ver3
                            By: Joshua Kropf

changes: 1. (v2)object oriented
            (v2.5)class Draw controls all
            (v3)class draw gets an address to a valid vga class

MCGA (320x200x256) chained
UN_MCGA (320x200x256) un-chained
MODEX (320x240x256) un-chained
VESA (640x400x256) un-chained
TEXT standard text mode

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

/*
typedef struct ModeNums
   {
   int
      TEXT = 0x03, // mode 3h
      MCGA = 0x0013, // mode 13h
      UN_MCGA = 0x0013, // mode 13h
      MODEX = 0x0013, // mode 13h
      VESA = 0x0100; // mode 100h
   } ModeNums;
*/

// enum Modes { TEXT,MCGA,UN_MCGA,MODEX,VESA };

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
      void GetPalletReg(Color_t colorID, char &red, char &green, char &blue);
      void SetPalletReg(Color_t colorID, char red ,char green, char blue);

      inline Coord_t ScreenWidth() { return screenWidth; }
      inline Coord_t ScreenHeight() { return screenHeight; }

   private:
      // size of screen in pixels
      Coord_t screenWidth;
      Coord_t screenHeight;

      // MCGA,UN_MCGA,MODEX,VESA Buffers
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