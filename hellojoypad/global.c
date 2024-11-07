#include "global.h"

#include <libgte.h>
#include <libgpu.h>

static u_long ot[2][OT_LENGTH];

static char primBuff[2][PRIMBUFF_LENGTH];
static char* nextPrim;

void EmptyOT(u_short curBuff) {
    ClearOTagR(ot[curBuff], OT_LENGTH);
}

void SetOTAt(u_short curBuff, u_int i, u_long prim) {
    ot[curBuff][i] = prim;
}

u_long* GetOTAt(u_short curBuff, u_int i) {
    return &ot[curBuff][i];
}

void IncreaseNextPrim(u_int size) {
    nextPrim += size;
}

void SetNextPrim(char* value) {
    nextPrim = value;
}

void ResetNextPrim(u_short curBuff) {
    nextPrim = primBuff[curBuff];
}

char* GetNextPrim() {
    return nextPrim;
}
