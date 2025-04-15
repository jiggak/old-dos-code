#ifndef __VGA_CPP
#define __VGA_CPP

void VgaMode::UnChain(void)
	{
	// Turn off the Chain-4 bit (bit 3 at index 4, port 0x3c4) //

	outport(0x3c4, 0x0604);

	// Turn off word mode, by setting the Mode Control register
	// of the CRT Controller (index 0x17, port 0x3d4)

	outport(0x3d4, 0xE317);

	// Turn off doubleword mode, by setting the Underline Location
	// register (index 0x14, port 0x3d4)

	outport(0x3d4, 0x0014);

	} // end UnChain()



VgaMode::VgaMode(void)
	{

	vga=(char far *)0xA0000000UL; // set vga pointer

	}



VgaMode::~VgaMode(void)
	{

	_AX=0x0003; // init text mode
	geninterrupt(0x10);

	}



void VgaMode::PutPixel(unsigned short int x,unsigned short int y,unsigned char color)
	{

	// unsigned variable are always greater >= 0 so... no condition needed
	if(x <= maxx && y <= maxy)
		Buffer[x&3][((maxx+1) / 4 * y) + (x/4)] = color;

	}



unsigned char VgaMode::GetPixel(unsigned short int x,unsigned short int y)
	{

	// return color value at offset/buffer
	// unsigned variable are always greater >= 0 so... no condition needed
	if(x <= maxx && y <= maxy)
		return Buffer[x&3][((maxx+1) / 4 * y)+(x/4)];

	else
		return NULL;

	}



void VgaMode::PutHLine(unsigned short int x1,unsigned short int x2,unsigned short int y,unsigned char color)
	{
	char dir;
	int k;

	if(x2 > x1)
		dir = 1;
	else
		dir = -1;

	for(k=x1;k!=(x2+dir);k += dir)
		PutPixel(k,y,color);

	}



void VgaMode::SetPalletReg(unsigned char colorID,char red,char green,char blue)
	{

	outportb(0x3C8,colorID);

	outportb(0x3C9,red);
	outportb(0x3C9,green);
	outportb(0x3C9,blue);

	}



void VgaMode::GetPalletReg(unsigned char colorID,char &red,char &green,char &blue)
	{

	outportb(0x3C7,colorID);

	red=inportb(0x3C9);
	green=inportb(0x3C9);
	blue=inportb(0x3C9);

	}



ModeMCGA::ModeMCGA(void)
	{

	_AX=0x0013; // init mode MCGA
	geninterrupt(0x10);

	maxx=319; // assign screen max values
	maxy=199;

	// allocate drawing buffer
	Buffer[0]=(char far *)farmalloc(0xFA00);

	// set global pointer !! Temporary !!
	pstdvga=this;

	}



ModeMCGA::~ModeMCGA(void)
	{

	// free allocated memory
	farfree(Buffer[0]);

	}


void ModeMCGA::PutPixel(unsigned short int x,unsigned short int y,unsigned char color)
	{

	// unsigned variable are always greater >= 0 so... no condition needed
	if(x <= maxx && y <= maxy)
		Buffer[0][((maxx+1)*y)+x]=color;

	}



unsigned char ModeMCGA::GetPixel(unsigned short int x,unsigned short int y)
	{
	// return color value at offset/buffer
	// unsigned variable are always greater >= 0 so... no condition needed
	if(x <= maxx && y <= maxy)
		return Buffer[0][((maxx+1)*y)+x];

	else
		return NULL;

	}



void ModeMCGA::ShowScreen(void)
	{

	_fmemcpy(vga,Buffer[0],0xFA00);

	}



void ModeMCGA::Cls(unsigned char color)
	{

	_fmemset(Buffer[0],color,0xFA00);

	}



void ModeMCGA::PutHLine(unsigned short int x1,unsigned short int x2,unsigned short int y,unsigned char color)
	{
	int line;
	int tmp;

	if(x1 > x2)
		{
		tmp=x1;
		x1=x2;
		x2=tmp;
		}

	line=((320*y)+x2)-((320*y)+x1); // total size of line

	_fmemset(&Buffer[0][(320*y)+x1],color,line);

	}



ModeUN_MCGA::ModeUN_MCGA(void)
	{

	_AX=0x0013; // set mode UN_MCGA
	geninterrupt(0x10);

	UnChain(); // Un-Chain the mode

	maxx=319; // assign screen max values
	maxy=199;

	char i;
	for(i=0;i<4;i++) // allocate buffers
		Buffer[i]=(char far *)farmalloc(0x3E80);

	// set global pointer !! Temporary !!
	pstdvga=this;

	}



ModeUN_MCGA::~ModeUN_MCGA(void)
	{

	// free allocated memory
	char i;
	for(i=0;i<4;i++)
		farfree(Buffer[i]);

	}



void ModeUN_MCGA::ShowScreen(void)
	{

	char i;

	// for i=plane0 .. i=plane3 //
	// select proper write plane and copy entire buffer section to vga pointer //

	for(i=0;i<4;i++)
		{
		outportb(0x3C4, 0x02);
		outportb(0x3C5, 0x01 << i);

		_fmemcpy(vga,Buffer[i&3],0x3E80);
		}

	}



void ModeUN_MCGA::Cls(unsigned char color)
	{
	char i;
	// Set all four plane buffers to color

	for(i=0;i<4;i++)
		_fmemset(Buffer[i],color,0x3E80);

	}



ModeMODEX::ModeMODEX(void)
	{

	_AX=0x0013; // set mode MODEX
	geninterrupt(0x10);

	UnChain(); // Un-Chain the mode

	// ModeX stuff begins //

	/* Modify the vertical sync polarity bits in the Misc. Output
		Register to achieve square aspect ratio */

	outportb(0x3C2, 0xE3);

	outport(0x3D4, 0x2C11);         /* turn off write protect */

	outport(0x3D4, 0x0D06);         /* vertical total */

	outport(0x3D4, 0x3E07);         /* overflow register */

	outport(0x3D4, 0xEA10);         /* vertical retrace start */

	outport(0x3D4, 0xAC11);         /* vertical retrace end AND wr.prot */

	outport(0x3D4, 0xDF12);         /* vertical display enable end */

	outport(0x3D4, 0xE715);         /* start vertical blanking */

	outport(0x3D4, 0x0616);         /* end vertical blanking */

	maxx=319; // assign screen max values
	maxy=239;

	char i;
	for(i=0;i<4;i++) // allocate buffers
		Buffer[i]=(char far *)farmalloc(0x4B00);

	// set global pointer !! Temporary !!
	pstdvga=this;


	}



ModeMODEX::~ModeMODEX(void)
	{

	// free allocated memory
	char i;
	for(i=0;i<4;i++)
		farfree(Buffer[i]);

	}



void ModeMODEX::ShowScreen(void)
	{

	char i;

	// for i=plane0 .. i=plane3 //
	// select proper write plane and copy entire buffer section to vga pointer //

	for(i=0;i<4;i++)
		{
		outportb(0x3C4, 0x02);
		outportb(0x3C5, 0x01 << i);

		_fmemcpy(vga,Buffer[i&3],0x4B00);
		}

	}



void ModeMODEX::Cls(unsigned char color)
	{
	char i;
	// Set all four plane buffers to color

	for(i=0;i<4;i++)
		_fmemset(Buffer[i],color,0x4B00);

	}



ModeVESA::ModeVESA(void)
	{

	_AX=0x4F02;
	_BX=0x0100; // set vesa mode 640x400x256
	geninterrupt(0x10);

	UnChain(); // un-chain the mode

	maxx=639; // assign screen max values
	maxy=399;

	char i;
	for(i=0;i<4;i++) // allocate buffers
		Buffer[i]=(char far *)farmalloc(0xFA00);

	// set global pointer !! Temporary !!
	pstdvga=this;


	}



ModeVESA::~ModeVESA(void)
	{

	// free allocated memory
	char i;
	for(i=0;i<4;i++)
		farfree(Buffer[i]);

	}


void ModeVESA::ShowScreen(void)
	{

	char i;

	// for i=plane0 .. i=plane3 //
	// select proper write plane and copy entire buffer section to vga pointer //

	for(i=0;i<4;i++)
		{
		outportb(0x3C4, 0x02);
		outportb(0x3C5, 0x01 << i);
//      outport(0x3C4,0x100 << (i &3) +2);

		_fmemcpy(vga,Buffer[i&3],0xFA00);
		}

	}



void ModeVESA::Cls(unsigned char color)
	{
	char i;
	// Set all four plane buffers to color

	for(i=0;i<4;i++)
		_fmemset(Buffer[i],color,0xFA00);

	}

#endif