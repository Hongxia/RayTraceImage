//#####################################################################
// Stanford CS148 Ray Tracer Example Code
// Copyright 2012, Ben Mildenhall
//#####################################################################

#ifndef RayTracer_Cylinder_h
#define RayTracer_Cylinder_h

#include "Shape.h"
#include "Triangle.h"

class Cylinder : public Shape {
public:
    Cylinder(const STPoint3& _A, const STPoint3& _B, float _radius)
    : A(_A), B(_B), radius(_radius) {
        this->name = "cylinder";
        maxInt = 2;
        STVector3 u = STVector3::Cross(B - A, STVector3(1.1424f, 5.25f, 0));
        if (!u.Length()) u = STVector3::Cross(B - A, STVector3(0.f, 3.251f, 0));
        STVector3 v = STVector3::Cross(B - A, u);
        u.Normalize();
        v.Normalize();
        STPoint3 bOrig = A - 1.5f * radius * v - 1.5f * radius * u;
        STPoint3 tOrig = B - 1.5f * radius * v - 1.5f * radius * u;
        top = new Triangle(bOrig, bOrig + 4.5f * radius * v, bOrig + 4.5f * radius * u);
        bottom = new Triangle(tOrig, tOrig + 4.5f * radius * u, tOrig + 4.5f * radius * v);
    }
    Cylinder(const Cylinder& copy)
        :A(copy.A),B(copy.B),radius(copy.radius)
    {
        this->name=copy.name;
        this->maxInt=copy.maxInt;
        if(copy.top!=NULL)this->top=new Triangle(*copy.top);
        if(copy.bottom!=NULL)this->bottom=new Triangle(*copy.bottom);
    }
    ~Cylinder()
    {
        delete top;delete bottom;
    }

    Intersection* getIntersect(const Ray& ray);
    bool doesIntersect(const Ray& ray);
    Intersection** getIntersections(const Ray& ray);
    bool isInsideOpen(const STPoint3& pt);
    bool isInsideClosed(const STPoint3& pt);
    AABB* getAABB();

private:
    STPoint3 A, B;
    float radius;
    Triangle *top, *bottom;
    bool isWithinCylinder(const STPoint3& pt);
    Intersection* closer(Intersection* a, Intersection* b);
};

#endif
