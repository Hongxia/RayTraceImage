//#####################################################################
// Stanford CS148 Ray Tracer Example Code
// Copyright 2012, Ben Mildenhall
//#####################################################################

#ifndef RayTracer_Shape_h
#define RayTracer_Shape_h

#include "Ray.h"
#include "Intersection.h"
#include "AABB.h"

class Shape {
public:
    virtual Intersection* getIntersect(const Ray& ray) = 0;
    virtual bool doesIntersect(const Ray& ray) = 0;
    virtual Intersection** getIntersections(const Ray& ray) = 0;
    virtual bool isInsideClosed(const STPoint3& pt) = 0;
    virtual bool isInsideOpen(const STPoint3& pt) = 0;
    virtual AABB* getAABB() = 0;
    std::string name;
    int maxInt;
};

#endif
