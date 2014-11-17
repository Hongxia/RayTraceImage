#ifndef RAY_TRACING_SCENE_H
#define RAY_TRACING_SCENE_H

#include "Scene.h"

class RayTracingScene : public Scene
{
public:
    using Scene::Render;
    using Scene::initializeSceneFromScript;

    RayTracingScene(){};
    ~RayTracingScene(){};

    // Assignment 4
    void initializeScene();

    // Assignment 5
    void initializeUniformScene();
    void initializeClusteredScene();

    //void commonly used functions
    void addAreaLight(const STPoint3& min_corner, const STVector2& size, const STColor3f& col, const STColor3f& src_col, bool counterclockwise=true);
    void addGround(const STPoint3& min_corner,const STVector2& size,bool counterclockwise=true);
    void addBackgroundWall(const STPoint3& min_corner,const STVector2& size,bool counterclockwise=true);
    void addWall(const STPoint3& min_corner,const STVector3& u,const STVector3& v,bool counterclockwise=true);



};

#endif