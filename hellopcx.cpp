#include <conio.h>
#include <stdio.h>
#include "vga.h"
#include "fontpcx.h"

void main()
{
   VgaMode *mode = ModeX();
   PCXbuffer *pcx = PCXLoadFile("font.pcx");
   FontPCX font(mode, pcx);

   mode->SetPallet(pcx->pallet);

   while (!kbhit()) {
      mode->Cls(0);

      font.PutS(100, 100, "Hello, World!");

      mode->ShowScreen();
   }

   PCXFree(pcx);
   delete mode;

   printf("Existing\n");
}