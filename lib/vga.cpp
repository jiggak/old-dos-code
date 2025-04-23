#include "vga.h"


VgaMode::VgaMode(Coord_t width, Coord_t height, char unchained, unsigned long buffSize) {
   screenWidth = width;
   screenHeight = height;
   this->unchained = unchained;
   this->buffSize = buffSize;

   if (unchained) {
      for (int i=0; i<4; i++) {
         buffer[i] = (char far *)farmalloc(buffSize);
      }
   } else {
      buffer[0] = (char far *)farmalloc(buffSize);
      buffer[1] = buffer[2] = buffer[3] = NULL;
   }
}


VgaMode::~VgaMode(void) {
   _AX=0x0003; // text mode
   geninterrupt(0x10);

   for (int i=0; i<4; i++) {
      if (buffer[i]) {
         farfree(buffer[i]);
      }
   }
}


void VgaMode::PutPixel(Coord_t x, Coord_t y, Color_t color) {
   // unsigned variable are always greater >= 0 so... no condition needed
   if (x < screenWidth && y < screenHeight) {
      if (unchained) {
         buffer[x&3][(screenWidth / 4 * y) + (x/4)] = color;
      } else {
         buffer[0][(screenWidth * y) + x] = color;
      }
   }
}


Color_t VgaMode::GetPixel(Coord_t x, Coord_t y) {
   // return color value at offset/buffer
   // unsigned variable are always greater >= 0 so... no condition needed
   if (x < screenWidth && y < screenHeight) {
      if (unchained) {
         return buffer[x&3][(screenWidth / 4 * y) + (x/4)];
      } else {
         return buffer[0][(screenWidth * y) + x];
      }
   }

   return 0;
}


void VgaMode::PutHLine(Coord_t x1, Coord_t x2, Coord_t y, Color_t color) {
   if (unchained) {
      char dir = x2 > x1? 1 : -1;

      for (int i=x1; i!=(x2+dir); i += dir)
         PutPixel(i, y, color);
   } else {
      if (x1 > x2) {
         Coord_t tmp = x1;
         x1 = x2;
         x2 = tmp;
      }

      // total size of line
      int line = ((screenWidth * y) + x2) - ((screenWidth * y) + x1);

      _fmemset(&buffer[0][(screenWidth * y) + x1], color, line);
   }
}


void VgaMode::ShowScreen(void) {
   if (unchained) {
      // select proper write plane and copy entire buffer section to vga pointer

      for (int i=0; i<4; i++) {
         outportb(0x3C4, 0x02);
         outportb(0x3C5, 0x01 << i);

         _fmemcpy(VGA_MEMORY, buffer[i], buffSize);
      }
   } else {
      _fmemcpy(VGA_MEMORY, buffer[0], buffSize);
   }
}


void VgaMode::Cls(Color_t color) {
   if (unchained) {
      for (int i=0; i<4; i++)
         _fmemset(buffer[i], color, buffSize);
   } else {
      _fmemset(buffer[0], color, buffSize);
   }
}


void VgaMode::SetPalletReg(Color_t colorID, char red, char green, char blue) {
   outportb(0x3C8, colorID);

   outportb(0x3C9, red);
   outportb(0x3C9, green);
   outportb(0x3C9, blue);
}


void VgaMode::GetPalletReg(Color_t colorID, char &red, char &green, char &blue) {
   outportb(0x3C7, colorID);

   red = inportb(0x3C9);
   green = inportb(0x3C9);
   blue = inportb(0x3C9);
}


void VgaMode::SetPallet(PalletReg pallet[256]) {
   for (int i=0; i<256; i++) {
      SetPalletReg(i, pallet[i].r/4, pallet[i].g/4, pallet[i].b/4);
   }
}


void UnChain(void) {
   // Turn off the Chain-4 bit (bit 3 at index 4, port 0x3c4) //
   outport(0x3c4, 0x0604);

   // Turn off word mode, by setting the Mode Control register
   // of the CRT Controller (index 0x17, port 0x3d4)
   outport(0x3d4, 0xE317);

   // Turn off doubleword mode, by setting the Underline Location
   // register (index 0x14, port 0x3d4)
   outport(0x3d4, 0x0014);
}


VgaMode* ModeMCGA() {
   // init mode MCGA
   _AX=0x0013;
   geninterrupt(0x10);

   return new VgaMode(320, 200, 0, 0xFA00);
}


VgaMode* ModeY() {
   // init mode MCGA
   _AX=0x0013;
   geninterrupt(0x10);

   UnChain(); // un-chain the mode

   return new VgaMode(320, 200, 1, 0x3E80);
}


VgaMode* ModeX() {
   // init mode MCGA
   _AX=0x0013;
   geninterrupt(0x10);

   UnChain(); // Un-Chain the mode

   // ModeX stuff begins //

   /* Modify the vertical sync polarity bits in the Misc. Output
      Register to achieve square aspect ratio */

   outportb(0x3C2, 0xE3);

   outport(0x3D4, 0x2C11);         /* turn off write protect */

   outport(0x3D4, 0x0D06);         /* vertical total */

   outport(0x3D4, 0x3E07);         /* overflow register */

   outport(0x3D4, 0xEA10);         /* vertical retrace start */

   outport(0x3D4, 0xAC11);         /* vertical retrace end AND wr.prot */

   outport(0x3D4, 0xDF12);         /* vertical display enable end */

   outport(0x3D4, 0xE715);         /* start vertical blanking */

   outport(0x3D4, 0x0616);         /* end vertical blanking */

   return new VgaMode(320, 240, 1, 0x4B00);
}


VgaMode* ModeVESA() {
   _AX=0x4F02;
   _BX=0x0100; // set vesa mode 640x400x256
   geninterrupt(0x10);

   UnChain(); // un-chain the mode

   return new VgaMode(640, 400, 1, 0xFA00);
}
