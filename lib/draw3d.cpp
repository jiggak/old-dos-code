#include <math.h>
#include "draw3d.h"


void RotateXYZ(float &x, float &y, float &z, float angleX, float angleY, float angleZ) {
   float temp[3];

   float c1=cos(angleX);
   float c2=cos(angleY);
   float c3=cos(angleZ);

   float s1=sin(angleX);
   float s2=sin(angleY);
   float s3=sin(angleZ);

   temp[0]=(x*((s1*s2*s3) + (c1*c3))) + (y*(c2*s3)) + (z*((c1*s2*s3) - (c3*s1)));
   temp[1]=(x*((c3*s1*s2) - (c1*s3))) + (y*(c2*c3)) + (z*((c1*c3*s2) + (s1*s3)));
   temp[2]=(x*(s1*c2)) - (y*s2) + (z*(c1*c2));

   x=temp[0];
   y=temp[1];
   z=temp[2];
}


void RotateObject(object3d *object, float angle1, float angle2, float angle3) {
   float temp[3];

   float c1=cos(angle1);
   float c2=cos(angle2);
   float c3=cos(angle3);

   float s1=sin(angle1);
   float s2=sin(angle2);
   float s3=sin(angle3);

   for (int i=0; i<object->MaxXYZ; i++) {
      temp[0]=(object->x[i]*((s1*s2*s3) + (c1*c3))) + (object->y[i]*(c2*s3)) + (object->z[i]*((c1*s2*s3) - (c3*s1)));
      temp[1]=(object->x[i]*((c3*s1*s2) - (c1*s3))) + (object->y[i]*(c2*c3)) + (object->z[i]*((c1*c3*s2) + (s1*s3)));
      temp[2]=(object->x[i]*(s1*c2)) - (object->y[i]*s2) + (object->z[i]*(c1*c2));

      object->x[i]=temp[0];
      object->y[i]=temp[1];
      object->z[i]=temp[2];
   }
}


// *************** Sub Routines for PutFillPolygon ***************** //
int minus(int number, int max)  {
   number--;        // lower numbers position in array
   if(number == -1) //
      number=max-1; //
   return number;
}
int add(int number, int max) {
   number++;         //
   if(number == max) // up numbers position in array
      number=0;      //
   return number;
}
// ***************************************************************** //


void Draw3D::PutFillPolygon(int xoff, int yoff, object3d object, Color_t color) {
   int dx[Max3DPoints],dy[Max3DPoints]; // calculated screen values
   int mindx,mindy,maxdx,maxdy,i; // sort variables
   char p1,p2,p3,p4; // array elements, line end points
   int y,start,end=0; // loop variables
   float x1,x2,ratio1,ratio2; // calculation variables
   float yratio; // divide by zero corection variable

   // calculate plotable points for drawing
   for (i=0;i<object.MaxXYZ;i++) {
      dx[i]=(perspective * object.x[i]) / (perspective + object.z[i]);
      dy[i]=(perspective * object.y[i]) / (perspective + object.z[i]);
   }

   // Get max and min values of x and y
   maxdx=mindx=dx[0];
   maxdy=mindy=dy[0];

   for (i=0; i<object.MaxXYZ; i++) {

      if (dx[i] > maxdx)
         maxdx=dx[i];

      else if (dx[i] < mindx)
         mindx=dx[i];

      if (dy[i] > maxdy)
         maxdy=dy[i];

      else if (dy[i] < mindy)
         mindy=dy[i];

   } // End Get max and min


   for (i=0; i<object.MaxXYZ; i++) {
      // check to find mindy value in array with the smallest x value
      if(dy[i] == mindy) {
         p1=i; // find FIRST element with min y value
         break;
      }
   }

   //p2 = one position lower than p1 in array
   p2=minus(p1,object.MaxXYZ);

   if (dy[p1] == dy[add(p1,object.MaxXYZ)]) // check that its not a horizontal line
      p3=add(p1,object.MaxXYZ); // if so, next point in array
   else
      p3=p1; // if not straight line, same point as p1

   //p4 = one position lower than p3 in array
   p4=add(p3,object.MaxXYZ);

   // start will always begin with p1!
   start=dy[p1]-1;
   // -1 to put value of start one short, to compensate for y=start+1

   // BEGIN LOOP
   for (;;) {
      // find minimum of two values
      // inside loop because end will always be smaller of p2/p4
      end=( (dy[p2] < dy[p4]) ? dy[p2] : dy[p4]);

      // calculat ratios for line calculations
      yratio=(float)(dy[p2]-dy[p1]);
      if (yratio == 0.0)
         yratio=0.000001;
      ratio1=(float)(dx[p2]-dx[p1])/yratio;

      yratio=(float)(dy[p4]-dy[p3]);
      if (yratio == 0.0)
         yratio=0.000001;
      ratio2=(float)(dx[p4]-dx[p3])/yratio;

      for (y=start+1;y<=end;y++) {
         x1=(float)(y-dy[p1])*ratio1+(float)dx[p1];

         x2=(float)(y-dy[p3])*ratio2+(float)dx[p3];

         draw.PutHLine(x1+xoff,x2+xoff,y+yoff,color);
      }

      if (end == dy[p2]) // if p2 was the end point
         {
         p1=minus(p1,object.MaxXYZ); // lower p1's position in array

         p2=minus(p2,object.MaxXYZ); // lower p2's position in array

         start=dy[p1];
      }

      else { // if p4 was the end point
         p3=add(p3,object.MaxXYZ); // up! p3's position in array

         p4=add(p4,object.MaxXYZ); // up! p4's position in array

         start=dy[p3];
      }

      // check for possiblility of straight line in array
      if (dy[p2] == dy[p1]) { // check for line in p1/p2
         p1=minus(p1,object.MaxXYZ); // if line exists, move both points in
         p2=minus(p2,object.MaxXYZ); // proper diriction in array
      }

      else if (dy[p4] == dy[p3]) { // check for line in p3/p4
         p3=add(p3,object.MaxXYZ); // if line exists, move both points in
         p4=add(p4,object.MaxXYZ); // proper diriction in array
      }
      // end checking

      if (end >= maxdy) // if end has reached max y values, break loop
         break;         //

   }  // END LOOP
}


void Draw3D::Put3DLine(float x1, float y1, float z1, float x2, float y2, float z2, Color_t color) {
   // calculate screen co-ordinates
   int dx1=(perspective * x1) / (perspective + z1);
   int dy1=(perspective * y1) / (perspective + z1);
   int dx2=(perspective * x2) / (perspective + z2);
   int dy2=(perspective * y2) / (perspective + z2);

   draw.PutLine(dx1, dy1, dx2, dy2, color); // draw the line
}


void Draw3D::Put3DPixel(float x, float y, float z, Color_t color) {
   int dx=(perspective * x) / (perspective + z); // calcualte plottable points
   int dy=(perspective * y) / (perspective + z); //

   draw.PutPixel(dx, dy, color); // put plottable point
}


void Draw3D::PutMeshObject(int xoff, int yoff, int zoff, object3d object, Color_t color) {
   for (int i=0; i<object.MaxPoints; i++) {
      Put3DLine(
         xoff+object.x[object.cpa[i]],
         yoff+object.y[object.cpa[i]],
         zoff+object.z[object.cpa[i]],
         xoff+object.x[object.cpb[i]],
         yoff+object.y[object.cpb[i]],
         zoff+object.z[object.cpb[i]],
         color
      );
   }
}