/*****************************************************************************
								  Graphic Video Modes ver3
									 By: Joshua Kropf

changes: 1. (v2)object oriented
				(v2.5)class Draw controls all
            (v3)class draw gets an address to a valid vga class

MCGA (320x200x256) chained
UN_MCGA (320x200x256) un-chained
MODEX (320x240x256) un-chained
VESA (640x400x256) un-chained
TEXT standard text mode

Virtual Screen based functions, (memory buffer)

*****************************************************************************/

#ifndef __DOS_H
#include <dos.h>
#endif

#ifndef __MEM_H
#include <mem.h>
#endif

#ifndef __ALLOC_H
#include <alloc.h>
#endif

#ifndef __VGA_H
#define __VGA_H

/*
typedef struct ModeNums
	{
	int
		TEXT = 0x03, // mode 3h
		MCGA = 0x0013, // mode 13h
		UN_MCGA = 0x0013, // mode 13h
		MODEX = 0x0013, // mode 13h
		VESA = 0x0100; // mode 100h
	} ModeNums;
*/

enum Modes { TEXT,MCGA,UN_MCGA,MODEX,VESA };

class VgaMode
	{

	public:

		VgaMode(void); // constructor sets mode and initializes buffers
		~VgaMode(void); // destructor returns old mode and frees buffers
		void UnChain(void); // executes un-chaining instructions

		// possibly over-riden functions
		virtual void PutPixel(unsigned short int x,unsigned short int y,unsigned char color);
		virtual unsigned char GetPixel(unsigned short int x,unsigned short int y);
		// horizontal line drawing
		virtual void PutHLine(unsigned short int x1,unsigned short int x2,unsigned short int y,unsigned char color);
		// ShowScreen and Cls are defined in derived classes
		virtual void ShowScreen(void){}
		virtual void Cls(unsigned char){}

		// Pallet manipulating functions
		void GetPalletReg(unsigned char colorID,char &red,char &green,char &blue);
		void SetPalletReg(unsigned char colorID,char red,char green,char blue);

	protected:
		unsigned short int maxx; // max screen values
		unsigned short int maxy;
		char far *vga; // Video Memory pointer
		char far *Buffer[4]; // MCGA,UN_MCGA,MODEX,VESA Buffer
									// MCGA only uses Buffer[0]

	}; // end VgaMode class


// ************** Temporary! A globabl vga class pointer *************** //
VgaMode *pstdvga; // standard vga mode


class ModeMCGA: public VgaMode // ModeMCGA is a derived class from VgaModes
	{
	public:

		ModeMCGA(void); // ModeMCGA constructor, sets the mode and prepares stuff
		~ModeMCGA(void); // ModeMCGA destructor, sets mode to text and cleans up

		void ShowScreen(void);
		void Cls(unsigned char color);

		void PutPixel(unsigned short int x,unsigned short int y,unsigned char color);
		unsigned char GetPixel(unsigned short int x,unsigned short int y);
		void PutHLine(unsigned short int x1,unsigned short int x2,unsigned short int y,unsigned char color);

	};



class ModeUN_MCGA: public VgaMode
	{
	public:

		ModeUN_MCGA(void); // ModeUN_MCGA constructor, sets the mode and prepares stuff
		~ModeUN_MCGA(void); // ModeUN_MCGA destructor, sets mode to text and cleans up

		void ShowScreen(void);
		void Cls(unsigned char color);

	};



class ModeMODEX: public VgaMode
	{
	public:

		ModeMODEX(void); // ModeMODEX constructor, sets the mode and prepares stuff
		~ModeMODEX(void); // ModeMODEX destructor, sets mode to text and cleans up

		void ShowScreen(void);
		void Cls(unsigned char color);

	};



class ModeVESA: public VgaMode
	{
	public:

		ModeVESA(void); // VESA constructor, sets the mode and prepares stuff
		~ModeVESA(void); // VESA destructor, sets mode to text and cleans up

		void ShowScreen(void);
		void Cls(unsigned char color);

	};

	#ifndef __VGA_CPP
	#include "vga.cpp"
	#endif

#endif