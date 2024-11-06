#include <stdlib.h>
#include <libgte.h>
#include <libetc.h>
#include <libgpu.h>

#define VIDEO_MODE 0
#define SCREEN_RES_X 320
#define SCREEN_RES_Y 240
#define SCREEN_CENTER_X (SCREEN_RES_X >> 1)
#define SCREEN_CENTER_Y (SCREEN_RES_Y >> 1)
#define SCREEN_Z 400

#define OT_LENGTH 16

typedef struct {
    DRAWENV draw[2];
    DISPENV disp[2];
} DoubleBuffer;

DoubleBuffer dbuff;
u_short currBuff;

u_long ot[2][OT_LENGTH];

char primBuff[2][2048];
char* nextPrim;

POLY_F3* triangle;
TILE* tile;
POLY_G4* quad;

void ScreenInit(void) {
    // reset gpu
    ResetGraph(0); 

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

    //DrawOTag(ot[currBuff] + OT_LENGTH - 1);
    DrawOTag(ot[currBuff]);

    currBuff = !currBuff;

    nextPrim = primBuff[currBuff];
}

void Setup(void) {
    ScreenInit();

    nextPrim = primBuff[currBuff];
}

void Update(void) {
    ClearOTagR(ot[currBuff], OT_LENGTH);

    tile = (TILE*)nextPrim;
    setTile(tile);
    setXY0(tile, 82, 32);
    setWH(tile, 64, 64);
    setRGB0(tile, 0, 255, 0);
    addPrim(ot[currBuff], tile);
    nextPrim += sizeof(TILE);

    triangle = (POLY_F3*)nextPrim;
    setPolyF3(triangle);
    setXY3(triangle, 64, 100, 200, 150, 50, 220);
    setRGB0(triangle, 255, 0, 255);
    addPrim(ot[currBuff], triangle);
    nextPrim += sizeof(POLY_F3);

    quad = (POLY_G4*)nextPrim;
    setPolyG4(quad);
    setXY4(quad, 200, 100, 200, 200, 100, 100, 100, 200);
    setRGB0(quad, 0, 0, 255);
    setRGB1(quad, 255, 0, 0);
    setRGB2(quad, 0, 255, 0);
    setRGB3(quad, 255, 255, 255);
    addPrim(ot[currBuff], quad);
    nextPrim += sizeof(POLY_G4);
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