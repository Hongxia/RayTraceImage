//#####################################################################
// Stanford CS148 Ray Tracer Example Code
// Copyright 2013, Yue Chen
//#####################################################################

#ifndef AABB_h
#define AABB_h

#include "st.h"
#include "Ray.h"
#include "Intersection.h"
#include <iostream>
#include <vector>
#include <algorithm>

class AABB
{
public:
    AABB();
    AABB(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);
    ~AABB(){}

    bool doesIntersect(const Ray& ray);
	Intersection* getIntersect(const Ray& ray);
	bool isInside(const STPoint3& point);
    float intersect(const Ray& ray,int& intersected_face);

    void rescale(const STTransform4& transform);
    void display();

	friend std::ostream& operator<<(std::ostream& out,const AABB& aabb){out<<"["<<aabb.xmin<<", "<<aabb.ymin<<", "<<aabb.zmin<<"]["<<aabb.xmax<<", "<<aabb.ymax<<", "<<aabb.zmax<<"]";return out;}

    float xcenter, ycenter, zcenter;
    float xmin, xmax, ymin, ymax, zmin, zmax;
	STPoint3 minCorner(){return STPoint3(xmin,ymin,zmin);}
	STPoint3 maxCorner(){return STPoint3(xmax,ymax,zmax);}
    STVector3 edgeLength(){return STVector3(xmax-xmin,ymax-ymin,zmax-zmin);}
    float maxEdgeLength(){STVector3 edge_length=edgeLength();return getMax(edge_length.x,edge_length.y,edge_length.z);}
    void enlarge(const float offset){xmin-=offset;xmax+=offset;ymin-=offset;ymax+=offset;zmin-=offset;zmax+=offset;}
    void enlarge(const float offset[3]){xmin-=offset[0];xmax+=offset[0];ymin-=offset[1];ymax+=offset[1];zmin-=offset[2];zmax+=offset[2];}

    static void combine(const AABB* b1,const AABB* b2,/*result*/AABB* c);
    static AABB* combine(const AABB* b1, const AABB* b2);
    static void combine(const STPoint3& p,/*result*/AABB* c);

    static float minWithIndex(const float x,const float y,const float z,/*result*/int& min_idx)
    {
        min_idx=0;float min_value=x;
        if(y<min_value){min_idx=1;min_value=y;}
        if(z<min_value){min_idx=2;min_value=z;}
        return min_value;
    }

    static float maxWithIndex(const float x,const float y,const float z,/*result*/int& max_idx)
    {
        max_idx=0;float max_value=x;
        if(y>max_value){max_idx=1;max_value=y;}
        if(z>max_value){max_idx=2;max_value=z;}
        return max_value;
    }

    static float getMin(const float a, const float b, const float c)
    {
        float min_value=a;if(b<min_value)min_value=b;if(c<min_value)min_value=c;return min_value;
    }

    static float getMax(const float a, const float b, const float c)
    {
        float max_value=a;if(b>max_value)max_value=b;if(c>max_value)max_value=c;return max_value;
    }
};

#endif