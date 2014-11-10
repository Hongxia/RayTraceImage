//#####################################################################
// Stanford CS148 Ray Tracer Example Code
// Copyright 2012, Ben Mildenhall
//#####################################################################

#ifndef RayTracer_ObjectGroup_h
#define RayTracer_ObjectGroup_h

#include "SceneObject.h"
#include <vector>

class ObjectGroup : public SceneObject{
public:
    ObjectGroup(SceneObject *_object)
    : object(_object), hasSubObject(false) {
        name="object_group";
        aabb=object->aabb;
    }
    ~ObjectGroup()
    {
        for(int i=0;i<(int)contained.size();i++){
            if(contained[i]!=NULL)delete contained[i];
        }
        if(object!=NULL)delete object;
    }

    void addSubObject(ObjectGroup *vol) {
        contained.push_back(vol);
        hasSubObject=true;
    }

    bool doesIntersect(const Ray& ray) {return object->doesIntersect(ray);}

    Intersection* getIntersect(const Ray& ray) {return object->getIntersect(ray);}

    Intersection* getIntersectionWithObject(const Ray& ray, /*result*/SceneObject*& intersected_object)
    {
        ////single object
        if(!hasSubObject){
            Intersection* inter = object->getIntersect(ray);
            if(inter){intersected_object=object;return inter;}
            else return NULL;
        }
        ////has sub-objects
        else{
            if(doesIntersect(ray)){////test the bounding volume first
                SceneObject* sub_object=NULL;
                SceneObject* min_object=NULL;
                Intersection* min_inter=NULL;
                for (int i = 0; i < (int)contained.size(); i++) {    ////traverse all sub-objects
                    Intersection *inter = contained[i]->getIntersectionWithObject(ray,sub_object);

                    ////find the nearest intersected sub-object
                    if (inter && (!min_inter || inter->t < min_inter->t) && ray.inRange(inter->t)) {
                        if (min_inter) delete min_inter;
                        min_inter = inter;
                        min_object = sub_object;
                    } 
                    else delete inter;
                }
                intersected_object=min_object;
                return min_inter;
            }
            else return NULL;
        }
    }

    SceneObject *object;    ////if having sub-object it is a bounding volume, else a common object
    std::vector<ObjectGroup*> contained;
    bool hasSubObject;
};

#endif