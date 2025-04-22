/****************************************************************************
                     Various Drawing Functions
                      PutPixel Based Functions

simple yet sort of powerfull graphics functions
v2 object oriented edition

Nov 19/99
(c) Josh Kropf
****************************************************************************/
#ifndef __DRAW_H
#define __DRAW_H

#include "vga.h"
#include "pcx.h"


class Draw {
   private:
      VgaMode *mode;

   public:
      Draw::Draw(VgaMode *mode) : mode(mode) { }

      void PutCircle(int cx, int cy, int r, Color_t color);
      void PutFillCircle(int cx, int cy, int r, Color_t color);
      void PutVLine(int y1, int y2, int x, Color_t color);
      void PutBox(int x1, int y1, int x2, int y2, Color_t color);
      void PutLine(int x1, int y1, int x2, int y2, Color_t color);

      void PutPCX(int xoff, int yoff, PCXbuffer *pcx);
      void SetPallet(PCXbuffer *pcx);

      inline void PutPixel(Coord_t x, Coord_t y, Color_t color) {
         mode->PutPixel(x, y, color);
      }

      inline void PutHLine(Coord_t x1, Coord_t x2, Coord_t y, Color_t color) {
         mode->PutHLine(x1, x2, y, color);
      }
};

#endif
