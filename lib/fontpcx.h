#ifndef __FONTPCX_H
#define __FONTPCX_H

#include "vga.h"
#include "pcx.h"

/*************************************************************************
Configuration of PCX for font printing:

1. the file must be character uppon character verticly
	eg: A
		 B
		 C

2. the characters must be 8x8 pixels

3. the characters must be in increasing ASCII order from 32 - 127
*************************************************************************/
class FontPCX {
   private:
      VgaMode *mode;
      PCXbuffer *pcx;

   public:
      FontPCX(VgaMode *mode, PCXbuffer *pcx) : mode(mode), pcx(pcx) { }

      void PutC(int x, int y, char ch);
      void PutS(int x, int y, char str[]);
};

#endif
