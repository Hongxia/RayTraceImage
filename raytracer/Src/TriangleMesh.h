#ifndef RayTracer_TriangleMesh_h
#define RayTracer_TriangleMesh_h

#include "Shape.h"
#include "Triangle.h"
#include "AABB.h"
#include "AABBTree.h"
#include "STTriangleMesh.h"

class TriangleMesh : public Shape {
public:
	TriangleMesh(STTriangleMesh& mesh_input,bool _counter_clockwise=true,bool calculate_smoothed_normal=false,
		bool read_normal_from_obj=true,bool read_tex_coord_from_obj=true,bool _use_accel_structure=true)
		:counter_clockwise(_counter_clockwise),mesh(mesh_input),use_accel_structure(_use_accel_structure)
	{
		this->name = "triangle_mesh";
		maxInt = 1;
		if(calculate_smoothed_normal){
			if(!counter_clockwise){
				for(int i=0;i<(int)mesh.mFaces.size();i++){
					for(int d=0;d<3;d++){
						(*mesh.mFaces[i]->normals[d])*=-1.;
					}
				}
			}
		}
		for(int i=0;i<(int)mesh.mFaces.size();i++){
			////calculate position
			STPoint3 v[3];
			for(int d=0;d<3;d++){v[d]=mesh.mFaces[i]->v[d]->pt;}
			if(!counter_clockwise){STPoint3 tmp=v[1];v[1]=v[2];v[2]=tmp;}
			
            ////calculate normal
            STVector3 n[3];
            for(int d=0;d<3;d++){
                if(mesh.mFaces[i]->normals[d]!=0)n[d]=*(mesh.mFaces[i]->normals[d]);
                else n[d]=mesh.mFaces[i]->normal;
            }
            //if(read_normal_from_obj||calculate_smoothed_normal){
            //for(int d=0;d<3;d++){n[d]=*(mesh.mFaces[i]->normals[d]);}
            //}
            //else{
            //for(int d=0;d<3;d++){n[d]=mesh.mFaces[i]->normal;}
            //}
            if(!counter_clockwise){STVector3 tmp=n[1];n[1]=n[2];n[2]=tmp;}

            ////calculate tex coordinate
            STPoint2 vt[3];
            for(int d=0;d<3;d++){
                if(mesh.mFaces[i]->texPos[d]!=0)vt[d]=*(mesh.mFaces[i]->texPos[d]);
            }
            //if(read_tex_coord_from_obj){
            //for(int d=0;d<3;d++){vt[d]=*(mesh.mFaces[i]->texPos[d]);}
            //}

			triangles.push_back(new SceneObject(new Triangle(v[0],v[1],v[2],n[0],n[1],n[2],vt[0],vt[1],vt[2])));
		}

		if(use_accel_structure){
			std::vector<SceneObject*> triangles_copy;triangles_copy.resize(triangles.size());
			for(int i=0;i<(int)triangles.size();i++){triangles_copy[i]=triangles[i];}
			aabb_tree=new AABBTree(triangles_copy);
		}
	}
    ~TriangleMesh()
    {
        for(int i=0;i<(int)triangles.size();i++){
            delete triangles[i];
        }
        if(aabb_tree) delete aabb_tree;
        delete &mesh;
    }
	Intersection* getIntersect(const Ray& ray);
	bool doesIntersect(const Ray& ray);
	AABB* getAABB();

	Intersection** getIntersections(const Ray& ray){return NULL;}
	bool isInsideOpen(const STPoint3& pt) { return false; }
	bool isInsideClosed(const STPoint3& pt) { return false; }
	
	bool counter_clockwise;
private:
	STTriangleMesh& mesh;
	std::vector<SceneObject*> triangles;
	AABBTree* aabb_tree;
	bool use_accel_structure;
};

#endif
