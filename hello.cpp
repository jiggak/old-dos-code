#include <conio.h>
#include <stdio.h>
#include "vga.h"
#include "font.h"

void main()
{
   VgaMode *mode = ModeX();
   Font font(mode);

   while (!kbhit()) {
      mode->Cls(100);

      font.PutS(100, 100, "Hello, World!", 56);

      mode->ShowScreen();
   }

   delete mode;

   printf("Existing\n");
}