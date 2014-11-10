//#####################################################################
// Stanford CS148 Ray Tracer Example Code
// Copyright 2012, Ben Mildenhall
//#####################################################################

#ifndef RayTracer_Intersection_h
#define RayTracer_Intersection_h

#include "STPoint3.h"
#include "STPoint2.h"
#include "STVector3.h"

struct Intersection {
    float t;
    STPoint3 point;
    STVector3 normal;
	STPoint2 uv;
    Intersection(const float _t, const STPoint3& _point, const STVector3& _normal,const STPoint2 _uv=STPoint2()): t(_t), point(_point), normal(_normal), uv(_uv) {}
    Intersection(const Intersection& copy){t=copy.t;point=copy.point;normal=copy.normal;uv=copy.uv;}
    Intersection() {}
    ~Intersection(){}
};

#endif
