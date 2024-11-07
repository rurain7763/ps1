#include "display.h"
#include "global.h"
#include <sys/types.h>

static DoubleBuffer dbuff;
static u_short currBuff;

void DisplayInit(void) {
     // reset gpu
    ResetGraph(VIDEO_MODE); 

    // set the display area of the first buffer
    SetDefDispEnv(&dbuff.disp[0], 0,            0, SCREEN_RES_X, SCREEN_RES_Y);
    SetDefDrawEnv(&dbuff.draw[0], 0, SCREEN_RES_Y, SCREEN_RES_X, SCREEN_RES_Y);

    // set the display area of the second buffer
    SetDefDispEnv(&dbuff.disp[1], 0, SCREEN_RES_Y, SCREEN_RES_X, SCREEN_RES_Y);
    SetDefDrawEnv(&dbuff.draw[1], 0,            0, SCREEN_RES_X, SCREEN_RES_Y);

    // set the back/drawing buffer
    dbuff.draw[0].isbg = 1;
    dbuff.draw[1].isbg = 1;

    // set the background clear color (purple)
    setRGB0(&dbuff.draw[0], 63, 0, 127);
    setRGB0(&dbuff.draw[1], 63, 0, 127);

    // set the current initial buffer
    currBuff = 0;
    PutDispEnv(&dbuff.disp[currBuff]);
    PutDrawEnv(&dbuff.draw[currBuff]);

    // initialize and setup the GTE geometry offsets
    InitGeom();
    SetGeomOffset(SCREEN_CENTER_X, SCREEN_CENTER_Y);
    SetGeomScreen(SCREEN_Z);

    // enable display
    SetDispMask(1);
}

void DisplayFrame(void) {
    DrawSync(0);
    VSync(0);

    PutDispEnv(&dbuff.disp[currBuff]);
    PutDrawEnv(&dbuff.draw[currBuff]);

    DrawOTag(GetOTAt(currBuff, OT_LENGTH - 1));

    currBuff = !currBuff;

    ResetNextPrim(currBuff);
}

u_short GetCurrBuff(void) {
    return currBuff;
}

