//#####################################################################
// Stanford CS148 Ray Tracer Example Code
// Copyright 2013, Yue Chen
//#####################################################################

#ifndef AABBTree_h
#define AABBTree_h

#include "st.h"
#include "Shape.h"
#include "SceneObject.h"
#include "AABB.h"
#include <iostream>
#include <vector>
#include <algorithm>

class AABBTreeNode
{
public:
	AABBTreeNode(SceneObject* obj, AABB* aabb);
	AABBTreeNode(std::vector<SceneObject*>& objs, int method=1);
	~AABBTreeNode();
	Intersection* getIntersectionWithObject(const Ray& ray, /*result*/SceneObject*& intersected_object);

    ////two ways to initialize the AABB tree
	static const int VOL = 1;
	static const int NUM = 2;

	AABB* aabb;
	SceneObject* object;
	AABBTreeNode* left;
	AABBTreeNode* right;
};

class AABBTree : public SceneObject
{
    using SceneObject::name;
public:
    AABBTreeNode* root;

    AABBTree():root(NULL){name="aabb_tree";}
    AABBTree(std::vector<SceneObject*>& objs)
    {
        name="aabb_tree";
        root=new AABBTreeNode(objs);
		//std::cout<<"aabb tree, node number: "<<objs.size()<<std::endl;
    }
    ~AABBTree()
    {
        if(root!=NULL)delete root;
    }

    bool doesIntersect(const Ray& ray) 
    {
        if(root){
            SceneObject* dummy_object;
            Intersection* dummy_intersect=root->getIntersectionWithObject(ray,dummy_object);
            if(dummy_intersect!=NULL){delete dummy_intersect;return true;}
            else return false;
        }
        else return false;
    }

    Intersection* getIntersect(const Ray& ray) 
    {
        if(root){SceneObject* dummy;return root->getIntersectionWithObject(ray,dummy);}
        else return NULL;
    }

    Intersection* getIntersectionWithObject(const Ray& ray, /*result*/SceneObject*& intersected_object)
    {
        if(root)return root->getIntersectionWithObject(ray,intersected_object);
        else return NULL;
    }
};
#endif 