#ifndef SCENE_H
#define SCENE_H

#include "st.h"
#include "RayTrace.h"

#include <iostream>

class Scene
{
public:
	Scene();
    virtual ~Scene();

	void Render();
    STColor3f TraceRay(const Ray& ray, int bounce = -1);
    Intersection* Intersect(const Ray& ray, /*result*/SceneObject *& object);

    std::string info();
    
    void initializeSceneFromScript(std::string sceneFilename);
    void buildAccelStructures(std::string& accel);

    ////ray tracing APIs
    void rtClear();
    void rtCamera(const STPoint3& eye, const STVector3& up, const STPoint3& lookAt, float fovy, float aspect);
    void rtOutput(int imgWidth, int imgHeight, const std::string& outputFilename);
    void rtBounceDepth(int depth);
    void rtShadowBias(float bias);
	void rtSampleRate(int sample_rate);
    void rtPushMatrix();
    void rtPopMatrix();
    void rtRotate(float rx, float ry, float rz);
    void rtScale(float sx, float sy, float sz);
    void rtTranslate(float tx, float ty, float tz);
    void rtInsideSphere(const STPoint3& center, float radius);
    void rtSphere(const STPoint3& center, float radius);
    void rtSphereWithMotion(const STPoint3& center, float radius, double tx, double ty, double tz);
    void rtTriangle(const STPoint3& v1, const STPoint3& v2, const STPoint3& v3);
    void rtTriangleLight(const STPoint3& v1, const STPoint3& v2, const STPoint3& v3);
	void rtTriangle(const STPoint3& v1, const STPoint3& v2, const STPoint3& v3, const STPoint2& uv1, const STPoint2& uv2, const STPoint2& uv3);
    void rtBox(const STPoint3& o, const STPoint3& x, const STPoint3& y, const STPoint3& z);
    void rtBox(const STPoint3& center,const STVector3& size);
    void rtCylinder(const STPoint3& A, const STPoint3 B, float radius);
    void rtParticipatingMedia(const STPoint3& center,const STVector3& size,const std::string& file_name);
    void rtCompound(char c);
    void rtGroupObjects(int num);
    void rtTriangleMesh(const std::string& file_name,const bool& counter_clockwise,const bool& smoothed_normal);    ////read geometry from the obj file, and load the geometry, the current material and current texture to ray tracer
    void rtTriangleMeshWithMaterialAndTexture(const std::string& file_name,const bool& counter_clockwise, const bool& smoothed_normal);    ////read geometry, material, and texture from the obj file and load them to ray tracer
    void rtTriangleMeshWithMotion(const std::string& file_name,const bool& counter_clockwise,const bool& smoothed_normal, double tx, double ty, double tz); 
    void rtAmbientLight(const STColor3f& col);
    void rtSpotLight(const STPoint3& loc, const STVector3& _direction, const double _cutoff, const STColor3f& col, const double _spot_exponent=0.0, const double _const_atten=1.0, const double _lin_atten=0.0, const double _quad_atten=0.0);
    void rtPointLight(const STPoint3& loc, const STColor3f& col);
    void rtDirectionalLight(const STVector3& dir, const STColor3f& col);
    void rtAreaLight(const STPoint3& v1, const STPoint3& v2, const STPoint3& v3, const STColor3f& col);
    void rtMaterial(const Material& mat);
	void rtMaterial(const STColor3f& amb, const STColor3f& diff, const STColor3f& spec, const STColor3f& mirr, float shine);
    void rtTransparentMaterial(const STColor3f& amb, const STColor3f& diff, const STColor3f& spec, const STColor3f& mirr, float shine, const STColor3f& refr, float sn);
    void rtSetFocus(const STPoint3& focal);
    void rtSetApeture(float a);
    void rtUseShadow(bool use=true){use_shadow=use;}
	void rtUseTransparentShadow(bool use=true){use_shadow=use;use_transparent_shadow=use;}
	void rtAttenuation(float a){attenuation_coefficient=a;}
	void rtLoadTexture(const std::string image_name,int& tex_index);
    void rtLoadTexture(STImage* texture_image,int& tex_index);
    void rtVolumetricTexture(VolumetricTexture* vt);
	void rtBindTexture(const int tex_id){currTexIndex=tex_id;}
	void rtUnbindTexture(){currTexIndex=-1;}

protected:
    std::vector<STTransform4> matStack;
    Material* currMaterial;
	int currTexIndex;
	bool use_shadow;
	bool use_transparent_shadow;
	std::vector<STImage*> textures;
    std::vector<VolumetricTexture*> volumetric_textures;
	
    int bounceDepth;
    float shadowBias;
    int width, height;
    std::string imageFilename;
    float focus;
    float aperture;
	float attenuation_coefficient;
    int sampleRate;

    Camera *camera;
    std::vector<SceneObject *> objects;
    std::vector<Light *> lights;
    std::vector<AreaLight *> areaLights;

	////texture
	STColor3f textureColor(const int texture_index,const STPoint2& uv);

    ////acceleration structures
    enum AccelStructure{NONE,AABB_TREE,UNIFORM_GRID} accel_structure;
    std::vector<AABBTree*> aabb_trees;
    UniformGrid* uniform_grid;

    ////intersection functions with acceleration structures
    Intersection* IntersectionNoAccelStructure(const Ray& ray, /*result*/SceneObject*& object);
    Intersection* IntersectAABBTree(const Ray& ray, /*result*/SceneObject*& object);
    Intersection* IntersectUniformGrid(const Ray& ray, /*result*/SceneObject*& object);

    void buildAABBTrees();	////build the entire scene as an aabb-tree
    void buildUniformGrids();	////build the entire scene as a uniform grid
    void fillLights(std::vector<Light *>& lights, Intersection* inter);
	void fillLightsWithAttenuation(std::vector<Light *>& lights,std::vector<STColor3f>& attenuations, Intersection* inter);
	STColor3f traceShadowRay(const Ray& ray,const Light& light);
    void getObjectsAABB(const std::vector<SceneObject*>& objs, /*result*/AABB& aabb);
};

#endif //SCENE_H

