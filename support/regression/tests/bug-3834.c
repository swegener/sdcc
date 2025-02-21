/* bug-3406.c
   A bug in z80 code generation for pointer writes. Hard to trigger, since it requires specific registers to be in use for particular operands, and multiple others to be in use.
 */
 
#include <testfwk.h>

typedef unsigned char u8;
typedef unsigned short u16;

struct VDP_Command {
    u16 DX, DY, NX;
    u8 CLR;
};

struct VDP_Command g_VDP_Command;

u16  words[8];
u16* pwords = words;
void my_VPD_CommandSetupR36( void ) {
    *pwords++ = g_VDP_Command.NX;
}

// HERE: VDP_CommandLMMV(x, y, pixelWidth_FAILS, src->Red );
inline void VDP_CommandLMMV(u16 dx, u16 dy, u16 nx, u8 col)
{
    g_VDP_Command.DX = dx;
    g_VDP_Command.DY = dy; // For this line, the order of register saves to stack is incompatible with restoring them.
    g_VDP_Command.NX = nx;
    g_VDP_Command.CLR = col;
    my_VPD_CommandSetupR36();
}

struct ScreenSetting {
    u16 Width;
    u8 Red;
};

const struct ScreenSetting g_Settings[] = { {
     256, // <===== WIDTH
     17
  },
};

void InitScreen( void ) {
    const struct ScreenSetting* src = &g_Settings[0];
    u8 pixelWidth = src->Width / 256;
    VDP_CommandLMMV(0, 0, src->Width, 0);
    for(u8 i = 0; i < 2; ++i) {
        u16 x = 19 % src->Width;
        u16 y = x & 1;
        VDP_CommandLMMV(x, y, pixelWidth, src->Red);
        VDP_CommandLMMV(x, y, pixelWidth, src->Red);
    }
}

void testBug( void ) {
    InitScreen();

    ASSERT(words[0] == 0x0100);
    ASSERT(words[1] == 1); // 0x1110 when bug is present
    ASSERT(words[2] == 1);
    ASSERT(words[3] == 1); // 0x1110 when bug is present
    ASSERT(words[4] == 1);
}

extern void VDP_CommandLMMV(u16 dx, u16 dy, u16 nx, u8 col);

