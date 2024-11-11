#ifndef CAMERA_H
#define CAMERA_H

#include "global.h"

typedef struct {
    VECTOR position;
    VECTOR rotation;
    MATRIX lookat;
} Camera;

void LookAt(Camera* camera, VECTOR* target, VECTOR* up);

#endif