//#####################################################################
// Stanford CS148 Ray Tracer Example Code
// Copyright 2012, Ben Mildenhall
//#####################################################################

#ifndef RayTracer_Sphere_h
#define RayTracer_Sphere_h

#include "Shape.h"

class Sphere : public Shape {
public:
    // Sphere(const STPoint3& _center, float _radius)
    //     : center(_center), radius(_radius) 
    // {
    //     this->name = "sphere";
    //     maxInt = 2;
    // }
    Sphere(const Sphere& copy)
        : center(copy.center), radius(copy.radius)
    {
        this->name = copy.name;
        this->maxInt = copy.maxInt;
    }
    Sphere(const STPoint3& _center, float _radius, bool _invert_normal = false)
        : center(_center), radius(_radius), invert_normal(_invert_normal)
    {
        this->name = "sphere";
        maxInt = 2;
    }

    Intersection* getIntersect(const Ray& ray);
    bool doesIntersect(const Ray& ray);
    Intersection** getIntersections(const Ray& ray);
    
    bool isInsideOpen(const STPoint3& pt);
    bool isInsideClosed(const STPoint3& pt);

    AABB* getAABB();
private:
    STPoint3 center;
    float radius;
    bool invert_normal;
};

#endif
