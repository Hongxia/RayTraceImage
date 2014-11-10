//#####################################################################
// Stanford CS148 Ray Tracer Example Code
// Copyright 2012, Ben Mildenhall
//#####################################################################

#ifndef RayTracer_Camera_h
#define RayTracer_Camera_h

#include "STPoint3.h"
#include "STVector3.h"
#include "Ray.h"

class Camera {
public:
    Camera(const STPoint3& eye, const STVector3& up, const STPoint3& lookAt, float fovy, float aspect);
    ~Camera(){}
    Ray* generateRay(float u, float v, float bias = 0.) const;
    float getFocalRatio(const STPoint3& f);
    STPoint3 pointOnPlane(float u, float v) const;
private:
    STPoint3 eye;
    STVector3 up;
    STPoint3 lookAt;
    float fovy;
    float aspect;
    
    STVector3 left, UL, UR, LL, LR;
};


#endif
