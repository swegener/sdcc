/* bug-3727.c
   ?
 */

#include <testfwk.h>

#pragma disable_warning 85

typedef unsigned char u8;
typedef signed char i8;
typedef unsigned int u16;
typedef signed int i16;

void VDP_Poke_16K(u8 val, u16 dest);

struct VDP_Sprite
{
    u8 Y;
    u8 X;
    u8 Pattern;
    u8 Color;
};

extern struct VDP_Sprite g_VDP_Sprite;

#define MaxNumSprtPerObj 2

typedef struct {
	u8 Status;
	i16	x,y;
	u8 NumSprt;
	u8 dx[MaxNumSprtPerObj],dy[MaxNumSprtPerObj];
	u8 pat[MaxNumSprtPerObj],col[MaxNumSprtPerObj];
} Object;

#define SatAddr0	0x1200

#define SatAddr1	0x1E00

#define MaxNumObj 2
Object MyObject[MaxNumObj];

#define YScreenSplit 106

extern u8 SprtColDef[];
extern u8 SprtPatDef[];

u8 n0,n1;

Object* MyObj;
u8 ii,jj;

i16 y;
i16 x;

void fake (void) {
	n1 = 32;
}

extern u8 Sprt_flip;

void SatUpdate(void)
{
	if (Sprt_flip) 
	{
		n1=0;
		
		MyObj = &MyObject[0];
		for (ii=0;ii<MaxNumObj;ii++)
		{
			if (MyObj->Status != 0) 
			{
				for (jj=0;jj<MyObj->NumSprt;jj++) 
				{
					y = MyObj->y + MyObj->dy[jj];
					x = MyObj->x + MyObj->dx[jj];

					// SAT1
					fake();
				}
			}
			MyObj++;					
		}
		if (n1<32) { // Here, SDCC assumed n1 == 0.
			VDP_Poke_16K(216, SatAddr1+4*n1);
		}
	}
	else 
	{
		n0=0;
		
		MyObj = &MyObject[MaxNumObj-1];
		for (ii=0;ii<MaxNumObj;ii++)
		{
			if (MyObj->Status != 0) 
			{
				for (jj=0;jj<MyObj->NumSprt;jj++) 
				{
					y = MyObj->y + MyObj->dy[jj];
					x = MyObj->x + MyObj->dx[jj];

					// SAT0
					if (n0<32)
						n0++;	
				}
			}
			MyObj--;					
		}
		if (n0<32) {
			VDP_Poke_16K(216, SatAddr0+4*n0);
		}
	}
}

u8 Sprt_flip;

void
testBug(void)
{
	Sprt_flip = 1;
	MyObject[0].Status = 1;
	MyObject[0].NumSprt = 1;
#if 0 // Bug not yet fixed
	SatUpdate();
#endif
}

void VDP_Poke_16K(u8 val, u16 dest)
{
	ASSERT(dest != SatAddr1);
}

struct VDP_Sprite g_VDP_Sprite;

u8 SprtColDef[1];
u8 SprtPatDef[1];


