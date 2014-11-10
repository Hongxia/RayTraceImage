//#####################################################################
// Stanford CS148 Ray Tracer Example Code
// Copyright 2012, Ben Mildenhall
//#####################################################################

#ifndef RayTracer_DirectionalLight_h
#define RayTracer_DirectionalLight_h

#include "Light.h"

class DirectionalLight : public Light {
public:
    DirectionalLight(const STVector3& _dir, const STColor3f& _col)
    : dir(_dir), col(_col) { name = "directional"; }
    ~DirectionalLight(){}

    STColor3f color(Intersection *inter, STVector3 view, const STColor3f& amb, const STColor3f& diff, const STColor3f& spec, float shine) {
        STVector3 L = -1 * dir;
        L.Normalize();
        STVector3 N = inter->normal;
        STVector3 R = STVector3::Dot(L, N) * 2 * N - L;
        view.Normalize();
        return diff * col * clamp(STVector3::Dot(L, N)) + spec * col * pow(clamp(STVector3::Dot(R, view)), shine);
    }
    STVector3 direction(const STPoint3& pt) { return -1. * dir; }
private:
    STColor3f col;
    STVector3 dir;
};


#endif
