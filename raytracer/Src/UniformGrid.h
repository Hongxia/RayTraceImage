#ifndef UniformGrid_h
#define UniformGrid_h

#include "SceneObject.h"
#include "AABB.h"

class UniformGrid : public SceneObject
{
public:
	UniformGrid(std::vector<SceneObject *>& _objects,const AABB& _box,const int _counts[3]):objects(_objects)
    {
        updateGrid(_box,_counts);
        updateObjectsInCells();
    }

	void clearGridCells()
	{
		for(int i=0;i<(int)grid_cells.size();i++)grid_cells[i].clear();
	}

    void updateGrid(const AABB& _box,const int _counts[3])
    {
        box=_box;
        min_corner=box.minCorner();
        max_corner=box.maxCorner();

        for(int d=0;d<3;d++){
            counts[d]=_counts[d];
            dx[d]=(max_corner.Component(d)-min_corner.Component(d))/(float)counts[d];
            one_over_dx[d]=1.f/dx[d];
        }

        grid_cells.resize(counts[0]*counts[1]*counts[2]);
        clearGridCells();
        //std::cout<<"uniform grid: box: ["<<min_corner<<", "<<max_corner<<"], counts: "<<counts[0]<<", "<<counts[1]<<", "<<counts[2]<<", dx: "<<dx[0]<<", "<<dx[1]<<", "<<dx[2]<<std::endl;
    }

	void updateObjectsInCells()
	{
        clearGridCells();

		for(int obj_i=0;obj_i<(int)objects.size();obj_i++){
			AABB* aabb=objects[obj_i]->aabb;
			if(aabb){
				STPoint3 obj_aabb_min_corner=aabb->minCorner();
				STPoint3 obj_aabb_max_corner=aabb->maxCorner();
				int obj_min_idx[3];
                getCell(obj_aabb_min_corner,obj_min_idx);
                if(!isValidCell(obj_min_idx)){std::cout<<"invalid min cell idx for uniform grid"<<std::endl;continue;}	
				int obj_max_idx[3];
                getCell(obj_aabb_max_corner,obj_max_idx);
                if(!isValidCell(obj_max_idx)){std::cout<<"invalid max cell idx for uniform grid"<<std::endl;continue;}
				for(int i=obj_min_idx[0];i<=obj_max_idx[0];i++){
					for(int j=obj_min_idx[1];j<=obj_max_idx[1];j++){
						for(int k=obj_min_idx[2];k<=obj_max_idx[2];k++){
                            if(!isValidCell(i,j,k))continue;
							grid_cells[getIndex(i,j,k)].push_back(obj_i);
						}
					}
				}
			}
		}
	}

	bool doesIntersect(const Ray& ray) 
	{
		SceneObject* dummy_object;
        Intersection* dummy_intersection;
        dummy_intersection=getIntersectionWithObject(ray,dummy_object);
        if(dummy_intersection!=NULL){delete dummy_intersection;return true;}
        else return false;
	}

	Intersection* getIntersect(const Ray& ray) 
	{
		SceneObject* dummy;return getIntersectionWithObject(ray,dummy);
	}

	Intersection* getIntersectionWithObject(const Ray& ray, /*result*/SceneObject*& intersected_object)
	{		
        int s[3]={0,0,0};
        float dt[3]={0.f,0.f,0.f};

        int cell[3]={0,0,0};
        float t_next[3]={0.f,0.f,0.f};

        float big_value=1e10f;
        ////initialize s and dt
		for(int d=0;d<3;d++){
            if(ray.d.Component(d)>0.f){
                s[d]=1;dt[d]=dx[d]/ray.d.Component(d);
            }
            else if(ray.d.Component(d)<0.f){
                s[d]=-1;dt[d]=dx[d]/(-ray.d.Component(d));
            }
            else{s[d]=1;dt[d]=big_value;}
		}

        float t_min=ray.t_min;
        float t_max=ray.t_max;

        STPoint3 p;float epsilon=1e-3f;
        bool inside=false;
        float t_intersect=-1.f;
		int i_intersect=-1;
        if(box.isInside(ray.e)){p=ray.e;inside=true;}
        else{
            t_intersect=box.intersect(ray,i_intersect);
            if(t_intersect==-1.f)return NULL;
            p=ray.at(t_intersect+epsilon);
            t_min=t_intersect;
        }
        getCell(p,cell);

        ////initialize t_next
        for(int d=0;d<3;d++){
            if(s[d]>0){ ////positive direction
                if(ray.d.Component(d)!=0.f) t_next[d]=(min_corner.Component(d)+dx[d]*(cell[d]+1)-ray.e.Component(d))/ray.d.Component(d);
                else t_next[d]=big_value;
            }
            else{   ////negative direction
                if(ray.d.Component(d)!=0.f) t_next[d]=(min_corner.Component(d)+dx[d]*cell[d]-ray.e.Component(d))/ray.d.Component(d);
                else t_next[d]=big_value;
            }
        }
        //t_max=std::min(t_next[0],std::min(t_next[1],t_next[2]));
        t_max=AABB::getMin(t_next[0],t_next[1],t_next[2]);

        while(isValidCell(cell)){
            Intersection* inter=getIntersectionWithObjectInCell(cell,ray,t_min,t_max,intersected_object);
            if(inter!=NULL){return inter;}
            incrementalRayTraverse(ray,s,dt,t_min,t_max,cell,t_next);
        }

        return NULL;
	}

private:
    void incrementalRayTraverse(const Ray& ray, const int s[3], const float dt[3], /*result*/float& t_min,/*result*/float& t_max,/*result*/int cell[3], /*result*/float t_next[3])
    {
        int i_intersect=-1;
        float t_intersect=AABB::minWithIndex(t_next[0],t_next[1],t_next[2],i_intersect);

        cell[i_intersect]+=s[i_intersect];
        t_next[i_intersect]+=dt[i_intersect];
        t_min=t_intersect;
        //t_max=std::min(t_next[0],std::min(t_next[1],t_next[2]));
        t_max=AABB::getMin(t_next[0],t_next[1],t_next[2]);
    }

	Intersection* getIntersectionWithObjectInCell(const int cell[3], const Ray& ray, const float t_min,const float t_max,/*result*/SceneObject*& intersected_object)
	{
		int cell_idx=getIndex(cell[0],cell[1],cell[2]);

		SceneObject* min_object=NULL;
		Intersection* min_inter=NULL;

		for(int c=0;c<(int)grid_cells[cell_idx].size();c++){
			int obj_i=grid_cells[cell_idx][c];
			SceneObject* current_object=NULL;
			Intersection *inter = objects[obj_i]->getIntersectionWithObject(ray,current_object);
            
			if (inter && (!min_inter || inter->t < min_inter->t) && inter->t>=t_min && inter->t<=t_max) {
				if(min_inter)delete min_inter;
				min_inter = inter;
				min_object = current_object;
			} 
			else delete inter;
		}
		intersected_object=min_object;

		return min_inter;
	}

	void getCell(const STPoint3& p, /*result*/int idx[3])
	{
		for(int d=0;d<3;d++){
			idx[d]=(int)floor((p.Component(d)-min_corner.Component(d))*one_over_dx[d]);
		}
	}

	STPoint3 getCellMinCorner(int idx[3])
	{
		STVector3 offset;for(int d=0;d<3;d++)offset.Component(d)=(float)idx[d]*dx[d];
		return min_corner+offset;
	}

	STPoint3 getCellMaxCorner(int idx[3])
	{
		STVector3 offset;for(int d=0;d<3;d++)offset.Component(d)=(float)(idx[d]+1)*dx[d];
		return min_corner+offset;
	}

    bool isValidCell(const int idx[3])
    {
        return idx[0]>=0 && idx[0]<counts[0] && idx[1]>=0 && idx[1]<counts[1] && idx[2]>=0 && idx[2]<counts[2];
    }

    bool isValidCell(const int i,const int j,const int k)
    {
        return i>=0 && i<counts[0] && j>=0 && j<counts[1] && k>=0 && k<counts[2];
    }

	int getIndex(const int idx[3])
	{
		return getIndex(idx[0],idx[1],idx[2]);	
	}

	int getIndex(const int i,const int j,const int k)
	{
		return k*counts[0]*counts[1]+j*counts[0]+i;
	}

	std::vector<std::vector<int> > grid_cells;
	std::vector<SceneObject *>& objects;

    AABB box;
    int counts[3];
	STPoint3 min_corner;
    STPoint3 max_corner;
	float dx[3];
    float one_over_dx[3];
};

#endif