//#####################################################################
// Stanford CS148 Ray Tracer Example Code
// Copyright 2012, Ben Mildenhall
//#####################################################################

#ifndef RayTracer_AmbientLight_h
#define RayTracer_AmbientLight_h

#include "Light.h"

class AmbientLight : public Light {
public:
    AmbientLight(const STColor3f& _col): col(_col) { name = "ambient"; }
    ~AmbientLight(){}
    STColor3f color(Intersection *inter, STVector3 view, const STColor3f& amb, const STColor3f& diff, const STColor3f& spec, float shine) { return col * amb; }
    STVector3 direction(const STPoint3& pt) { return STVector3(0., 0., 0.); }
private:
    STColor3f col;
};

#endif
