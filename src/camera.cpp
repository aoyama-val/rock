#include "camera.h"
#include "common.h"
#include "math3d.h"
#include <SDL_opengl.h>

Camera::Camera()
{
    distance = 6;
    height = 2.5;
}

void Camera::move(float* playerPos, float* playerRot)
{
    float front[3];

    calcEyeLineVector(playerRot, front);
    pos[0] = playerPos[0] - front[0] * distance;
    pos[1] = playerPos[1] - front[1] * distance;
    pos[2] = playerPos[2] - front[2] * distance + height;

    rot[0] = playerRot[0];
    rot[1] = playerRot[1];
    rot[2] = playerRot[2]; 
}

void Camera::glRotate()
{
    glRotatef(-rot[0], 1, 0, 0);
    glRotatef(-rot[1], 0, 1, 0);
    glRotatef(-(rot[2]-90), 0, 0, 1);
}

void Camera::glTranslate()
{
    glTranslatef(-pos[0], -pos[1], -pos[2]);
}

