//#####################################################################
// Stanford CS148 Ray Tracer Example Code
// Copyright 2012, Ben Mildenhall
//#####################################################################

#ifndef RayTracer_Triangle_h
#define RayTracer_Triangle_h

#include "Shape.h"

class Triangle : public Shape {
public:
    Triangle(const STPoint3& _v1, const STPoint3& _v2, const STPoint3& _v3)
		: v1(_v1), v2(_v2), v3(_v3)
	{
        STVector3 n = STVector3::Cross(v2 - v1, v3 - v1); //going counterclockwise
        n.Normalize();
		n1 = n2 = n3 = n;
        this->name = "triangle";
        maxInt = 1;
    }
	Triangle(const STPoint3& _v1, const STPoint3& _v2, const STPoint3& _v3, 
		const STPoint2& _vt1,const STPoint2& _vt2,const STPoint2& _vt3)
		: v1(_v1), v2(_v2), v3(_v3), uv1(_vt1), uv2(_vt2), uv3(_vt3)
	{
		STVector3 n = STVector3::Cross(v2 - v1, v3 - v1); //going counterclockwise
		n.Normalize();
		n1 = n2 = n3 = n;
		this->name = "triangle";
		maxInt = 1;
	}
	Triangle(const STPoint3& _v1, const STPoint3& _v2, const STPoint3& _v3, 
		const STVector3& _n1, const STVector3& _n2, const STVector3& _n3,
		const STPoint2& _vt1,const STPoint2& _vt2,const STPoint2& _vt3)
		: v1(_v1), v2(_v2), v3(_v3), n1(_n1), n2(_n2), n3(_n3), uv1(_vt1), uv2(_vt2), uv3(_vt3)
	{
		this->name = "triangle";
		maxInt = 1;
	}
    Triangle(const Triangle& copy)
        :v1(copy.v1), v2(copy.v2), v3(copy.v3), 
		n1(copy.n1), n2(copy.n2), n3(copy.n3), 
		uv1(copy.uv1), uv2(copy.uv2), uv3(copy.uv3)
    {
        this->name = copy.name;
        this->maxInt = copy.maxInt;
    }

    Intersection* getIntersect(const Ray& ray);
    bool doesIntersect(const Ray& ray);
    Intersection** getIntersections(const Ray& ray);
    bool isInsideOpen(const STPoint3& pt) { return false; }
    bool isInsideClosed(const STPoint3& pt) { return false; }
    AABB* getAABB();
private:
    STPoint3 v1, v2, v3;
    STVector3 n1, n2, n3;
	STPoint2 uv1, uv2, uv3;
};

#endif
