//#####################################################################
// Stanford CS148 Ray Tracer Example Code
// Copyright 2012, Ben Mildenhall
//#####################################################################

#include "Sphere.h"

Intersection* Sphere::getIntersect(const Ray &ray) {
	float a = ray.d.LengthSq();
	float b = 2 * STVector3::Dot(ray.d, ray.e - center);
	float c = (ray.e - center).LengthSq() - radius * radius;
	float disc = b * b - 4 * a * c;
	if (disc < 0.) return NULL;
	float t1 = (-b - sqrt(disc)) / (2 * a);
	float t2 = (-b + sqrt(disc)) / (2 * a);
	if (!(ray.inRange(t1) || ray.inRange(t2))) return NULL;
	float t = (ray.inRange(t1) ? t1 : t2);
	STPoint3 point = ray.at(t);
	STVector3 normal = point - center;
	normal.Normalize();
	return new Intersection(t, point, normal);
}

Intersection** Sphere::getIntersections(const Ray &ray) {
    Intersection ** result = new Intersection*[maxInt];
    for (int i = 0; i < maxInt; i++) result[i] = NULL;
    
    float a = ray.d.LengthSq();
    float b = 2 * STVector3::Dot(ray.d, ray.e - center);
    float c = (ray.e - center).LengthSq() - radius * radius;
    float disc = b * b - 4 * a * c;
    if (disc < 0.) return result;
    float t1 = (-b - sqrt(disc)) / (2 * a);
    float t2 = (-b + sqrt(disc)) / (2 * a);
    if (!(ray.inRange(t1) || ray.inRange(t2))) return result;
    
    STPoint3 point = ray.at(t1);
    STVector3 normal = point - center;
    normal.Normalize();
    Intersection *inter1 = new Intersection(t1, point, normal);
    
    point = ray.at(t2);
    normal = point - center;
    normal.Normalize();
    Intersection *inter2 = new Intersection(t2, point, normal);
    
    if (!ray.inRange(t1)) {
        delete inter1;
        result[0] = inter2;
        return result;
    }
    if (!ray.inRange(t2)) {
        delete inter2;
        result[0] = inter1;
        return result;
    }
    
    if (t1 <= t2) {
        result[0] = inter1;
        result[1] = inter2;
    } else if (t1 == t2) {
        result[0] = inter1;
        delete inter2;
    } else {
        result[0] = inter2;
        result[1] = inter1;
    }
    return result;
}

bool Sphere::doesIntersect(const Ray &ray) {
    float a = ray.d.LengthSq();
    float b = 2 * STVector3::Dot(ray.d, ray.e - center);
    float c = (ray.e - center).LengthSq() - radius * radius;
    float disc = b * b - 4 * a * c;
    if (disc < 0.) return false;
    float t1 = (b * b - sqrt(disc)) / (2 * a);
    float t2 = (b * b + sqrt(disc)) / (2 * a);
    return ray.inRange(t1) || ray.inRange(t2);
}

bool Sphere::isInsideClosed(const STPoint3 &pt) {
    return ((pt - center).LengthSq() <= radius * radius + .001);
}

bool Sphere::isInsideOpen(const STPoint3 &pt) {
    return ((pt - center).LengthSq() < radius * radius - .001);
}

AABB* Sphere::getAABB()
{
    return new AABB(center.x - radius, center.x + radius, center.y - radius, center.y + radius, center.z - radius, center.z + radius);
}
