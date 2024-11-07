#include <stdlib.h>
#include <libgte.h>
#include <libetc.h>
#include <libgpu.h>

#include "joypad.h"
#include "global.h"
#include "display.h"

#define NUM_CUBE_VERTICES 8
#define NUM_CUBE_FACES 12

#define NUM_FLOOR_VERTICES 4
#define NUM_FLOOR_FACES 2

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

void Setup(void) {
    DisplayInit();

    JoyPadInit();

    cubeTranslation.vx = 0;
    cubeTranslation.vy = 0;
    cubeTranslation.vz = 2500;

    ResetNextPrim(GetCurrBuff());
}

void Update(void) {
    POLY_G3* poly;
    int i;
    int nclip;
    long otz, p, flag;
    int currBuff = GetCurrBuff();

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