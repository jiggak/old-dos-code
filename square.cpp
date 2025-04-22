#include <conio.h>
#include <math.h>
#include "vga.h"
#include "draw.h"
#include "draw3d.h"


void main() {
   VgaMode *mode = ModeMCGA();
   Draw draw(mode);
   Draw3D draw3d(draw);

   object3d box1;
   object3d box2;
   object3d box3;
   object3d box4;

   box1.MaxXYZ=4;
   box1.x[0]=40;
   box1.y[0]=40;
   box1.z[0]=40;
   box1.x[1]=40;
   box1.y[1]=-40;
   box1.z[1]=40;
   box1.x[2]=-40;
   box1.y[2]=-40;
   box1.z[2]=40;
   box1.x[3]=-40;
   box1.y[3]=40;
   box1.z[3]=40;

   box2.MaxXYZ=4;
   box2.x[0]=40;
   box2.y[0]=40;
   box2.z[0]=40;
   box2.x[1]=40;
   box2.y[1]=40;
   box2.z[1]=-40;
   box2.x[2]=40;
   box2.y[2]=-40;
   box2.z[2]=-40;
   box2.x[3]=40;
   box2.y[3]=-40;
   box2.z[3]=40;

   box3.MaxXYZ=4;
   box3.x[0]=40;
   box3.y[0]=40;
   box3.z[0]=-40;
   box3.x[1]=40;
   box3.y[1]=-40;
   box3.z[1]=-40;
   box3.x[2]=-40;
   box3.y[2]=-40;
   box3.z[2]=-40;
   box3.x[3]=-40;
   box3.y[3]=40;
   box3.z[3]=-40;

   box4.MaxXYZ=4;
   box4.x[0]=-40;
   box4.y[0]=40;
   box4.z[0]=40;
   box4.x[1]=-40;
   box4.y[1]=40;
   box4.z[1]=-40;
   box4.x[2]=-40;
   box4.y[2]=-40;
   box4.z[2]=-40;
   box4.x[3]=-40;
   box4.y[3]=-40;
   box4.z[3]=40;

   char a,b,c;
   char string[50];
   int dx,dy;

   while (!kbhit()) {
      mode->Cls(0);

      /*
      a=random(6);
      b=random(6);
      c=random(6);
      RotateObject(box1,a*(M_PI/180),b*(M_PI/180),c*(M_PI/180));
      RotateObject(box2,a*(M_PI/180),b*(M_PI/180),c*(M_PI/180));
      RotateObject(box3,a*(M_PI/180),b*(M_PI/180),c*(M_PI/180));
      RotateObject(box4,a*(M_PI/180),b*(M_PI/180),c*(M_PI/180));
      */

      RotateObject(&box1, 2*(M_PI/180), 2*(M_PI/180), 2*(M_PI/180));
      RotateObject(&box2, 2*(M_PI/180), 2*(M_PI/180), 2*(M_PI/180));
      RotateObject(&box3, 2*(M_PI/180), 2*(M_PI/180), 2*(M_PI/180));
      RotateObject(&box4, 2*(M_PI/180), 2*(M_PI/180), 2*(M_PI/180));

      // for(a=0;a<4;a++) {
      //    dx=(Perspective * box3.x[a]) / (Perspective + box3.z[a]);
      //    dy=(Perspective * box3.y[a]) / (Perspective + box3.z[a]);

      //    sprintf(string,"%d %d",dx,dy);
      //    // sprintf(string,"%f %f %f",box3.x[a],box3.y[a],box3.z[a]);
      //    Gprintf(0,a*8,string,15);
      // }

      draw3d.PutFillPolygon(160, 100, box1, 18);
      draw3d.PutFillPolygon(160, 100, box2, 19);
      draw3d.PutFillPolygon(160, 100, box3, 15);
      draw3d.PutFillPolygon(160, 100, box4, 21);

      mode->ShowScreen();
   }

   delete mode;
}
