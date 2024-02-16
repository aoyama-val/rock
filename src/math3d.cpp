#include <math.h>
#include "math3d.h"

float distance3fv(float *a, float *b)
{
    float d[3];
    d[0] = a[0]-b[0];
    d[1] = a[1]-b[1];
    d[2] = a[2]-b[2];
    return sqrt(d[0]*d[0]+d[1]*d[1]+d[2]*d[2]);
}

void normalize(float *v)
{
    float n;
    n = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
    if (NEAR_ZERO(n)) return;
    v[0] /= n;
    v[1] /= n;
    v[2] /= n;
}

void calc2DFrontVector(float *rot, float *out)
{
    out[0] = cos(DEG2RAD(rot[2]));
    out[1] = sin(DEG2RAD(rot[2]));
}

void calc2DRightVector(float *rot, float *out)
{
    out[0] = cos(DEG2RAD(rot[2])-90);
    out[1] = sin(DEG2RAD(rot[2])-90);
}

void calcEyeLineVector(float *rot, float *out)
{
    out[0] = cos(DEG2RAD(rot[0])) * cos(DEG2RAD(rot[2]));
    out[1] = cos(DEG2RAD(rot[0])) * sin(DEG2RAD(rot[2]));
    out[2] = sin(DEG2RAD(rot[0]));
}

float cosdeg(float d)
{
    return cos(DEG2RAD(d));
}

float sindeg(float d)
{
    return sin(DEG2RAD(d));
}
