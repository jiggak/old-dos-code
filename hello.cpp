#include <conio.h>
#include <stdio.h>
#include "vga.h"
#include "font.h"

void main()
{
   VgaMode *mode = ModeX();
   Font font(mode);

   while (!kbhit()) {
      mode->Cls(5);

      font.PutS(100, 100, "Hello, World!", 10);

      mode->ShowScreen();
   }

   delete mode;

   printf("Existing\n");
}