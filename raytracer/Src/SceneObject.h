//#####################################################################
// Stanford CS148 Ray Tracer Example Code
// Copyright 2012, Ben Mildenhall
//#####################################################################

#ifndef RayTracer_SceneObject_h
#define RayTracer_SceneObject_h

#include "Shape.h"
#include "Ray.h"
#include "Material.h"
#include "STTransform4.h"
#include <random>

class SceneObject {
public:
    SceneObject(Shape* _shape=NULL, const Material* _material=NULL, const STTransform4* _transform=NULL, const int _texture_index=-1)
    : shape(_shape), aabb(NULL), material(_material==NULL?Material():(*_material)), transform(_transform==NULL?STTransform4::Identity():(*_transform)), texture_index(_texture_index), name("scene_object")
    {
        tInverse = transform.Inverse();
        tInverseTranspose = tInverse.Transpose();

        if (_transform != NULL) transformFinal = transform;
        if(shape)aabb=shape->getAABB();if(aabb)aabb->rescale(transform);
        if(shape)name=shape->name;
        motionBlur = false;
    }

    ~SceneObject()
    {
        //if(shape!=NULL){delete shape;shape=NULL;} ////may cause memory leak here
        if(aabb!=NULL){delete aabb;aabb=NULL;}
    }

    virtual bool doesIntersect(const Ray& ray) 
    { return shape->doesIntersect(ray.transform(tInverse)); }
    
    virtual Intersection* getIntersect(const Ray& ray, double t=0.) 
    {
        if (motionBlur) {
            STTransform4 tempTransform, tempTInverse, temptTInverseTranspose;

            tempTransform = transform + t*(transformFinal - transform);
            tempTInverse = tempTransform.Inverse();
            temptTInverseTranspose = tempTInverse.Transpose();

            Intersection *inter = shape->getIntersect(ray.transform(tempTInverse));
            if (!inter) return NULL;
            inter->point = tempTransform * inter->point;
            inter->normal = temptTInverseTranspose * inter->normal;
            inter->normal.Normalize();
            return inter;
        }


        Intersection *inter = shape->getIntersect(ray.transform(tInverse));
        if (!inter) return NULL;
        inter->point = transform * inter->point;
        inter->normal = tInverseTranspose * inter->normal;
        inter->normal.Normalize();
        return inter;
    }

    virtual Intersection* getIntersectionWithObject(const Ray& ray, /*result*/SceneObject*& intersected_object)
    {
        double t = 0;
        if (motionBlur) {
            t = ((double)rand()/(double)RAND_MAX);
        }
        Intersection* inter = getIntersect(ray, t);
        if(inter){intersected_object=this;}
        return inter;
    }

    virtual void setTranslation(double x, double y, double z) {
        motionBlur = true;
        transformFinal = transform * STTransform4::Translation(x, y, z);
    }

    Shape* shape;
    AABB* aabb;
    Material material;
	int texture_index;
    STTransform4 transform, tInverse, tInverseTranspose, transformFinal;
    std::string name;
    // int tx; int ty int tz;
    bool motionBlur;

private:
    SceneObject(const SceneObject& copy)    ////shallow copy
        : shape(copy.shape), aabb(copy.aabb), material(copy.material), transform(copy.transform), tInverse(copy.tInverse), tInverseTranspose(copy.tInverseTranspose), name(copy.name)
    {}
};

#endif
