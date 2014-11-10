//#####################################################################
// Stanford CS148 Ray Tracer Example Code
// Copyright 2012, Ben Mildenhall
//#####################################################################

#ifndef RayTracer_Ray_h
#define RayTracer_Ray_h

#include "float.h"
#include "STVector3.h"
#include "STPoint3.h"
#include "STTransform4.h"

class Ray {
public:
    Ray(const STPoint3& _e, const STVector3& _d, float _t_min = 0.f, float _t_max = FLT_MAX)
		: e(_e), d(_d), t_min(_t_min), t_max(_t_max) {}
    
    bool inRange(float t) const { return t >= t_min && t <= t_max; }
    STPoint3 at(float t) const { return e + d * t ; }
    Ray transform(const STTransform4& M) const { return Ray(M * e, M * d, t_min, t_max); }
    
    STPoint3 e;
    STVector3 d;
    float t_min;
    float t_max;
};

#endif
