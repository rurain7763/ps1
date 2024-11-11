#ifndef DIPSLAY_H
#define DISPLAY_H

#include <sys/types.h>
#include <libgte.h>
#include <libetc.h>
#include <libgpu.h>

#define VIDEO_MODE 0
#define SCREEN_RES_X 320
#define SCREEN_RES_Y 240
#define SCREEN_CENTER_X (SCREEN_RES_X >> 1)
#define SCREEN_CENTER_Y (SCREEN_RES_Y >> 1)
#define SCREEN_Z 320

typedef struct {
    DRAWENV draw[2];
    DISPENV disp[2];
} DoubleBuffer;

void DisplayInit(void);
void DisplayFrame(void);

u_short GetCurrBuff(void);

#endif