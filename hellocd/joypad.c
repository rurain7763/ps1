#include "joypad.h"

static u_long padState;

void JoyPadInit(void) {
    PadInit(0);
}

void JoyPadUpdate(void) {
    padState = PadRead(0);
}

void JoyPadReset(void) {
    padState = 0;
}

int JoyPadCheck(int p) {
    return padState & p;
}