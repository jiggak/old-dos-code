/****************************************************************************
				Various Drawing Functions
				 PutPixel Based Functions

simple yet sort of powerfull graphics functions
v2 object oriented edition

Nov 19/99
(c) Josh Kropf
****************************************************************************/

#ifndef __VGA_H
#include "vga.h"
#endif

#ifndef __DRAW_H
#define __DRAW_H

#ifndef __STDIO_H
#include <stdio.h>
#endif

#ifndef __STRING_H
#include <string.h>
#endif

#ifndef __CONIO_H
#include <conio.h>
#endif

#ifndef __MATH_H
#include <math.h>
#endif

typedef struct Pallet // 256 color pallet structure
	{
	unsigned char r[256];
	unsigned char g[256];
	unsigned char b[256];
	} Pallet;


typedef struct PalletReg // pallet register structure
	{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	} PalletReg;


// structure to hold ALL image info
typedef struct PCXbuffer
	{
	unsigned char far *buffer;
	unsigned short int Width;
	unsigned short int Height;
	Pallet pallet;
	}PCXbuffer;

// pcx file header
typedef struct PCXhead
	{
	unsigned char manuf; // manufacturer
	unsigned char version; // version number 0=v2.5 2=v2.8(pallet)
					 // 3=v2.8(no pallet) 4=paint(v5) 5=v3> (24 bit)
	unsigned char encoding; // rle <run length encoding>
	unsigned char bpp; // BitsPerPixel
	unsigned short left;   // window! image dimentions
	unsigned short top;    // 8unsigned chars, 2unsigned chars per dimension
	unsigned short right;  //
	unsigned short bottom; //
	unsigned short hres; // resolution DotsPerInch
	unsigned short vres; // 2 unsigned chars per dim
	unsigned char colormap[48]; // color palett
	unsigned char reserved; // reserved set to 0
	unsigned char nplanes; // number of color planes
	unsigned short bpl; // number of unsigned chars to allocate for a scan line
	unsigned short pinfo; // palett info
	unsigned char filler[58]; // just set to 0's
	} PCXhead;


#define MaxPCXs 16
#define Max3DPoints 256

// 3 dimensional object //
// Mesh based //

typedef struct object3d
	{
	unsigned char MaxXYZ; // number of points in xyz array
	unsigned char MaxPoints; // number of points in cpa/cpb array

	double x[Max3DPoints]; // XYZ virtual co-ordinates
	double y[Max3DPoints]; //
	double z[Max3DPoints]; //

	unsigned char cpa[Max3DPoints/2]; // line conector points
	unsigned char cpb[Max3DPoints/2]; //

	} object3d;



class Draw
	{

	public:
		//constructor sets mode
		Draw(void);
      Draw(VgaMode *validMode); // constuctor sets vgaMode3 poointer
		~Draw(void); //destructor
		void PutCircle(int cx,int cy,int r,unsigned char color);
		void PutFillCircle(int cx,int cy,int r,unsigned char color);
		void PutVLine(int y1,int y2,int x,unsigned char color);
		void PutBox(int x1,int y1,int x2,int y2,unsigned char color);
		void PutLine(int x1,int y1,int x2,int y2,unsigned char color);
		// VgaModes class pointer
		VgaMode *pVgaMode;

	};



class PCX : public Draw
	{

	public:
		char SetPCXbuffer(PCXbuffer *buffer,char fileName[]);
      void FreePCXbuffer(PCXbuffer &buffer);
		void SetPCXBufferPallet(const PCXbuffer &buffer);
		void PutPCXBuffer(int xoff,int yoff,const PCXbuffer &buffer);
		char PutPCX(int xoff,int yoff,char FileName[]);

	};



/*************************************************************************
Configuration of PCX for font printing:

1. the file must be character uppon character verticly
	eg: A
		 B
		 C

2. the characters must be 8x8 pixels

3. the characters must be in increasing ASCII order from 32 - 127
*************************************************************************/
class Font : public PCX
	{

	private:
		PCXbuffer PCXfontBuffer; // area in memory for pcx font file
		char far *pFont; // dos ROM font pointer

	public:
		Font(void);  //font constructor sets PCXfontBuffer, and font pointer
		~Font(void); //free's PCXfontBuffer
		// put character in graphics mode
		void Gputc(int x,int y,char ch,unsigned char color);
		void Pputc(int xoff,int yoff,char ch);
		void Pprintf(int x,int y,char string[]);
		void Gprintf(int x,int y,char string[],unsigned char color);
		void Ggets(int x,int y,char *string,unsigned char color);
		void Pgets(int x,int y,char *string);

	};



class Draw3D: public Draw
	{
	public:
		Draw3D(void);
		Draw3D(int pers); //constuctor initializes perspective
		~Draw3D(void); //destructor takes no actions
		inline void SetPerspective(int pers);
		void RotateObject(object3d *object,float angle1,float angle2,float angle3);
		void RotateXYZ(float &x,float &y,float &z,float angleX,float angleY,float angleZ);
		void PutFillPolygon(int xoff,int yoff,object3d object,unsigned char color);
		void Put3DLine(float x1,float y1,float z1,float x2,float y2,float z2,unsigned char color);
		void Put3DPixel(float x,float y,float z,unsigned char color);
		void PutMeshObject(int xoff,int yoff,int zoff,object3d object,unsigned char color);

	private:
		short int Perspective; // Perspective level

	};

	#ifndef __DRAW_CPP
	#include "draw.cpp"
	#endif

#endif