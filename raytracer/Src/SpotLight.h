//#####################################################################
// Stanford CS148 Ray Tracer Example Code
// Copyright 2012, Hongxia Zhong
//#####################################################################

#ifndef RayTracer_SpotLight_h
#define RayTracer_SpotLight_h

#include "Light.h"
#include <iostream>
#include <math.h>

#define PI 3.14159265

class SpotLight : public Light {
public:
    SpotLight(const STPoint3& _point, const STVector3& _dir, const double _cutoff, const STColor3f& _col)
    : point(_point), dir(_dir), cutoff(_cutoff), col(_col) { name = "spot"; }
    
    STColor3f color(Intersection *inter, STVector3 view, const STColor3f& amb, const STColor3f& diff, const STColor3f& spec, float shine) {
        STVector3 L = point - inter->point;
        L.Normalize();
        dir.Normalize();
        // cutoff = max(0.0, cutoff);
        // cutoff = min(cutoff, 180.0);

        // Within cutoff angle
        if (STVector3::Dot(L, -dir) >= cos (cutoff*PI/180.0)) {
            STVector3 N = inter->normal;
            STVector3 R = STVector3::Dot(L, N) * 2 * N - L;
            view.Normalize();
            return diff * col * clamp(STVector3::Dot(L, N)) + spec * col * pow(clamp(STVector3::Dot(R, view)), shine);
        } else {
            return STColor3f(0.f,0.f,0.f);
        }
    }
    
    STVector3 direction(const STPoint3& pt) { return point - pt; }

    STColor3f col;
    STPoint3 point;
    STVector3 dir;
    double cutoff;
};


#endif
