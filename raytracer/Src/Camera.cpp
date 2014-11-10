//#####################################################################
// Stanford CS148 Ray Tracer Example Code
// Copyright 2012, Ben Mildenhall
//#####################################################################

#include "Camera.h"

static const double pi = 3.14159265358979;

Camera::Camera(const STPoint3& _eye, const STVector3& _up, const STPoint3& _lookAt, float _fovy, float _aspect)
: eye(_eye), up(_up), lookAt(_lookAt), fovy(_fovy), aspect(_aspect)
{
    left = STVector3::Cross(up, lookAt - eye);
    up = STVector3::Cross(lookAt - eye, left);
    left.Normalize();
    up.Normalize();
    
    float y = (lookAt - eye).Length() * tan(.5f * (fovy * (float)pi / 180.f));
    float x = y * aspect;
    UL = STVector3(lookAt) + x * left + y * up;
    UR = STVector3(lookAt) - x * left + y * up;
    LL = STVector3(lookAt) + x * left - y * up;
    LR = STVector3(lookAt) - x * left - y * up;
    
}

STPoint3 Camera::pointOnPlane(float u, float v) const {
    STVector3 pVec = (1 - u) * ((1 - v) * LL + v * UL)
                        + u * ((1 - v) * LR + v * UR);
    return STPoint3(pVec);
}

Ray* Camera::generateRay(float u, float v, float bias) const {
    return new Ray(eye, pointOnPlane(u, v) - eye, bias);
}

float Camera::getFocalRatio(const STPoint3 &f) {
    return STVector3::Dot(f - eye, lookAt - eye) / (lookAt - eye).LengthSq();
}