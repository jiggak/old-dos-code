#ifndef __DRAW_CPP
#define __DRAW_CPP

// ***************** begin PCX class implementation ***************** //

char PCX::SetPCXbuffer(PCXbuffer *pcx,char fileName[])
{

FILE *file;

// open PCX file for read binary
if((file=fopen(fileName,"rb")) == NULL)
	return NULL; // error opening file (does not exist, invalid permisions)

PCXhead Pic;

if( (fread(&Pic,sizeof(PCXhead),1,file) ) == NULL )
	return NULL; // error reading PCX header info.

unsigned long TotalBytes,cnt=0;

pcx->Width=(Pic.right-Pic.left+1);  // set width of PCX
pcx->Height=(Pic.bottom-Pic.top+1); // set length of PCX

TotalBytes=(unsigned long)pcx->Width*(unsigned long)pcx->Height;

if((pcx->buffer=(unsigned char far *)farmalloc(TotalBytes)) == NULL)
	return 0; // error allocating memory for buffer


/******* (Decode) Read pixels *******/

unsigned char byte,RunLength;

// read all pcx data into buffer
while(cnt < TotalBytes)
	{

	fread(&byte,1,1,file);

// if upper two bits are set, lower six are run length
// if(bitCheck(byte,16) && bitCheck(byte,15))
	if(byte > 192)
		{
		RunLength = byte - 192; // get run length

		fread(&byte,1,1,file); // get the next byte in file

		// write the repeating byte to the buffer until runlength = 0
		while(RunLength -- > 0)
			{
			pcx->buffer[cnt]=byte;
			cnt++;
			}

		}

	else // if run length bits are not set, write the byte to the buffer
		{
		pcx->buffer[cnt]=byte;
		cnt++;
		}
	} // end read all pcx stuff into buffer

// End Read pixels


// begin Read pallet

unsigned short n;

// seek back from end of file 768 bytes
fseek(file,-768L,SEEK_END);

for(n=0;n<256;n++)
	{
	// read the pallet information RGB
	pcx->pallet.r[n]=fgetc(file);
	pcx->pallet.g[n]=fgetc(file);
	pcx->pallet.b[n]=fgetc(file);
	}

fclose(file); // close file

// End Read pallet

// pallet Must be set latter
SetPCXBufferPallet(*pcx);

return 1; // everything is good, return 1!

} // End PCX::SetPCXBuffer();



void FreePCXbuffer(PCXbuffer &buffer)
{

farfree(buffer.buffer);

}



void PCX::SetPCXBufferPallet(const PCXbuffer &pcx)
{

short i;

for(i=0;i<256;i++)
	pVgaMode->SetPalletReg(i,pcx.pallet.r[i]/4,pcx.pallet.g[i]/4,pcx.pallet.b[i]/4);

}



void PCX::PutPCXBuffer(int xoff,int yoff,const PCXbuffer &pcx)
{
unsigned long int n;
unsigned short int x=0,y=0;


for(n=0;n<((unsigned long)pcx.Width*(unsigned long)pcx.Height);n++)
	{
	pVgaMode->PutPixel(x+xoff,y+yoff,pcx.buffer[n]);

	if(x < (pcx.Width-1))
		x++;

	else
		{
		x=0;
		y++;
		}
	}

// sets the pallet aswell
SetPCXBufferPallet(pcx);

} // End PutPCXBuffer



char PCX::PutPCX(int xoff,int yoff,char FileName[])
{
FILE *file; // declaire file pointer //

if((file = fopen(FileName,"rb")) == NULL)
	return NULL; // unable to open pcx file

PCXhead Pic; // declaire PCX file header //

if((fread(&Pic,sizeof(PCXhead),1,file)) == NULL)
	return NULL; // error reading pcx header


unsigned char byte,RunLength;
unsigned int x=0,y=0,dx,dy;
unsigned long TotalBytes,cnt=0;

dx = (Pic.right-Pic.left)+1;
dy = (Pic.bottom-Pic.top)+1;

TotalBytes=(unsigned long)dx*(unsigned long)dy;

// (Decode) Read pixels
while(cnt < TotalBytes)
	{
	fread(&byte,1,1,file);

// if upper two bits are set, lower six are run length
// if(bitCheck(byte,16) && bitCheck(byte,15))
	if(byte > 192)
		{
		RunLength = byte - 192;

		fread(&byte,1,1,file);

		while(RunLength -- > 0)
			{
			pVgaMode->PutPixel(x+xoff,y+yoff,byte);
			cnt++;
			if(x < (dx-1))
				x++;
			else
				{
				x=0;
				y++;
				}
			}
		}

	else
		{
		pVgaMode->PutPixel(x+xoff,y+yoff,byte);
		cnt++;
		if(x < (dx-1))
			x++;
		else
			{
			x=0;
			y++;
			}
		}
	}

//End Read pixels //


// Read pallet and set it

unsigned short n;
unsigned char r,g,b;
PalletReg reg;

fseek(file,-768L,SEEK_END);

for(n=0;n<256;n++)
	{
	fread(&reg,sizeof(reg),1,file);
	// read red green and blue in that order

	pVgaMode->SetPalletReg(n,reg.r/4,reg.g/4,reg.b/4);
	}


fclose(file);

// End Read pallet //

return 1;

} // End PCX::PutPCX();




// ***************** begin Draw class implementation ***************** //


Draw::Draw(void)
	{

	pVgaMode=pstdvga;

	}

Draw::Draw(VgaMode *validMode)
	{

	pVgaMode=validMode;

   }


   
Draw::~Draw(void)
	{

   // constructor does not need to take any action

	}



void Draw::PutCircle(int cx,int cy,int r,unsigned char color)
{
int delta,i,r2;

r2=pow(r,2);
i=0;
delta=(sqrt( abs(r2 - pow(i,2)) ));

while(i<=delta)
	{
	pVgaMode->PutPixel(cx+delta,cy+i,color); // bottom right
	pVgaMode->PutPixel(cx+i,cy+delta,color); //
	pVgaMode->PutPixel(cx-delta,cy+i,color); // bottom left
	pVgaMode->PutPixel(cx-i,cy+delta,color); //
	pVgaMode->PutPixel(cx+delta,cy-i,color); // top right
	pVgaMode->PutPixel(cx+i,cy-delta,color); //
	pVgaMode->PutPixel(cx-delta,cy-i,color); // top left
	pVgaMode->PutPixel(cx-i,cy-delta,color); //
	i++;
	delta=(sqrt( abs(r2 - pow(i,2)) ))+1;
	}
} // End Draw::PutCircle()



void Draw::PutFillCircle(int cx,int cy,int r,unsigned char color)
{
int delta,i,r2;

r2=pow(r,2);
i=0;
delta=(sqrt( abs(r2 - pow(i,2)) ));

while(i<=delta)
	{
	pVgaMode->PutHLine(cx+delta,cx-delta,cy+i,color); // bottom
	pVgaMode->PutHLine(cx+i,cx-i,cy+delta,color);     //
	pVgaMode->PutHLine(cx+delta,cx-delta,cy-i,color); // top
	pVgaMode->PutHLine(cx+i,cx-i,cy-delta,color);     //
	i++;
	delta=(sqrt( abs(r2 - pow(i,2)) ))+1;
	}
}



void Draw::PutVLine(int y1,int y2,int x,unsigned char color)
{

char dir;
int k;

if(y2 > y1)
	dir = 1;

else
	dir = -1;

for(k=y1;k!=(y2+dir);k += dir)
	pVgaMode->PutPixel(x,k,color);

}



void Draw::PutBox(int x1,int y1,int x2,int y2,unsigned char color)
{
int k;
char xdir,ydir;

if(x2 < x1)
	xdir = -1;
else
	xdir = 1;

if(y2 < y2)
	ydir = -1;
else
	ydir = 1;


for(k=y1;k!=(y2+ydir);k+=ydir)
	{
	pVgaMode->PutPixel(x1,k,color);
	pVgaMode->PutPixel(x2,k,color);
	}

for(k=x1;k!=(x2+xdir);k+=xdir)
	{
	pVgaMode->PutPixel(k,y1,color);
	pVgaMode->PutPixel(k,y2,color);
	}

}



void Draw::PutLine(int x1,int y1,int x2,int y2,unsigned char color)
{
int dx,dy,incr1,incr2,d,x,y,xend,yend,xinc,yinc;

dx = abs(x2-x1); // delta x //
dy = abs(y2-y1); // delta y //

if (dx > dy) // plot in the X direction //
	{
	d = 2*dy-dx;
	incr1 = 2*dy;
	incr2 = 2*(dy-dx);
	if (x1 > x2) // start at point with smaller x //
		{
		x = x2;
		y = y2;
		xend = x1;
		if (y2 < y1)
			yinc = 1;
		else
			yinc = -1;
		}

	else
		{
		x = x1;
		y = y1;
		xend = x2;
		if (y1 < y2)
			yinc = 1;
		else
			yinc = -1;
		}

	pVgaMode->PutPixel(x,y,color);  // first point on the line segment //

	while(x < xend)
		{
		x = x + 1;
		if (d < 0)
			d=d+incr1; // choose Si //

		else
			{
			y= y + yinc;  // choose Ti //
			d= d + incr2;
			}

		pVgaMode->PutPixel(x,y,color);

		}
	}

else // plot in the Y direction //
	{
	d = 2*dx-dy;
	incr1 = 2*dx;
	incr2 = 2*(dx-dy);
	if (y1 > y2)  // start at point with smaller y //
		{
		x = x2;
		y = y2;
		yend = y1;
		if (x2 < x1)
			xinc = 1;
		else
			xinc = -1;
		}
	else
		{
		x = x1;
		y = y1;
		yend = y2;
		if (x1 < x2)
			xinc = 1;
		else
			xinc = -1;
		}
	pVgaMode->PutPixel(x,y,color);  // first point on the line segment //
	while(y < yend)
		{
		y = y+1;
		if (d < 0)
			d = d + incr1; // choose Si //
		else
			{
			x = x+xinc; // choose Ti //
			d = d+incr2;
			}
		pVgaMode->PutPixel(x,y,color);
		}
	}
}



// ***************** begin Font class implementation ***************** //



Font::Font(void)
{

SetPCXbuffer(&PCXfontBuffer,"font.pcx");

// get font info from ROM //
_AX=0x1130;
_BX=0x0400; // call 8x8 font -128 high characters (256)//
geninterrupt(0x10);

pFont=(char far *)MK_FP(_ES,_BP); // create font pointer //

}



Font::~Font(void)
{

farfree(PCXfontBuffer.buffer);

}



void Font::Gputc(int x,int y,char ch,unsigned char color)
{
unsigned char byte,k,xoff=0,yoff,bit;

ch = ch + 128;

for(yoff=0;yoff<8;yoff++)
	{
	// font pointer is in VGA.H
	byte = pFont[yoff+(ch*8)];

	for(bit=128;bit>=1;bit=bit/2)
		{
		xoff++;

		if(byte >= bit)
			{
			pVgaMode->PutPixel(xoff+x,yoff+y,color);
			byte=byte-bit;
			}
		// fill pack drop of character with 0
//    else
//       pVgaMode->PutPixel(xoff+x,yoff+y,0);

		}

	xoff=0;

	}

}



void Font::Pputc(int xoff,int yoff,char ch)
{
char x=0,y=0;
unsigned int offset;

offset=(ch-32)*64;

for(ch=0;ch<64;ch++)
	{
	pVgaMode->PutPixel(x+xoff,y+yoff,PCXfontBuffer.buffer[offset+ch]);

	if(x < 7)
		x++;
	else
		{
		x=0;
		y++;
		}
	}
}



void Font::Pprintf(int x,int y,char string[])
{
unsigned char k;


for(k=0;k<strlen(string);k++)
	{

	Pputc(x,y,string[k]);

	x += 8;

	}
}



void Font::Gprintf(int x,int y,char string[],unsigned char color)
{
unsigned char k;

for(k=0;k<strlen(string);k++)
	{

	Gputc(x,y,string[k],color);

	x += 8;

	}
}



void Font::Ggets(int x,int y,char *string,unsigned char color)
{
char ch;
unsigned cnt=0;

while((ch = getch()) != '\n')
	{
	if(ch != '\b')
		{
		string[cnt]=ch;
		Gputc(x+(cnt*8),y,ch,color);
		cnt++;
		}
	else
		{
		cnt--;
		Gputc(x+(cnt*8),y,' ',color);
		}
	pVgaMode->ShowScreen();
	}
string[cnt] = 0;

}



void Font::Pgets(int x,int y,char *string)
{
char ch;
unsigned cnt=0;

while((ch = getch()) != 13)
	{
	if(ch != '\b')
		{
		string[cnt]=ch;
		Pputc(x+(cnt*8),y,ch);
		cnt++;
		}
	else
		{
		cnt--;
		Pputc(x+(cnt*8),y,' ');
		}
	pVgaMode->ShowScreen();
	}

string[cnt] = 0;
}



// ***************** begin Draw3D class implementations ***************** //



Draw3D::Draw3D(void)
{

// constuctur sets perspective level
Perspective=256;

}



Draw3D::Draw3D(int pers)
{

SetPerspective(pers);

}



Draw3D::~Draw3D(void)
{
// destructor takes no action
}



void Draw3D::SetPerspective(int pers)
{

Perspective=pers;

}



void Draw3D::RotateObject(object3d *object,float angle1,float angle2,float angle3)
{
char i;
float temp[3];
float c1,c2,c3,s1,s2,s3;

c1=cos(angle1);
c2=cos(angle2);
c3=cos(angle3);

s1=sin(angle1);
s2=sin(angle2);
s3=sin(angle3);


for(i=0;i<object->MaxXYZ;i++)
	{

	temp[0]=(object->x[i]*((s1*s2*s3) + (c1*c3))) + (object->y[i]*(c2*s3)) + (object->z[i]*((c1*s2*s3) - (c3*s1)));
	temp[1]=(object->x[i]*((c3*s1*s2) - (c1*s3))) + (object->y[i]*(c2*c3)) + (object->z[i]*((c1*c3*s2) + (s1*s3)));
	temp[2]=(object->x[i]*(s1*c2)) - (object->y[i]*s2) + (object->z[i]*(c1*c2));

	object->x[i]=temp[0];
	object->y[i]=temp[1];
	object->z[i]=temp[2];

	}

}



void Draw3D::RotateXYZ(float &x,float &y,float &z,float angleX,float angleY,float angleZ)
{
float temp[3];
float c1,c2,c3,s1,s2,s3;

c1=cos(angleX);
c2=cos(angleY);
c3=cos(angleZ);

s1=sin(angleX);
s2=sin(angleY);
s3=sin(angleZ);

temp[0]=(x*((s1*s2*s3) + (c1*c3))) + (y*(c2*s3)) + (z*((c1*s2*s3) - (c3*s1)));
temp[1]=(x*((c3*s1*s2) - (c1*s3))) + (y*(c2*c3)) + (z*((c1*c3*s2) + (s1*s3)));
temp[2]=(x*(s1*c2)) - (y*s2) + (z*(c1*c2));

x=temp[0];
y=temp[1];
z=temp[2];

}

// *************** Sub Routines for PutFillPolygon ***************** //
int minus(int number,int max)                                        //
{                                                                    //
number--;         // lower numbers position in array                 //
if(number == -1)  //                                                 //
	number=max-1; //                                                 //
return number;                                                       //
}                                                                    //
int add(int number,int max)                                          //
{                                                                    //
number++;         //                                                 //
if(number == max) // up numbers position in array                    //
	number=0;     //                                                 //
return number;                                                       //
}                                                                    //
// ***************************************************************** //



void Draw3D::PutFillPolygon(int xoff,int yoff,object3d object,unsigned char color)
{

int dx[Max3DPoints],dy[Max3DPoints]; // calculated screen values
int mindx,mindy,maxdx,maxdy,i; // sort variables
char p1,p2,p3,p4; // array elements, line end points
int y,start,end=0; // loop variables
float x1,x2,ratio1,ratio2; // calculation variables
float yratio; // divide by zero corection variable

// calculate plotable points for drawing
for(i=0;i<object.MaxXYZ;i++)
	{
	dx[i]=(Perspective * object.x[i]) / (Perspective + object.z[i]);
	dy[i]=(Perspective * object.y[i]) / (Perspective + object.z[i]);
	}

// Get max and min values of x and y
maxdx=mindx=dx[0];
maxdy=mindy=dy[0];

for(i=0;i<object.MaxXYZ;i++)
	{

	if(dx[i] > maxdx)
		maxdx=dx[i];

	else if(dx[i] < mindx)
		mindx=dx[i];

	if(dy[i] > maxdy)
		maxdy=dy[i];

	else if(dy[i] < mindy)
		mindy=dy[i];

	}
// End Get max and min


for(i=0;i<object.MaxXYZ;i++)
	{
	// check to find mindy value in array with the smallest x value
	if(dy[i] == mindy)
		{
		p1=i; // find FIRST element with min y value
		break;
		}
	}

//p2 = one position lower than p1 in array
p2=minus(p1,object.MaxXYZ);

if(dy[p1] == dy[add(p1,object.MaxXYZ)]) // check that its not a horizontal line
	p3=add(p1,object.MaxXYZ); // if so, next point in array
else
	p3=p1; // if not straight line, same point as p1

//p4 = one position lower than p3 in array
p4=add(p3,object.MaxXYZ);

// start will always begin with p1!
start=dy[p1]-1;
// -1 to put value of start one short, to compensate for y=start+1

// BEGIN LOOP
while(1 != 0)
	{
	// find minimum of two values
	// inside loop because end will always be smaller of p2/p4
	end=( (dy[p2] < dy[p4]) ? dy[p2] : dy[p4]);

	// calculat ratios for line calculations
	yratio=(float)(dy[p2]-dy[p1]);
	if(yratio == 0.0)
		yratio=0.000001;
	ratio1=(float)(dx[p2]-dx[p1])/yratio;

	yratio=(float)(dy[p4]-dy[p3]);
	if(yratio == 0.0)
		yratio=0.000001;
	ratio2=(float)(dx[p4]-dx[p3])/yratio;

	for(y=start+1;y<=end;y++)
		{
		x1=(float)(y-dy[p1])*ratio1+(float)dx[p1];

		x2=(float)(y-dy[p3])*ratio2+(float)dx[p3];

		pVgaMode->PutHLine(x1+xoff,x2+xoff,y+yoff,color);
		}

	if(end == dy[p2]) // if p2 was the end point
		{
		p1=minus(p1,object.MaxXYZ); // lower p1's position in array

		p2=minus(p2,object.MaxXYZ); // lower p2's position in array

		start=dy[p1];
		}

	else // if p4 was the end point
		{
		p3=add(p3,object.MaxXYZ); // up! p3's position in array

		p4=add(p4,object.MaxXYZ); // up! p4's position in array

		start=dy[p3];
		}

	// check for possiblility of straight line in array
	if(dy[p2] == dy[p1]) // check for line in p1/p2
		{
		p1=minus(p1,object.MaxXYZ); // if line exists, move both points in
		p2=minus(p2,object.MaxXYZ); // proper diriction in array
		}

	else if(dy[p4] == dy[p3]) // check for line in p3/p4
		{
		p3=add(p3,object.MaxXYZ); // if line exists, move both points in
		p4=add(p4,object.MaxXYZ); // proper diriction in array
		}
	// end checking

	if(end >= maxdy) // if end has reached max y values, break loop
		break;   //

	}
	// END LOOP

}



void Draw3D::Put3DLine(float x1,float y1,float z1,float x2,float y2,float z2,unsigned char color)
{
int dx1,dx2,dy1,dy2;

// calculate screen co-ordinates
dx1=(Perspective * x1) / (Perspective + z1);
dy1=(Perspective * y1) / (Perspective + z1);
dx2=(Perspective * x2) / (Perspective + z2);
dy2=(Perspective * y2) / (Perspective + z2);

PutLine(dx1,dy1,dx2,dy2,color); // draw the line

}



void Draw3D::Put3DPixel(float x,float y,float z,unsigned char color)
{
int dx,dy;
dx=(Perspective * x) / (Perspective + z); // calcualte plottable points
dy=(Perspective * y) / (Perspective + z); //

pVgaMode->PutPixel(dx,dy,color); // put plottable point

}



void Draw3D::PutMeshObject(int xoff,int yoff,int zoff,object3d object,unsigned char color)
{
char i;

for(i=0;i<object.MaxPoints;i++)
	{
	Put3DLine(xoff+object.x[object.cpa[i]],yoff+object.y[object.cpa[i]],
		zoff+object.z[object.cpa[i]],xoff+object.x[object.cpb[i]],
			yoff+object.y[object.cpb[i]],zoff+object.z[object.cpb[i]],color);
	}

}

#endif