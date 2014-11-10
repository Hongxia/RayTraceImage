//#####################################################################
// Stanford CS148 Ray Tracer Example Code
// Copyright 2012, Ben Mildenhall
//#####################################################################

#ifndef RayTracer_Light_h
#define RayTracer_Light_h

#include "STColor3f.h"
#include "Intersection.h"
#include "STVector3.h"
#include "STPoint3.h"

class Light {
public:
    virtual ~Light() {}
    virtual STColor3f color(Intersection *inter, STVector3 view, const STColor3f& amb, const STColor3f& diff, const STColor3f& spec, float shine) = 0;
    virtual STVector3 direction(const STPoint3& pt) = 0;
    std::string name;
protected:
    float clamp(float x) {
        if (x > 1.) return 1.;
        if (x < 0.) return 0.;
        return x;
    }
};

#endif
