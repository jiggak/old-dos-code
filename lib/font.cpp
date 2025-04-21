#include "font.h"
#include <dos.h>
#include <string.h>


Font::Font(VgaMode *mode) {
   this->mode = mode;

   _AX = 0x1130; // get font info from ROM
   _BX = 0x0300; // get addr of ROM 8x8 font
   geninterrupt(0x10);

   // address of requested font-definition table
   fontRom = (unsigned char far *)MK_FP(_ES, _BP);
}

void Font::PutC(int x, int y, char ch, Color_t color) {
   int xoff = 0;

   // this assumes we are using 8x8 font
   for (int yoff=0; yoff<8; yoff++) {
      unsigned char line = fontRom[(ch * 8) + yoff];

      for (int b=7; b>=0; b--) {
         if (line & (1 << b)) {
            mode->PutPixel(xoff+x, yoff+y, color);
         } // else we could fill pixel with some other color for text background

         xoff++;
      }

      xoff = 0;
   }
}

void Font::PutS(int x, int y, char str[], Color_t color) {
   for (int i=0; i<strlen(str); i++) {
      PutC(x, y, str[i], color);
      x += 8;
   }
}
