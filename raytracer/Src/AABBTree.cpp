//#####################################################################
// Stanford CS148 Ray Tracer Example Code
// Copyright 2013, Yue Chen
//#####################################################################

#include "AABBTree.h"

AABBTreeNode::AABBTreeNode(SceneObject* obj, AABB* aabb)
{
    this->object = obj;
    this->left = NULL;
    this->right = NULL;
    this->aabb = aabb;
}

bool compare1(SceneObject* obj1, SceneObject* obj2)
{
    return obj1->aabb->xcenter < obj2->aabb->xcenter;
}

bool compare2(SceneObject* obj1, SceneObject* obj2)
{
    return obj1->aabb->ycenter < obj2->aabb->ycenter;
}

bool compare3(SceneObject* obj1, SceneObject* obj2)
{
    return obj1->aabb->zcenter < obj2->aabb->zcenter;
}

////construct a aabb tree
AABBTreeNode::AABBTreeNode(std::vector<SceneObject*>& objs, int method)
{
	int num = objs.size();
	if(num == 1){
		this->object = objs[0];
		this->left = NULL;
		this->right = NULL;
		this->aabb = objs[0]->aabb;
	}
	else if(num == 2){
		this->object = NULL;
		this->left = new AABBTreeNode(objs[0], objs[0]->aabb);
		this->right = new AABBTreeNode(objs[1], objs[1]->aabb);
		this->aabb = AABB::combine(objs[0]->aabb, objs[1]->aabb);
	}
	else{
		if(method == AABBTreeNode::VOL){
			float xsum = 0, ysum = 0, zsum = 0;
			float xdev = 0, ydev = 0, zdev = 0;
			int size = objs.size();
			for(int i = 0;i < size;++i){
				xsum += objs[i]->aabb->xcenter;
				ysum += objs[i]->aabb->ycenter;
				zsum += objs[i]->aabb->zcenter;
			}
			xsum /= size;
			ysum /= size;
			zsum /= size;
			for(int i = 0;i < size;++i){
				xdev += (objs[i]->aabb->xcenter - xsum) * (objs[i]->aabb->xcenter - xsum);
				ydev += (objs[i]->aabb->ycenter - ysum) * (objs[i]->aabb->ycenter - ysum);
				zdev += (objs[i]->aabb->zcenter - zsum) * (objs[i]->aabb->zcenter - zsum);
			}
			int idx = xdev > ydev ? 1 : 2;
			if(idx == 1)
				idx = xdev > zdev ? 1 : 3;
			else
				idx = ydev > zdev ? 2 : 3;

			std::vector<SceneObject*> lobjs;
			std::vector<SceneObject*> robjs;
			if(idx == 1){
				for(int i = 0;i < size;++i){
					if(objs[i]->aabb->xcenter < xsum) lobjs.push_back(objs[i]);
					else robjs.push_back(objs[i]);
				}
			}
			else if(idx == 2){
				for(int i = 0;i < size;++i){
					if(objs[i]->aabb->ycenter < ysum) lobjs.push_back(objs[i]);
					else robjs.push_back(objs[i]);
				}
			}
			else if(idx == 3){
				for(int i = 0;i < size;++i){
					if(objs[i]->aabb->zcenter < zsum) lobjs.push_back(objs[i]);
					else robjs.push_back(objs[i]);
				}
			}


			std::vector<SceneObject*>().swap(objs);
			this->object = NULL;
			if(lobjs.size()==0||robjs.size()==0){	////having duplicate triangles
				if(lobjs.size()>0){
					this->object = lobjs[0];
					this->left = NULL;
					this->right = NULL;
					this->aabb = lobjs[0]->aabb;
				}
				else if(robjs.size()>0){
					this->object = robjs[0];
					this->left = NULL;
					this->right = NULL;
					this->aabb = robjs[0]->aabb;
				}
				return;
			}

			this->left = new AABBTreeNode(lobjs, method);
			this->right = new AABBTreeNode(robjs, method);
			this->aabb = AABB::combine(this->left->aabb, this->right->aabb);
			//std::cout<<*(this->aabb)<<std::endl;
		}
		else if(method == AABBTreeNode::NUM){
			float xsum = 0, ysum = 0, zsum = 0;
			float xdev = 0, ydev = 0, zdev = 0;
			int size = objs.size();
			for(int i = 0;i < size;++i){
				xsum += objs[i]->aabb->xcenter;
				ysum += objs[i]->aabb->ycenter;
				zsum += objs[i]->aabb->zcenter;
			}
			xsum /= size;
			ysum /= size;
			zsum /= size;
			for(int i = 0;i < size;++i){
				xdev += (objs[i]->aabb->xcenter - xsum) * (objs[i]->aabb->xcenter - xsum);
				ydev += (objs[i]->aabb->ycenter - ysum) * (objs[i]->aabb->ycenter - ysum);
				zdev += (objs[i]->aabb->zcenter - zsum) * (objs[i]->aabb->zcenter - zsum);
			}
			int idx = xdev > ydev ? 1 : 2;
			if(idx == 1) idx = xdev > zdev ? 1 : 3;
			else idx = ydev > zdev ? 2 : 3;

			if(idx == 1) std::sort(objs.begin(), objs.end(), compare1);
			if(idx == 2) std::sort(objs.begin(), objs.end(), compare2);
			if(idx == 3) std::sort(objs.begin(), objs.end(), compare3);

			std::vector<SceneObject*> lobjs;
			std::vector<SceneObject*> robjs;
			for(int i = 0;i < num/2;++i) lobjs.push_back(objs[i]);
			for(int i = num/2;i < num;++i) robjs.push_back(objs[i]);

			std::vector<SceneObject*>().swap(objs);
			this->object = NULL;
			this->left = new AABBTreeNode(lobjs, method);
			this->right = new AABBTreeNode(robjs, method);
			this->aabb = AABB::combine(this->left->aabb, this->right->aabb);
		}
	}
}

AABBTreeNode::~AABBTreeNode()
{
    if(left!=NULL)delete left;
    if(right!=NULL)delete right;
}

Intersection* AABBTreeNode::getIntersectionWithObject(const Ray& ray, /*result*/SceneObject*& intersected_object)
{
	//std::cout<<"recursion parent: "<<*(this->aabb)<<std::endl;
	//if(this->left)std::cout<<"left: "<<*(this->left->aabb)<<std::endl;
	//if(this->right)std::cout<<"right: "<<*(this->right->aabb)<<std::endl;
    if(object != NULL){
        return object->getIntersectionWithObject(ray,intersected_object);
    }
    else{
        if(aabb->doesIntersect(ray)){
            SceneObject *left_obj = NULL;
            SceneObject *right_obj = NULL;
			//std::cout<<"trace left"<<std::endl;
            Intersection* left_inter = this->left->getIntersectionWithObject(ray, left_obj);
			//std::cout<<"trace right"<<std::endl;
            Intersection* right_inter = this->right->getIntersectionWithObject(ray, right_obj);
            if(left_inter!=NULL && right_inter!=NULL){
                if(left_inter->t < right_inter->t){
                    intersected_object = left_obj;
                    delete right_inter;
                    return left_inter;
                }
                else{
                    intersected_object = right_obj;
                    delete left_inter;
                    return right_inter;
                }
            }
            else if(left_inter!=NULL){
                intersected_object = left_obj;
                return left_inter;
            }
            else if(right_inter!=NULL){
                intersected_object = right_obj;
                return right_inter;
            }
            else return NULL;
        }
        else{
			//std::cout<<"recursion return"<<std::endl;
			return NULL;
		}
    }
}