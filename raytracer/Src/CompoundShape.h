//#####################################################################
// Stanford CS148 Ray Tracer Example Code
// Copyright 2012, Ben Mildenhall
//#####################################################################

#ifndef RayTracer_CompoundShape_h
#define RayTracer_CompoundShape_h

#include "Shape.h"
#include <iostream>

class CompoundShape : public Shape {
public:
    char kind;
    Shape *one, *two;
    float bias, c1, c2;

    CompoundShape(Shape*& _one, Shape*& _two, char _kind, float _bias)
    : one(_one), two(_two), bias(_bias) {
        maxInt = one->maxInt + two->maxInt;
        this->name = "compound";
        kind = _kind;
        switch (kind) {
                case 'u':
                    c1 = 1.; c2 = 1.;
                break;
                case 'd':
                    c1 = 1.; c2 = -1.;
                break;
                case 'i':
                    c1 = .5; c2 = .5;
                break;
        }
    }
    ~CompoundShape(){}

    Intersection* getIntersect(const Ray& ray) {
        Intersection **sorted = getAllIntersections(ray);
        int index;
        switch (kind) {
            case 'u':
                index = 0;
                while (index < maxInt && sorted[index]) {
                    if (!(one->isInsideOpen(sorted[index]->point) || two->isInsideOpen(sorted[index]->point)))
                        return sorted[index];
                    index++;
                }
                break;
            case 'd':
                index = 0;
                while (index < maxInt && sorted[index]) {
                    if (!one->isInsideOpen(sorted[index]->point) && one->isInsideClosed(sorted[index]->point) && !two->isInsideClosed(sorted[index]->point)) {
                        return sorted[index];
                    }
                    if (two->isInsideClosed(sorted[index]->point) && !two->isInsideOpen(sorted[index]->point) && one->isInsideClosed(sorted[index]->point)) {
                        sorted[index]->normal *= -1;
                        return sorted[index];
                        
                    }
                    index++;
                }
                break;
            case 'i':
                index = 0;
                while (index < maxInt && sorted[index]) {
                    if (one->isInsideClosed(sorted[index]->point) && two->isInsideClosed(sorted[index]->point))
                        return sorted[index];
                    index++;
                }
                break;
        }
        return NULL;
        
        Intersection** oneInt = one->getIntersections(ray);
        Intersection** twoInt = two->getIntersections(ray);
        if (kind == 'u') {
                Intersection *inter1 = oneInt[0];
                Intersection *inter2 = twoInt[0];
            delete [] oneInt;
            delete [] twoInt;
                if (!inter1) {
                    if (!inter2) return NULL;
                    return inter2;
                } else if (!inter2) {
                    return inter1;
                } else {
                    if (inter1->t <= inter2->t) return inter1;
                    else return inter2;
                }
        } else if (kind == 'i') {
            Intersection **sorted = getAllIntersections(ray);
            
            int index = 0, bound1 = 0, bound2 = 0;
            while (bound1 < one->maxInt && oneInt[bound1]) bound1++;
            while (bound2 < two->maxInt && twoInt[bound2]) bound2++;
            if (!bound1 || !bound2) return NULL;
            while (index < maxInt && sorted[index]) {
                int index1 = bound1 - 1, index2 = bound2 - 1;
                bool in1 = false, in2 = false;
                while (index1 >= 0 && sorted[index]->t <= oneInt[index1]->t) {
                    in1 = !in1;
                    if (sorted[index]->t == oneInt[index1]->t) in1 = true;
                    index1--; 
                }
                while (index2 >= 0 && sorted[index]->t <= twoInt[index2]->t) {
                    in2 = !in2;
                    if (sorted[index]->t == twoInt[index2]->t) in2 = true;
                    index2--; 
                }
                if (in1 && in2) return sorted[index];
                index++;
            }
            return NULL;
        } else if (kind == 'd' ) {
            Intersection **sorted = getIntersections(ray);
            
            int index = 0, bound1 = 0, bound2 = 0;
            while (bound1 < one->maxInt && oneInt[bound1]) bound1++;
            while (bound2 < two->maxInt && twoInt[bound2]) bound2++;
            if (bound1 || bound2) return NULL;
            while (index < maxInt && sorted[maxInt]) {
                int index1 = bound1 - 1, index2 = bound2 - 1;
                bool in1 = false, in2 = true;
                while (index1 >= 0 && sorted[index]->t < oneInt[index1]->t) {
                    index1--; in1 = !in1;
                }
                while (index2 >= 0 && sorted[index]->t < twoInt[index2]->t) {
                    index2--; in2 = !in2;
                }
                if (in1 && in2) return sorted[index];
                index++;
            }
            return NULL;
        }
        return NULL;
        
    }
    bool doesIntersect(const Ray& ray) {
        return getIntersect(ray) != NULL;
    }
    Intersection** getAllIntersections(const Ray& ray) {
        Intersection** oneInt = one->getIntersections(ray);
        Intersection** twoInt = two->getIntersections(ray);
        Intersection** result = new Intersection*[maxInt];
        for (int i = 0; i < maxInt; i++) result[i] = NULL;
        
        int index1 = 0, index2 = 0;
        Intersection *inter1 = oneInt[index1];
        Intersection *inter2 = twoInt[index2];
        while (inter1 || inter2) {
            if (!inter1) {
                result[index1 + index2] = inter2;
                index2++;
            } else if (!inter2) {
                result[index1 + index2] = inter1;
                index1++;
            } else {
                if (inter1->t <= inter2->t) {
                    result[index1 + index2] = inter1;
                    index1++;
                } else {
                    result[index1 + index2] = inter2;
                    index2++;
                }
            }
            
            
            inter1 = index1 < one->maxInt ? oneInt[index1] : NULL;
            inter2 = index2 < two->maxInt ? twoInt[index2] : NULL;
        }
        delete [] oneInt;
        delete [] twoInt;
        return result;
    }
    Intersection** getIntersections(const Ray& ray) {
        Intersection** oneInt = one->getIntersections(ray);
        Intersection** twoInt = two->getIntersections(ray);
        Intersection** result = new Intersection*[maxInt];
        for (int i = 0; i < maxInt; i++) result[i] = NULL;
        
        int index1 = 0, index2 = 0;
        Intersection *inter1 = oneInt[index1];
        Intersection *inter2 = twoInt[index2];
        while (inter1 || inter2) {
            if (!inter1) {
                result[index1 + index2] = inter2;
                index2++;
            } else if (!inter2) {
                result[index1 + index2] = inter1;
                index1++;
            } else {
                if (inter1->t <= inter2->t) {
                    result[index1 + index2] = inter1;
                    index1++;
                } else {
                    result[index1 + index2] = inter2;
                    index2++;
                }
            }
            
            
            inter1 = index1 < one->maxInt ? oneInt[index1] : NULL;
            inter2 = index2 < two->maxInt ? twoInt[index2] : NULL;
        }
        delete [] oneInt;
        delete [] twoInt;
        
        return result;
    }
    
    bool isInsideOpen(const STPoint3& pt) {
        switch (kind) {
            case 'u':
                return one->isInsideOpen(pt) || two->isInsideOpen(pt);
                break;
            case 'i':
                return one->isInsideOpen(pt) && two->isInsideOpen(pt);
                break;
            case 'd':
                return one->isInsideOpen(pt) && !two->isInsideClosed(pt);
                break;
        }
        return false;
    }
    bool isInsideClosed(const STPoint3& pt) {
        switch (kind) {
            case 'u':
                return one->isInsideClosed(pt) || two->isInsideClosed(pt);
                break;
            case 'i':
                return one->isInsideClosed(pt) && two->isInsideClosed(pt);
                break;
            case 'd':
                return one->isInsideClosed(pt) && !two->isInsideOpen(pt);
                break;
        }
        return false;
    }

    AABB* getAABB(){return NULL;/*TODO*/}
};

#endif
