//#####################################################################
// Stanford CS148 Ray Tracer Example Code
// Copyright 2012, Hongxia Zhong
//#####################################################################

#ifndef RayTracer_SpotLight_h
#define RayTracer_SpotLight_h

#include "Light.h"
#include <iostream>
#include <math.h>
#include "STPoint3.h"

#define PI 3.14159265

class SpotLight : public Light {
public:
    SpotLight(const STPoint3& _point, const STVector3& _dir, const double _cutoff, const STColor3f& _col, const double _spot_exponent = 0.0, const double _const_atten = 1.0, const double _lin_atten = 0.0, const double _quad_atten = 0.0)
    : point(_point), dir(_dir), cutoff(_cutoff), col(_col), spot_exponent(_spot_exponent), const_atten(_const_atten), lin_atten(_lin_atten), quad_atten(_quad_atten) { name = "spot"; }
    
    STColor3f color(Intersection *inter, STVector3 view, const STColor3f& amb, const STColor3f& diff, const STColor3f& spec, float shine) {
        STVector3 L = point - inter->point;
        L.Normalize();
        dir.Normalize();
        cutoff = std::max(0.0, cutoff);
        cutoff = std::min(180.0, cutoff);

        // Within cutoff angle
        if (STVector3::Dot(L, -dir) >= cos (cutoff*PI/180.0)) {
            float d = STPoint3::Dist(inter->point, point);
            float atten_factor = 1.0/(const_atten + lin_atten*d + quad_atten*d*d);
            STVector3 N = inter->normal;
            STVector3 R = STVector3::Dot(L, N) * 2 * N - L;
            view.Normalize();
            float exponent = pow(STVector3::Dot(-L, dir), spot_exponent);
            return exponent*atten_factor*(diff * col * clamp(STVector3::Dot(L, N)) + spec * col * pow(clamp(STVector3::Dot(R, view)), shine));
        } else {
            return STColor3f(0.f,0.f,0.f);
        }
    }
    
    STVector3 direction(const STPoint3& pt) { return point - pt; }

    STColor3f col;
    STPoint3 point;
    STVector3 dir;
    double cutoff;
    double spot_exponent;
    double const_atten;
    double lin_atten;
    double quad_atten;
};

#endif
