//#####################################################################
// Stanford CS148 Ray Tracer Example Code
// Copyright 2012, Ben Mildenhall
//#####################################################################

#include "Triangle.h"

float determinant(const STVector3& a, const STVector3& b, const STVector3& c) {
    return a.x*(b.y*c.z-b.z*c.y)-b.x*(a.y*c.z-a.z*c.y)+c.x*(a.y*b.z-a.z*b.y);
}

Intersection* Triangle::getIntersect(const Ray &ray) {
    float detA = determinant(v1 - v2, v1 - v3, ray.d);
    if (detA == 0.) return NULL;
    float beta  = determinant(v1 - ray.e, v1 - v3, ray.d) / detA;
    float gamma = determinant(v1 - v2, v1 - ray.e, ray.d) / detA;
    float t     = determinant(v1 - v2, v1 - v3, v1 - ray.e) / detA;
    if (!ray.inRange(t) || beta < 0 || gamma < 0 || beta + gamma > 1) return NULL;
    STPoint3 point = ray.at(t);
	STVector3 normal = (1-beta-gamma)*n1+beta*n2+gamma*n3;
	STPoint2 uv = STPoint2((1-beta-gamma)*uv1.x+beta*uv2.x+gamma*uv3.x,(1-beta-gamma)*uv1.y+beta*uv2.y+gamma*uv3.y);
    return new Intersection(t, point, normal, uv);
}

bool Triangle::doesIntersect(const Ray& ray) {
    float detA = determinant(v1 - v2, v1 - v3, ray.d);
    if (detA == 0.) return false;
    float beta  = determinant(v1 - ray.e, v1 - v3, ray.d) / detA;
    float gamma = determinant(v1 - v2, v1 - ray.e, ray.d) / detA;
    float t     = determinant(v1 - v2, v1 - v3, v1 - ray.e) / detA;
    return ray.inRange(t) && beta >= 0 && gamma >= 0 && beta + gamma <= 1;
}

Intersection** Triangle::getIntersections (const Ray & ray) {
    Intersection *inter = getIntersect(ray);
    Intersection ** result = &inter;
    return result;
}

AABB* Triangle::getAABB()
{
    float xmin, xmax, ymin, ymax, zmin, zmax;
    if(v1.x > v2.x){xmin = v2.x;xmax = v1.x;}
    else{xmin = v1.x;xmax = v2.x;}
    if(v3.x < xmin)xmin = v3.x;
    if(v3.x > xmax)xmax = v3.x;
    
    if(v1.y > v2.y){ymin = v2.y;ymax = v1.y;}
    else{ymin = v1.y;ymax = v2.y;}
    if(v3.y < ymin)ymin = v3.y;
    if(v3.y > ymax)ymax = v3.y;
    
    if(v1.z > v2.z){zmin = v2.z;zmax = v1.z;}
    else{zmin = v1.z;zmax = v2.z;}
    if(v3.z < zmin)zmin = v3.z;
    if(v3.z > zmax)zmax = v3.z;
    
    return new AABB(xmin, xmax, ymin, ymax, zmin, zmax);
}
