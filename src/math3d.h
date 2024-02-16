#ifndef MATH3D_H
#define MATH3D_H

#include <boost/operators.hpp>

#define DEG2RAD(x) ((x)/360*2*M_PI)
#define RAD2DEG(x) ((x)/(2*M_PI)*360)

#define EPSILON (1e-10)
#define NEAR_ZERO(x) ( (-(EPSILON) < (x) && (x) < (EPSILON)) ? 1 : 0 )

template<typename T>
struct Vector3
    : boost::additive< Vector3<T> >
    , boost::multiplicative< Vector3<T>, T>
{ 
    operator T*() { return v; }

    Vector3() {}

    Vector3(T v0, T v1, T v2) {
        v[0] = v0;
        v[1] = v1;
        v[2] = v2;
    }

    Vector3 operator+=(const Vector3& rhs) {
        v[0] += rhs.v[0];
        v[1] += rhs.v[1];
        v[2] += rhs.v[2];
        return *this;
    }

    Vector3 operator*=(T rhs) {
        v[0] *= rhs;
        v[1] *= rhs;
        v[2] *= rhs;
        return *this;
    }

    Vector3 operator/=(T rhs) {
        v[0] /= rhs;
        v[1] /= rhs;
        v[2] /= rhs;
        return *this;
    } 

    T& operator[](int n) {
        return v[n];
    }
    
    const T& operator[](int n) const {
        return v[n];
    }

    T dot(const Vector3& rhs) const {
        return v[0]*rhs.v[0] + v[1]*rhs.v[1] + v[2]*rhs.v[2];
    }

    Vector3 cross(const Vector3& rhs) {
        Vector3<T> a;
        a.v[0] = v[1]*rhs.v[2] - v[2]*rhs.v[1];
        a.v[1] = v[2]*rhs.v[0] - v[0]*rhs.v[2];
        a.v[2] = v[0]*rhs.v[1] - v[1]*rhs.v[0];
        return a;
    }

    T norm() const {
        return sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
    }

    void normalize() {
        float n = norm();
        v[0] /= n;
        v[1] /= n;
        v[2] /= n;
    }

    T v[3];
};

typedef Vector3<int> Vector3i;
typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;

float distance3fv(float *a, float *b);
void normalize(float *v);
void calc2DFrontVector(float *rot, float *out);
void calc2DRightVector(float *rot, float *out);
void calcEyeLineVector(float *rot, float *out);
float cosdeg(float d);
float sindeg(float d);

#endif
