#include "fontpcx.h"
#include <string.h>


void FontPCX::PutC(int xoff, int yoff, char ch) {
   char x=0, y=0;
   unsigned int offset = (ch-32) * 64;

   for (ch=0; ch<64; ch++) {
      mode->PutPixel(x+xoff, y+yoff, pcx->buffer[offset+ch]);

      if (x < 7) {
         x++;
      } else {
         x=0;
         y++;
      }
	}
}

void FontPCX::PutS(int x, int y, char str[]) {
   for (int i=0; i<strlen(str); i++) {
      PutC(x, y, str[i]);
      x += 8;
   }
}
