//#####################################################################
// Stanford CS148 Ray Tracer Example Code
// Copyright 2012, Ben Mildenhall
//#####################################################################

#ifndef RayTracer_PointLight_h
#define RayTracer_PointLight_h

#include "Light.h"
#include <iostream>

class PointLight : public Light {
public:
    PointLight(const STPoint3& _point, const STColor3f& _col)
    : point(_point), col(_col) { name = "point"; }
    
    STColor3f color(Intersection *inter, STVector3 view, const STColor3f& amb, const STColor3f& diff, const STColor3f& spec, float shine) {
		STVector3 L = point - inter->point;
		L.Normalize();
		STVector3 N = inter->normal;
		STVector3 R = STVector3::Dot(L, N) * 2 * N - L;
		view.Normalize();
		return diff * col * clamp(STVector3::Dot(L, N)) + spec * col * pow(clamp(STVector3::Dot(R, view)), shine);
    }
    
    STVector3 direction(const STPoint3& pt) { return point - pt; }

    STColor3f col;
    STPoint3 point;
};


#endif
