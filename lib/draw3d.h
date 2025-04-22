#ifndef __DRAW3D_H
#define __DRAW3D_H

#include "draw.h"

#define Max3DPoints 256

// 3 dimensional object //
// Mesh based //

typedef struct {
   unsigned char MaxXYZ; // number of points in xyz array
   unsigned char MaxPoints; // number of points in cpa/cpb array

   double x[Max3DPoints]; // XYZ virtual co-ordinates
   double y[Max3DPoints]; //
   double z[Max3DPoints]; //

   unsigned char cpa[Max3DPoints/2]; // line conector points
   unsigned char cpb[Max3DPoints/2]; //
} object3d;

class Draw3D {
   private:
      Draw& draw;
      short int perspective;

   public:
      Draw3D(Draw& draw) : draw(draw) {
         perspective = 256;
      }

      inline short int GetPerspective() { return perspective; }
      inline void SetPerspective(short int v) { perspective = v; }

      void PutFillPolygon(int xoff, int yoff, object3d object, Color_t color);
      void Put3DLine(float x1, float y1, float z1, float x2, float y2, float z2, Color_t color);
      void Put3DPixel(float x, float y, float z, Color_t color);
      void PutMeshObject(int xoff, int yoff, int zoff, object3d object, Color_t color);
};

void RotateXYZ(float &x, float &y, float &z, float angleX, float angleY, float angleZ);
void RotateObject(object3d *object, float angle1, float angle2, float angle3);

#endif