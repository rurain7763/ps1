#ifndef GLOBAL_H
#define GLOBAL_H

#define OT_LENGTH 8192
#define PRIMBUFF_LENGTH 131072

#include <sys/types.h>

void EmptyOT(u_short curBuff);
void SetOTAt(u_short curBuff, u_int i, u_long prim);
u_long* GetOTAt(u_short curBuff, u_int i);

void IncreaseNextPrim(u_int size);
void SetNextPrim(char* value);
void ResetNextPrim(u_short curBuff);
char* GetNextPrim();

#endif