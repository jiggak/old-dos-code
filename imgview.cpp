#include <stdio.h>
#include <conio.h>
#include "vga.h"
#include "draw.h"


void main(int argc, char *argv[]) {
   if (argc != 2) {
      printf("Missing file path argument\n");
      return;
   }

   PCXbuffer *pcx = PCXLoadFile(argv[1]);
   if (!pcx) {
      printf("Error loading pcx\n");
      return;
   }

   VgaMode *mode = ModeMCGA();
   Draw draw(mode);

   mode->Cls(0);

   draw.SetPallet(pcx);
   draw.PutPCX(0, 0, pcx);

   mode->ShowScreen();

   getch();

   delete mode;
   PCXFree(pcx);
}