#ifndef CAMERA_H
#define CAMERA_H

class Camera {
public:
    Camera();
    void move(float* playerPos, float* playerRot);
    void glRotate();
    void glTranslate();
    void setDistance(float d) { distance = d; }
    void getPos(float* dst) { dst[0] = pos[0]; dst[1] = pos[1]; dst[2] = pos[2]; }
    void getRot(float* dst) { dst[0] = rot[0]; dst[1] = rot[1]; dst[2] = rot[2]; }
    void setPos(float x, float y, float z) { pos[0] = x; pos[1] = y; pos[2] = z; }
    void setRot(float x, float y, float z) { rot[0] = x; rot[1] = y; rot[2] = z; }
protected:
    float distance;
    float height;
    float pos[3];
    float rot[3];
};

#endif
