#ifndef EXAMPLE_SCENE_H
#define EXAMPLE_SCENE_H

#include "Scene.h"

class ExampleScene : public Scene
{
public:
    using Scene::Render;
    using Scene::initializeSceneFromScript;
    
    ExampleScene(){}
    ~ExampleScene(){}

    ////initializing scenes
    ////example scenes assignment 4: basic geometry, shading, texturing, and transparency
    void initializeSceneBasicGeometry();
    void initializeSceneBasicLightingAndShading();
    void initializeSceneTransform();
	void initializeSceneObjMesh();      ////totoro
    void initializeSceneObjMesh2();     ////turbosonic
	void initializeSceneTexture();
    void initializeSceneTransparentObject();
	void initializeSceneTransparentObject2();
    
    ////example scenes for assignment 5: acceleration structures
    void initializeSceneAccelerationStructureGrid();   ////uniform distributed
    void initializeSceneAccelerationStructureBVH();   ////clustered objects

    ////example assignment 6: partipating media and camera effects
    void initializeSceneParticipatingMedia();
    void initializeSceneDepthOfField();
	
	////void commonly used functions
	void addGround(const STPoint3& min_corner,const STVector2& size,bool counterclockwise=true);
	void addBackgroundWall(const STPoint3& min_corner,const STVector2& size,bool counterclockwise=true);
	void addWall(const STPoint3& min_corner,const STVector3& u,const STVector3& v,bool counterclockwise=true);
};

#endif

