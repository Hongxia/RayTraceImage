//#####################################################################
// Stanford CS148 Ray Tracer Example Code
// Copyright 2012, Ben Mildenhall
//#####################################################################

#ifndef RayTracer_Box_h
#define RayTracer_Box_h

#include "Shape.h"
#include "Triangle.h"

class Box : public Shape {
public:
    Box(const STPoint3& o, const STPoint3& x, const STPoint3& y, const STPoint3& z)
        : o(o), i(x - o), j(y - o), k(z - o)
    {
        maxInt = 2;
        this->name = "box";
        fillCob();
        initTriangles();
    }
    Box(const STPoint3& center,const STVector3& size)   ////axis aligned box
        : o(center-size*.5f),i(size.x,0.f,0.f),j(0.f,size.y,0.f),k(0.f,0.f,size.z)
    {
        maxInt = 2;
        this->name = "box";
        fillCob();
        initTriangles();
    }
    Box(const Box& copy)
    {
        cob=copy.cob;
        maxInt=copy.maxInt;
        this->name=copy.name;
        for(int i=0;i<=12;i++){
            if(copy.sides[i]!=NULL)this->sides[i]=new Triangle(*copy.sides[i]);
        }
    }
    ~Box()
    {
        for(int i=0;i<12;i++)delete sides[i];
    }

    Intersection* getIntersect(const Ray& ray) {
        Intersection *min_int = NULL, *inter;
        for (int i = 0; i < 12; i++) {
            inter = sides[i]->getIntersect(ray);
            if (inter && (!min_int || inter->t < min_int->t)) {
                if (min_int) delete min_int;
                min_int = inter;
            } else delete inter;
        }
        return min_int;
    }
    bool doesIntersect(const Ray& ray) {
        for (int i = 0; i < 12; i++) {
            bool inter = sides[i]->doesIntersect(ray);
            if (inter) return inter;
        }
        return false;
    }
    Intersection** getIntersections(const Ray& ray) {
        Intersection ** result = new Intersection*[maxInt];
        for (int i = 0; i < maxInt; i++) result[i] = NULL;
        Intersection *inter;
        int index = 0;
        for (int i = 0; i < 12; i++) {
            inter = sides[i]->getIntersect(ray);
            if (inter) {
                result[index] = inter;
                index++;
            }
        }
        return result;
    }
    bool isInsideOpen(const STPoint3& pt) {
        STVector3 coords = cob * (pt - o);
        return (coords.x > .001 && coords.y > .001 && coords.z > .001 &&
                coords.x < 1 - .001 && coords.y < 1 - .001 && coords.z < 1 - .001);
    }
    bool isInsideClosed(const STPoint3& pt) {
        STVector3 coords = cob * (pt - o);
        return (coords.x >= -.001 && coords.y >= -.001 && coords.z >= -.001 &&
                coords.x <= 1 + .001 && coords.y <= 1 + .001 && coords.z <= 1 + .001);
    }
    AABB* getAABB()
    {
        AABB* aabb=new AABB(FLT_MAX,-FLT_MAX,FLT_MAX,-FLT_MAX,FLT_MAX,-FLT_MAX);
        STVector3 min_corner;STVector3 max_corner;
        AABB::combine(o,aabb);AABB::combine(o+i,aabb);AABB::combine(o+j,aabb);AABB::combine(o+k,aabb);
        AABB::combine(o+i+j,aabb);AABB::combine(o+j+k,aabb);AABB::combine(o+k+i,aabb);AABB::combine(o+i+j+k,aabb);
        return aabb;
    }

private:
    STPoint3 o;
    STVector3 i, j, k;
    STTransform4 cob;
    Triangle *sides[12];
    void fillCob() {
        for (int ind = 0; ind < 3; ind++) {
            cob[ind][0] = this->i.Component(ind);
            cob[ind][1] = this->j.Component(ind);
            cob[ind][2] = this->k.Component(ind);
        }
        cob[3][3] = 1.;
        cob = cob.Inverse();
    }
    void initTriangles(){
        sides[0] = new Triangle(o, o + j, o + i + j);
        sides[1] = new Triangle(o, o + i + j, o + i);
        sides[2] = new Triangle(o + k, o + i + j + k, o + j + k);
        sides[3] = new Triangle(o + k, o + i + k, o + i + j + k);
        sides[4] = new Triangle(o, o + k, o + j + k);
        sides[5] = new Triangle(o, o + j + k, o + j);
        sides[6] = new Triangle(o + i, o + i + j + k, o + i + k);
        sides[7] = new Triangle(o + i, o + i + j, o + i + j + k);
        sides[8] = new Triangle(o, o + i, o + i + k);
        sides[9] = new Triangle(o, o + i + k, o + k);
        sides[10] = new Triangle(o + j, o + i + j + k, o + i + j);
        sides[11] = new Triangle(o + j, o + j + k, o + i + j + k);
    }
};

#endif
