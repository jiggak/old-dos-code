#ifndef __FONT_H
#define __FONT_H

#include "vga.h"

class Font {
   private:
      VgaMode *mode;
      unsigned char far *fontRom; // dos ROM font pointer

   public:
      Font(VgaMode *mode);

      void PutC(int x, int y, char ch, Color_t color);
      void PutS(int x, int y, char str[], Color_t color);
};

#endif
