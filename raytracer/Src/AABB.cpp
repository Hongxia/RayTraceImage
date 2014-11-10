//#####################################################################
// Stanford CS148 Ray Tracer Example Code
// Copyright 2013, Yue Chen
//#####################################################################

#include "AABB.h"

AABB::AABB(){}

AABB::AABB(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax)
{
    this->xmin = xmin;
    this->xmax = xmax;
    this->ymin = ymin;
    this->ymax = ymax;
    this->zmin = zmin;
    this->zmax = zmax;
    this->xcenter = .5f*(xmin + xmax);
    this->ycenter = .5f*(ymin + ymax);
    this->zcenter = .5f*(zmin + zmax);
}

void AABB::rescale(const STTransform4& transform)
{
    std::vector<STPoint3> pts;
    pts.push_back(STPoint3(xmin, ymin, zmin));
    pts.push_back(STPoint3(xmin, ymin, zmax));
    pts.push_back(STPoint3(xmin, ymax, zmin));
    pts.push_back(STPoint3(xmin, ymax, zmax));
    pts.push_back(STPoint3(xmax, ymin, zmin));
    pts.push_back(STPoint3(xmax, ymin, zmax));
    pts.push_back(STPoint3(xmax, ymax, zmin));
    pts.push_back(STPoint3(xmax, ymax, zmax));
    pts[0] = transform * pts[0];
    pts[1] = transform * pts[1];
    pts[2] = transform * pts[2];
    pts[3] = transform * pts[3];
    pts[4] = transform * pts[4];
    pts[5] = transform * pts[5];
    pts[6] = transform * pts[6];
    pts[7] = transform * pts[7];
    xmin = xmax = pts[0].x;
    ymin = ymax = pts[0].y;
    zmin = zmax = pts[0].z;
    for(int i = 0;i < 8;++i)
    {
        if(pts[i].x < xmin) xmin = pts[i].x;
        if(pts[i].x > xmax) xmax = pts[i].x;
        if(pts[i].y < ymin) ymin = pts[i].y;
        if(pts[i].y > ymax) ymax = pts[i].y;
        if(pts[i].z < zmin) zmin = pts[i].z;
        if(pts[i].z > zmax) zmax = pts[i].z;
    }
    xcenter = .5f*(xmin + xmax);
    ycenter = .5f*(ymin + ymax);
    zcenter = .5f*(zmin + zmax);
}

float AABB::intersect(const Ray& ray,int& i_intersect)
{
    float ax = 1.f / ray.d.x;
    float ay = 1.f / ray.d.y;
    float az = 1.f / ray.d.z;
    float txnear, txfar, tynear, tyfar, tznear, tzfar;
    if(ray.d.x == 0.f){
        if(ray.e.x >=xmin && ray.e.x <= xmax){
            txnear = -FLT_MAX; 
            txfar = FLT_MAX;
        }
    }
    else{
        if(ax >= 0.f){
            txnear = ax * (xmin - ray.e.x);
            txfar = ax * (xmax - ray.e.x);
        }
        else{
            txnear = ax * (xmax - ray.e.x);
            txfar = ax * (xmin - ray.e.x);
        }
    }

    if(ray.d.y == 0.f){
        tynear = -FLT_MAX; 
        tyfar = FLT_MAX;
    }
    else{
        if(ay >= 0.f){
            tynear = ay * (ymin - ray.e.y);
            tyfar = ay * (ymax - ray.e.y);
        }
        else{
            tynear = ay * (ymax - ray.e.y);
            tyfar = ay * (ymin - ray.e.y);
        }
    }

    if(ray.d.z == 0.f){
        tznear = -FLT_MAX; 
        tzfar = FLT_MAX;
    }
    else{
        if(az >= 0.f){
            tznear = az * (zmin - ray.e.z);
            tzfar = az * (zmax - ray.e.z);
        }
        else{
            tznear = az * (zmax - ray.e.z);
            tzfar = az * (zmin - ray.e.z);
        }
    }

    int inear = -1;
    float tnear = maxWithIndex(txnear, tynear, tznear, inear);
    int ifar = -1;
    float tfar = minWithIndex(txfar, tyfar, tzfar, ifar);

    float t_intersect = -1.f;
    
    i_intersect = -1;
    if(tnear <= tfar){
        if(txnear < 0.f && tynear < 0.f && tznear < 0.f){t_intersect = tfar;i_intersect = ifar;} ////e is inside the box
        else{t_intersect = tnear;i_intersect = inear;}

        if(/*!ray.inRange(t_intersect)*/t_intersect < 0.f || t_intersect > ray.t_max){t_intersect = -1.f;i_intersect = -1;}
    }
    return t_intersect;
}

Intersection* AABB::getIntersect(const Ray& ray)
{
	int face=0;float t=intersect(ray,face);if(t==-1.f)return NULL;
	STPoint3 point=ray.at(t);
	STVector3 normal=STVector3::Zero;normal.Component(face)=ray.d.Component(face)>=0.f?-1.f:1.f;
	normal.Normalize();
	return new Intersection(t,point,normal);
}

bool AABB::doesIntersect(const Ray& ray)
{
	int dummy_idx=0;return intersect(ray,dummy_idx)!=-1.f;
}

bool AABB::isInside(const STPoint3& point)
{
	return point.x>=xmin && point.x<=xmax && point.y>=ymin && point.y<=ymax && point.z>=zmin && point.z<=zmax;
}

void AABB::combine(const AABB* b1,const AABB* b2,/*result*/AABB* c)
{
    c->xmin = b1->xmin < b2->xmin ? b1->xmin : b2->xmin;
    c->xmax = b1->xmax > b2->xmax ? b1->xmax : b2->xmax;
    c->ymin = b1->ymin < b2->ymin ? b1->ymin : b2->ymin;
    c->ymax = b1->ymax > b2->ymax ? b1->ymax : b2->ymax;
    c->zmin = b1->zmin < b2->zmin ? b1->zmin : b2->zmin;
    c->zmax = b1->zmax > b2->zmax ? b1->zmax : b2->zmax;
}

AABB* AABB::combine(const AABB* b1, const AABB* b2)
{
    AABB* c=new AABB();
    AABB::combine(b1,b2,c);
    return c;
}

void AABB::combine(const STPoint3& p,/*result*/AABB* c)
{
    if(p.x<c->xmin)c->xmin=p.x;
    if(p.x>c->xmax)c->xmax=p.x;
    if(p.y<c->ymin)c->ymin=p.y;
    if(p.y>c->ymax)c->ymax=p.y;
    if(p.z<c->zmin)c->zmin=p.z;
    if(p.z>c->zmax)c->zmax=p.z;
}
