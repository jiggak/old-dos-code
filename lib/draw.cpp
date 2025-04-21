#include "draw.h"
#include <math.h>


void Draw::PutCircle(int cx, int cy, int r, Color_t color) {
   int r2 = pow(r, 2);
   int i = 0;
   int delta = sqrt( abs(r2 - pow(i, 2)) );

   while (i<=delta) {
      mode->PutPixel(cx+delta, cy+i, color); // bottom right
      mode->PutPixel(cx+i, cy+delta, color); //
      mode->PutPixel(cx-delta, cy+i, color); // bottom left
      mode->PutPixel(cx-i, cy+delta, color); //
      mode->PutPixel(cx+delta, cy-i, color); // top right
      mode->PutPixel(cx+i, cy-delta, color); //
      mode->PutPixel(cx-delta, cy-i, color); // top left
      mode->PutPixel(cx-i, cy-delta, color); //

      i++;
      delta = sqrt( abs(r2 - pow(i, 2)) ) + 1;
   }
}


void Draw::PutFillCircle(int cx, int cy, int r, Color_t color) {
   int r2 = pow(r, 2);
   int i = 0;
   int delta = sqrt( abs(r2 - pow(i, 2)) );

   while (i<=delta) {
      mode->PutHLine(cx+delta, cx-delta, cy+i, color); // bottom
      mode->PutHLine(cx+i, cx-i, cy+delta, color);     //
      mode->PutHLine(cx+delta, cx-delta, cy-i, color); // top
      mode->PutHLine(cx+i, cx-i, cy-delta, color);     //

      i++;
      delta=sqrt( abs(r2 - pow(i, 2)) ) + 1;
   }
}


void Draw::PutVLine(int y1, int y2, int x, Color_t color) {
   char dir = y2 > y1 ? 1 : -1;

   for (int i=y1; i != (y2+dir); i += dir)
      mode->PutPixel(x, i, color);
}


void Draw::PutBox(int x1, int y1, int x2, int y2, Color_t color) {
   char xdir = x2 < x1? -1 : 1;
   char ydir = y2 < y1? -1 : 1;

   int i;

   for (i=y1; i != (y2+ydir); i+=ydir) {
      mode->PutPixel(x1, i, color);
      mode->PutPixel(x2, i, color);
   }

   for (i=x1; i != (x2+xdir); i+=xdir) {
      mode->PutPixel(i, y1, color);
      mode->PutPixel(i, y2, color);
   }
}


void Draw::PutLine(int x1, int y1, int x2, int y2, Color_t color) {
   int incr1,incr2,d,x,y,xend,yend,xinc,yinc;

   int dx = abs(x2-x1); // delta x //
   int dy = abs(y2-y1); // delta y //

   if (dx > dy) { // plot in the X direction //
      d = 2*dy-dx;
      incr1 = 2*dy;
      incr2 = 2*(dy-dx);

      if (x1 > x2) { // start at point with smaller x //
         x = x2;
         y = y2;
         xend = x1;
         yinc = y2 < y1? 1 : -1;
      } else {
         x = x1;
         y = y1;
         xend = x2;
         yinc = y1 < y2? 1 : -1;
      }

      mode->PutPixel(x, y, color);  // first point on the line segment //

      while (x < xend) {
         x = x + 1;
         if (d < 0) {
            d = d + incr1; // choose Si //
         } else {
            y = y + yinc;  // choose Ti //
            d = d + incr2;
         }

         mode->PutPixel(x, y, color);
      }
   } else { // plot in the Y direction //
      d = 2*dx-dy;
      incr1 = 2*dx;
      incr2 = 2*(dx-dy);

      if (y1 > y2) { // start at point with smaller y //
         x = x2;
         y = y2;
         yend = y1;
         xinc = x2 < x1? 1 : -1;
      } else {
         x = x1;
         y = y1;
         yend = y2;
         xinc = x1 < x2? 1 : -1;
      }

      mode->PutPixel(x, y, color);  // first point on the line segment //

      while (y < yend) {
         y = y+1;
         if (d < 0) {
            d = d + incr1; // choose Si //
         } else {
            x = x + xinc;  // choose Ti //
            d = d + incr2;
         }

         mode->PutPixel(x, y, color);
      }
   }
}