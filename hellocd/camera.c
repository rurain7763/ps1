#include "camera.h"

#define VectorCross(v1, v2, result) OuterProduct12(v1, v2, result)

void LookAt(Camera* camera, VECTOR* target, VECTOR* up) {
    VECTOR xright;
    VECTOR yup;
    VECTOR zforward;
    VECTOR x, y, z;
    VECTOR pos;
    VECTOR t;

    zforward.vx = target->vx - camera->position.vx;
    zforward.vy = target->vy - camera->position.vy;
    zforward.vz = target->vz - camera->position.vz;
    VectorNormal(&zforward, &z);

    VectorCross(&z, up, &xright);
    VectorNormal(&xright, &x);

    VectorCross(&z, &x, &yup);
    VectorNormal(&yup, &y);

    camera->lookat.m[0][0] = x.vx; camera->lookat.m[0][1] = x.vy; camera->lookat.m[0][2] = x.vz;
    camera->lookat.m[1][0] = y.vx; camera->lookat.m[1][1] = y.vy; camera->lookat.m[1][2] = y.vz;
    camera->lookat.m[2][0] = z.vx; camera->lookat.m[2][1] = z.vy; camera->lookat.m[2][2] = z.vz;

    pos.vx = -camera->position.vx;
    pos.vy = -camera->position.vy;
    pos.vz = -camera->position.vz;

    ApplyMatrixLV(&camera->lookat, &pos, &t);
    TransMatrix(&camera->lookat, &t);
}