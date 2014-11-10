#include "TriangleMesh.h"

Intersection* TriangleMesh::getIntersect(const Ray& ray)
{
	if(use_accel_structure) return aabb_tree->getIntersect(ray);
	else{
		Intersection* min_inter = NULL;
		SceneObject* current_object=NULL;
		SceneObject* min_object = NULL;
		for (int i = 0; i < (int)triangles.size(); i++) {
			SceneObject* obj = triangles[i];
			Intersection *inter = obj->getIntersectionWithObject(ray,current_object);

			if (inter && (!min_inter || inter->t < min_inter->t) && ray.inRange(inter->t)) {
				if (min_inter) delete min_inter;
				min_inter = inter;
				min_object = current_object;
			} else delete inter;
		}
		return min_inter;
	}
}

bool TriangleMesh::doesIntersect(const Ray& ray)
{
	if(use_accel_structure) return aabb_tree->doesIntersect(ray);
	else{
		Intersection* min_inter = NULL;
		SceneObject* current_object=NULL;
		SceneObject* min_object = NULL;
		for (int i = 0; i < (int)triangles.size(); i++) {
			SceneObject* obj = triangles[i];
			Intersection *inter = obj->getIntersectionWithObject(ray,current_object);
			if (inter){delete inter;return true;}
		}
		return false;
	}
}

AABB* TriangleMesh::getAABB()
{
	STVector3 min_corner=STVector3(FLT_MAX);
	STVector3 max_corner=STVector3(-FLT_MAX);

	for(int i=0;i<(int)mesh.mVertices.size();i++){
		const STPoint3& v=mesh.mVertices[i]->pt;
		if(v.x<min_corner.x)min_corner.x=v.x;
		if(v.y<min_corner.y)min_corner.y=v.y;
		if(v.z<min_corner.z)min_corner.z=v.z;
		if(v.x>max_corner.x)max_corner.x=v.x; 
		if(v.y>max_corner.y)max_corner.y=v.y; 
		if(v.z>max_corner.z)max_corner.z=v.z;
	}
	return new AABB(min_corner.x,max_corner.x,min_corner.y,max_corner.y,min_corner.z,max_corner.z);
}
