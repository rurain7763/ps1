#include <stdlib.h>
#include <libgte.h>
#include <libetc.h>
#include <libgpu.h>

#include "joypad.h"
#include "global.h"

#define VIDEO_MODE 0
#define SCREEN_RES_X 320
#define SCREEN_RES_Y 240
#define SCREEN_CENTER_X (SCREEN_RES_X >> 1)
#define SCREEN_CENTER_Y (SCREEN_RES_Y >> 1)
#define SCREEN_Z 320

#define NUM_CUBE_VERTICES 8
#define NUM_CUBE_FACES 12

#define NUM_FLOOR_VERTICES 4
#define NUM_FLOOR_FACES 2

typedef struct {
    DRAWENV draw[2];
    DISPENV disp[2];
} DoubleBuffer;

DoubleBuffer dbuff;
u_short currBuff;

SVECTOR vertices[] = {
    { -128, -128, -128 },
    {  128, -128, -128 },
    { 128, -128, 128 },
    { -128, -128, 128 },
    { -128, 128, -128 },
    {  128, 128, -128 },
    { 128, 128, 128 },
    { -128, 128, 128 }
};

u_short faces[] = {
    0, 3, 2,
    0, 2, 1,
    4, 0, 1,
    4, 1, 5,
    7, 4, 5,
    7, 5, 6,
    5, 1, 2,
    5, 2, 6,
    2, 3, 7,
    2, 7, 6,
    0, 4, 7,
    0, 7, 3 
};

SVECTOR floor[] = {
    { -600, 0, -600 },
    { -600, 0, 600 },
    { 600, 0, 600 },
    { 600, 0, -600 }
};

u_short floorFaces[] = {
    0, 1, 2,
    0, 2, 3
};

SVECTOR cubeRotation = { 0, 0, 0 };
VECTOR cubeTranslation = { 0, 0, 900 };
VECTOR cubeScale = { ONE, ONE, ONE };

MATRIX worldMatrix = { 0 };

void ScreenInit(void) {
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

void Setup(void) {
    ScreenInit();

    JoyPadInit();

    cubeTranslation.vx = 0;
    cubeTranslation.vy = 0;
    cubeTranslation.vz = 2500;

    ResetNextPrim(currBuff);
}

void Update(void) {
    POLY_G3* poly;
    int i;
    int nclip;
    long otz, p, flag;

    EmptyOT(currBuff);

    JoyPadUpdate();

    if(JoyPadCheck(PAD1_LEFT)) {
        cubeRotation.vy += 10;
    }

    if(JoyPadCheck(PAD1_RIGHT)) {
        cubeRotation.vy -= 10;
    }

    // draw cube
    RotMatrix(&cubeRotation, &worldMatrix);
    TransMatrix(&worldMatrix, &cubeTranslation);
    ScaleMatrix(&worldMatrix, &cubeScale);

    SetRotMatrix(&worldMatrix);
    SetTransMatrix(&worldMatrix);

    for(i = 0; i < NUM_CUBE_FACES * 3; i += 3) {
        poly = (POLY_G3*)GetNextPrim();
        setPolyG3(poly);
        setRGB0(poly, 0, 255, 255);
        setRGB1(poly, 255, 0, 255);
        setRGB2(poly, 255, 255, 0);

        nclip = RotAverageNclip3(
            &vertices[faces[i + 0]], 
            &vertices[faces[i + 1]], 
            &vertices[faces[i + 2]], 
            (long*)&poly->x0, 
            (long*)&poly->x1, 
            (long*)&poly->x2, 
            &p,
            &otz, 
            &flag);

        if(nclip <= 0) {
            continue;
        }

        if(otz > 0 && otz < OT_LENGTH) {
            addPrim(GetOTAt(currBuff, otz), poly);
            IncreaseNextPrim(sizeof(POLY_G3));
        }
    }
}

void Render(void) {
    DisplayFrame();
}

int main(void) {
    Setup();
    while(1) {
        Update();
        Render();
    }
    return 0;
}