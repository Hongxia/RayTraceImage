#include "RayTracingScene.h"

inline static double eu(double d) {return d+1e-4f;}
inline static double eb(double d) {return d-1e-4f;}

inline static STColor3f rgbToColor(double r, double g, double b) {return STColor3f(r/255.0, g/255.0, b/255.0);}
inline static STColor3f color_A() {return rgbToColor(237, 2, 11); }
inline static STColor3f color_F() {return rgbToColor(255, 169, 206); }
inline static STColor3f color_G() {return rgbToColor(64, 120, 211); }
inline static STColor3f color_H() {return rgbToColor(80, 168, 227); }
inline static STColor3f color_I() {return rgbToColor(173, 222, 250); }
inline static STColor3f color_J() {return rgbToColor(127, 176, 5); }
inline static STColor3f color_L() {return rgbToColor(204, 255, 0); }
inline static STColor3f color_M() {return rgbToColor(124, 20, 77); }
inline static STColor3f color_N() {return rgbToColor(127, 166, 51); }
inline static STColor3f color_O() {return rgbToColor(108, 92, 50); }

void RayTracingScene::initializeFinalScene()
{
    rtClear();

    // rtCamera(/*eye*/STPoint3(0.f,5.f,20.f),/*up*/STVector3(0.f,1.f,0.f),/*lookat*/STPoint3(0.f,4.f,0.f),/*fov*/45.f,/*aspect*/1.f);
    rtCamera(/*eye*/STPoint3(-18.f,5.f,16.f),/*up*/STVector3(0.f,1.f,0.f),/*lookat*/STPoint3(0.f,0.f,0.f),/*fov*/45.f,/*aspect*/1.f);
    rtOutput(/*width*/512,/*height*/512,/*path*/"../Standard_Tests/RayTraceScene.png");
    rtBounceDepth(2);
    rtUseShadow(true);
    rtSetApeture(16);
    rtShadowBias(1e-4f);
    rtSampleRate(1);

    rtAmbientLight(STColor3f(1.f,1.f,1.f));
    rtAreaLight(STPoint3(-10.f, 40.f, -10.f), STPoint3(10.f, 40.f, -10.f), STPoint3(-10.f, 40.f, 10.f), STColor3f(1.f,1.f,1.f));
    rtSpotLight(STPoint3(0.f, 40.f, 20.f), STVector3(0.f, -1.f, -0.5f), 18, STColor3f(.5f,.5f,.5f), 0.0);
    rtSpotLight(STPoint3(-16.f, 40.f, -20.f), STVector3(0.f, -1.f, 0.5f), 18, STColor3f(.5f,.5f,.5f), 0.0);
    rtSpotLight(STPoint3(-40.f, 40.f, 0.f), STVector3(0.6f, -1.f, 0.f), 18, STColor3f(.5f,.5f,.5f), 0.0);

    ////scene mesh
    rtPushMatrix();
    rtScale(8.f,8.f,8.f);

    // Ground
    rtTriangleMeshWithMaterialAndTexture("../Standard_Tests/ground/ground.obj",true,false);
    rtTriangleMeshWithMotion("../Standard_Tests/ground/car.obj",true,false,0,0,-0.25);
    rtTriangleMeshWithMaterialAndTexture("../Standard_Tests/ground/car.obj",true,false);
    rtTriangleMeshWithMotion("../Standard_Tests/ground/car1.obj",true,false,0,0,-0.35);
    rtTriangleMeshWithMaterialAndTexture("../Standard_Tests/ground/car1.obj",true,false);

    // Water on the ground
    // Material mat_mirror(STColor3f(1.f, 1.f, 1.f), STColor3f(0.f, 0.f, 0.f), STColor3f(0.f, 0.f, 0.f), STColor3f(.9f, .9f, .9f), 0.f);
    // rtMaterial(mat_mirror);
    // rtTriangleMeshWithMaterialAndTexture("../Standard_Tests/ground/water.obj",true,false);
    rtPopMatrix();
}

void RayTracingScene::initializeClusteredScene()
{
    rtClear();

    // rtCamera(/*eye*/STPoint3(-5.f,10.f,40.f),/*up*/STVector3(0.f,1.f,0.f),/*lookat*/STPoint3(0.f,8.f,0.f),/*fov*/45.f,/*aspect*/1.f);
    rtCamera(/*eye*/STPoint3(0.f,50.f,0.f),/*up*/STVector3(0.f,0.f,-1.f),/*lookat*/STPoint3(0.f,0.f,0.f),/*fov*/45.f,/*aspect*/1.f);
    rtOutput(/*width*/512,/*height*/512,/*path*/"../Standard_Tests/RayTraceClusteredScene.png");
    rtBounceDepth(1);
    rtUseShadow(true);
    rtSetApeture(16);
    rtShadowBias(1e-4f);
    rtSampleRate(4);

    rtAmbientLight(STColor3f(.05f,.05f,.05f));
    // rtAreaLight(STPoint3(-15.f, 40.f, -15.f), STPoint3(-5.f, 40.f, -15.f), STPoint3(-15.f, 40.f, -5.f), STColor3f(0.2f,0.2f,0.2f));
    // rtAreaLight(STPoint3(5.f, 40.f, -15.f), STPoint3(15.f, 40.f, -15.f), STPoint3(5.f, 40.f, -5.f), STColor3f(0.2f,0.2f,0.2f));
    // rtAreaLight(STPoint3(-15.f, 40.f, 5.f), STPoint3(-5.f, 40.f, 5.f), STPoint3(-15.f, 40.f, 15.f), STColor3f(0.2f,0.2f,0.2f));
    // rtAreaLight(STPoint3(5.f, 40.f, 5.f), STPoint3(15.f, 40.f, 5.f), STPoint3(5.f, 40.f, 15.f), STColor3f(0.2f,0.2f,0.2f));
    rtSpotLight(STPoint3(0.f, 20.f, 0.f), STVector3(0.f, -1.f, 0.f), 60, STColor3f(1.f,1.f,1.f), 4.0, 1.0, 0.05, 0.0);

    ////car mesh
    rtPushMatrix();
    rtTranslate(0.f,1.f,10.f);
    rtRotate(270.f,0.f,338.f);
    rtScale(10e-4f,10e-4f,10e-4f);
    rtTriangleMeshWithMaterialAndTexture("../Standard_Tests/turbosonic/turbosonic.obj",true,false);
    rtPopMatrix();

    rtPushMatrix();
    rtTranslate(-8.66f,1.f,-5.f);
    rtRotate(270.f,0.f,218.f);
    rtScale(10e-4f,10e-4f,10e-4f);
    rtTriangleMeshWithMaterialAndTexture("../Standard_Tests/turbosonic/turbosonic.obj",true,false);
    rtPopMatrix();

    rtPushMatrix();
    rtTranslate(8.66f,1.f,-5.f);
    rtRotate(270.f,0.f,98.f);
    rtScale(10e-4f,10e-4f,10e-4f);
    rtTriangleMeshWithMaterialAndTexture("../Standard_Tests/turbosonic/turbosonic.obj",true,false);
    rtPopMatrix();

    ////environment box
    // Material mat_floor(color_O(),color_O(),STColor3f(),STColor3f(),30.f);
    // ////ground
    // rtMaterial(mat_floor);
    // addGround(STPoint3(-20.f, 0.f, -20.f),STVector2(40.f, 40.f),true);

    ////use acceleration structure
    ////aabb tree
    // accel_structure=AABB_TREE;
    // AABBTree* aabb_tree=new AABBTree(objects);
    // aabb_trees.push_back(aabb_tree);

    ////uniform grid
    // accel_structure=UNIFORM_GRID;
    // AABB scene_bounding_box;getObjectsAABB(objects,scene_bounding_box);
    // int subdivision[3]={3,1,2};
    // uniform_grid=new UniformGrid(objects,scene_bounding_box,subdivision);
}

void RayTracingScene::initializeScene()
{
    rtClear();

    rtCamera(/*eye*/STPoint3(-5.f,10.f,40.f),/*up*/STVector3(0.f,1.f,0.f),/*lookat*/STPoint3(0.f,8.f,0.f),/*fov*/45.f,/*aspect*/1.f);
    rtOutput(/*width*/512,/*height*/512,/*path*/"../Standard_Tests/RayTraceScene.png");
    rtBounceDepth(8);
    rtUseShadow(true);
    rtSetApeture(16);
    rtShadowBias(1e-4f);
    rtSampleRate(16);

    rtAmbientLight(STColor3f(.05f,.05f,.05f));
    rtSpotLight(STPoint3(-6.f, 14.f, 8.f), STVector3(0.6f, -1.f, -0.2f), 20, STColor3f(.5f,.5f,.5f));
    rtSpotLight(STPoint3(9.f, 12.f, 12.f), STVector3(-0.7f, -1.f, -0.4f), 30, STColor3f(.5f,.5f,.5f));
    rtSpotLight(STPoint3(9.f, 14.f, -8.f), STVector3(-0.6f, -1.f, 0.8f), 20, STColor3f(.5f,.5f,.5f));
    addAreaLight(STPoint3(-5.f,eb(20.f),-5.f), STVector2(10.f, 10.f), STColor3f(.6f,.6f,.6f), STColor3f(1.f,1.f,1.f), true);
    addAreaLight(STPoint3(-5.f,eb(20.f),15.f), STVector2(10.f, 10.f), STColor3f(.3f,.3f,.3f), STColor3f(1.f,1.f,1.f), true);
    addAreaLight(STPoint3(-5.f,eb(20.f),35.f), STVector2(10.f, 10.f), STColor3f(.3f,.3f,.3f), STColor3f(1.f,1.f,1.f), true);

    Material mat_overlay_glass(/*ambient*/STColor3f(1.f, 0.f, 0.f),/*diffuse*/STColor3f(1.f, 0.f, 0.f),/*spec*/STColor3f(0.f,0.f,0.f),/*mirror*/STColor3f(0.f,0.f,0.f),/*shiness*/0.f,/*refr*/color_F(),/*sn*/1.5f);
    rtMaterial(mat_overlay_glass);
    rtBox(/*origin*/STPoint3(-8.5f,15.f,3.f),/*x*/STPoint3(8.5f,15.f,3.f), /*y*/STPoint3(-8.5f,15.f,13.f), /*z*/STPoint3(-8.5f,15.2f,3.f));

    Material mat_side_glass(/*ambient*/STColor3f(1.f, 0.f, 0.f),/*diffuse*/STColor3f(1.f, 0.f, 0.f),/*spec*/STColor3f(0.f,0.f,0.f),/*mirror*/STColor3f(0.f,0.f,0.f),/*shiness*/0.f,/*refr*/color_G(),/*sn*/1.5f);
    rtMaterial(mat_side_glass);
    rtBox(/*origin*/STPoint3(-8.5f,1.f,-8.5f),/*x*/STPoint3(-8.3f,1.f,-8.5f), /*y*/STPoint3(-8.5f,12.f,-8.5f), /*z*/STPoint3(-8.5f,1.f,20.f));
    rtBox(/*origin*/STPoint3(-7.2f,2.f,-8.5f),/*x*/STPoint3(-7.f,2.f,-8.5f), /*y*/STPoint3(-7.2f,8.f,-8.5f), /*z*/STPoint3(-7.2f,2.f,20.f));

    Material mat_mirror(STColor3f(1.f, 1.f, 1.f), STColor3f(0.f, 0.f, 0.f), STColor3f(0.f, 0.f, 0.f), STColor3f(.9f, .9f, .9f), 0.f);
    rtMaterial(mat_mirror);
    rtSphere(STPoint3(5.5f,2.5f,-2.5f), 2.5f);
    rtSphere(STPoint3(5.5f,2.f,5.f), 2.f);
    rtSphere(STPoint3(5.5f,1.5f,11.f), 1.5f);
    rtSphere(STPoint3(5.5f,1.f,15.5f), 1.f);
    rtSphere(STPoint3(5.5f,.5f,18.f), .5f);

    Material mat_plat(/*ambient*/STColor3f(1.f, 1.f, 1.f),/*diffuse*/color_O(),/*spec*/STColor3f(0.f,0.f,0.f),/*mirror*/STColor3f(0.f,0.f,0.f),/*shiness*/0.f,/*refr*/STColor3f(0.f,0.f,0.f),/*sn*/1.5f);
    rtMaterial(mat_plat);
    rtCylinder(STPoint3(-2.f,0.f,8.f), STPoint3(-2.f,1.f,8.f), 2.5f);
    rtCylinder(STPoint3(-2.f,1.f,8.f), STPoint3(-2.f,3.f,8.f), 2.f);
    rtCylinder(STPoint3(-2.f,3.f,8.f), STPoint3(-2.f,4.f,8.f), 2.5f);

    Material mat_car(/*ambient*/STColor3f(1.f, 0.f, 0.f),/*diffuse*/STColor3f(1.f, 0.f, 0.f),/*spec*/STColor3f(0.f,0.f,0.f),/*mirror*/STColor3f(0.f,0.f,0.f),/*shiness*/0.f,/*refr*/color_M(),/*sn*/1.5f);
    ////car mesh
    rtMaterial(mat_car);
    rtPushMatrix();
    rtTranslate(-2.f,eu(4.f),8.f);
    rtRotate(0,90.f,0.f);
    rtScale(2.f,2.f,2.f);
    rtTriangleMesh("../Meshes/car.obj",true,false);
    rtPopMatrix();

    ////environment box
    Material mat_left_wall(STColor3f(1.f,1.f,1.f),color_J(),STColor3f(),STColor3f(),30.f);
    Material mat_right_wall(STColor3f(1.f,1.f,1.f),color_L(),STColor3f(),STColor3f(),30.f);
    Material mat_background_wall(STColor3f(1.f,1.f,1.f),color_I(),STColor3f(),STColor3f(),30.f);
    Material mat_floor(STColor3f(.89f, .84f, .69f),color_O(),STColor3f(),STColor3f(),30.f);
    Material mat_white_wall(STColor3f(1.f,1.f,1.f),STColor3f(.8f,.8f,.8f),STColor3f(),STColor3f(),0.f);
    Material mat_ceiling(STColor3f(0.f,0.f,0.f),STColor3f(.0f,.0f,.0f),STColor3f(),STColor3f(),0.f);
    ////ground
    rtMaterial(mat_floor);
    addGround(STPoint3(-10.f,0.f,-10.f),STVector2(20.f,60.f),true);
    ////ceil
    rtMaterial(mat_ceiling);
    addGround(STPoint3(-10.f,20.f,-10.f),STVector2(20.f,60.f),false);
    ////background wall
    rtMaterial(mat_background_wall);
    addBackgroundWall(STPoint3(-10.f,0.f,-10.f),STVector2(20.f,20.f),true);
    ////forward wall
    rtMaterial(mat_background_wall);
    addBackgroundWall(STPoint3(-10.f,0.f,50.f),STVector2(20.f,20.f),false);
    ////left wall
    rtMaterial(mat_left_wall);
    addWall(STPoint3(-10.f,0.f,-10.f),STVector3(0.f,20.f,0.f),STVector3(0.f,0.f,60.f),true);
    ////right wall
    rtMaterial(mat_right_wall);
    addWall(STPoint3(10.f,0.f,-10.f),STVector3(0.f,20.f,0.f),STVector3(0.f,0.f,60.f),false);
}



void RayTracingScene::initializeUniformScene()
{
    rtClear();

    // rtCamera(/*eye*/STPoint3(0.f,25.f,10.f),/*up*/STVector3(0.f,1.f,0.f),/*lookat*/STPoint3(0.f,0.f,9.f),/*fov*/45.f,/*aspect*/1.f);
    rtCamera(/*eye*/STPoint3(0.f,18.f,40.f),/*up*/STVector3(0.f,1.f,0.f),/*lookat*/STPoint3(0.f,0.f,0.f),/*fov*/45.f,/*aspect*/1.f);
    rtOutput(/*width*/512,/*height*/512,/*path*/"../UniformScene.png");
    rtBounceDepth(8);
    rtBounceDepth(8);
    rtUseShadow(true);
    rtSetApeture(16);
    rtShadowBias(1e-4f);
    rtSampleRate(4);

    rtAmbientLight(STColor3f(.05f,.05f,.05f));
    rtSpotLight(STPoint3(-6.f, 14.f, 8.f), STVector3(0.4f, -1.f, -0.3f), 25, STColor3f(.5f,.5f,.5f));
    rtSpotLight(STPoint3(9.f, 12.f, 12.f), STVector3(-0.5f, -1.f, -0.18f), 35, STColor3f(.5f,.5f,.5f));
    rtSpotLight(STPoint3(9.f, 14.f, 25.0f), STVector3(-0.4f, -1.f, -0.05f), 25, STColor3f(.5f,.5f,.5f));
    addAreaLight(STPoint3(-5.f,eb(20.f),-5.f), STVector2(10.f, 10.f), STColor3f(.6f,.6f,.6f), STColor3f(1.f,1.f,1.f), true);
    addAreaLight(STPoint3(-5.f,eb(20.f),15.f), STVector2(10.f, 10.f), STColor3f(.3f,.3f,.3f), STColor3f(1.f,1.f,1.f), true);
    addAreaLight(STPoint3(-5.f,eb(20.f),35.f), STVector2(10.f, 10.f), STColor3f(.3f,.3f,.3f), STColor3f(1.f,1.f,1.f), true);

    ////environment box
    Material mat_left_wall(STColor3f(1.f,1.f,1.f),color_J(),STColor3f(),STColor3f(),30.f);
    Material mat_right_wall(STColor3f(1.f,1.f,1.f),color_L(),STColor3f(),STColor3f(),30.f);
    Material mat_background_wall(STColor3f(1.f,1.f,1.f),color_I(),STColor3f(),STColor3f(),30.f);
    Material mat_floor(STColor3f(.89f, .84f, .69f),color_O(),STColor3f(),STColor3f(),30.f);
    Material mat_white_wall(STColor3f(1.f,1.f,1.f),STColor3f(.8f,.8f,.8f),STColor3f(),STColor3f(),0.f);
    Material mat_ceiling(STColor3f(0.f,0.f,0.f),STColor3f(.0f,.0f,.0f),STColor3f(),STColor3f(),0.f);
    ////ground
    rtMaterial(mat_floor);
    addGround(STPoint3(-10.f,0.f,-10.f),STVector2(20.f,60.f),true);
    ////ceil
    rtMaterial(mat_ceiling);
    addGround(STPoint3(-10.f,20.f,-10.f),STVector2(20.f,60.f),false);
    ////background wall
    rtMaterial(mat_background_wall);
    addBackgroundWall(STPoint3(-10.f,0.f,-10.f),STVector2(20.f,20.f),true);
    ////forward wall
    rtMaterial(mat_background_wall);
    addBackgroundWall(STPoint3(-10.f,0.f,50.f),STVector2(20.f,20.f),false);
    ////left wall
    rtMaterial(mat_left_wall);
    addWall(STPoint3(-10.f,0.f,-10.f),STVector3(0.f,20.f,0.f),STVector3(0.f,0.f,60.f),true);
    ////right wall
    rtMaterial(mat_right_wall);
    addWall(STPoint3(10.f,0.f,-10.f),STVector3(0.f,20.f,0.f),STVector3(0.f,0.f,60.f),false);

    ////objects
    int counts[3]={16,1,48};
    float size[3]={16.f,3.f,48.f};
    STPoint3 min_corner(-size[0]*.5f,0.0f,-4.0f);
    STPoint3 max_corner(size[0]*.5f,size[1],size[2] - 4.0f);
    STVector3 dx(size[0]/(float)counts[0],size[1]/(float)counts[1],size[2]/(float)counts[2]);

    ////spheres and cubes
    for(int i=0;i<counts[0];i++){
        for(int j=0;j<counts[1];j++){
            for(int k=0;k<counts[2];k++){
                STColor3f color((float)rand()/RAND_MAX,(float)rand()/RAND_MAX,(float)rand()/RAND_MAX);
                Material mat(color,color,STColor3f(),STColor3f(),40.f);
                // STVector3 perturb(1,1,1);
                STVector3 perturb((float)rand()/RAND_MAX,(float)rand()/RAND_MAX,(float)rand()/RAND_MAX);
                STPoint3 center=min_corner+STVector3(dx.Component(0)*(float)(i+.5f+(perturb.Component(0)-.5f)*.2f),dx.Component(1)*(float)(j+.5f+(perturb.Component(1)-.5f)*.2f),dx.Component(2)*(float)(k+.5f+(perturb.Component(2)-.5f)*.2f));
                // std::cout << center << std::endl;
                rtMaterial(mat);
                int mode=rand()%2;
                switch(mode){
                case 0:rtSphere(center,.25f);break;
                case 1:rtBox(center,STVector3(0.5f));break;
                }
            }
        }
    }

    
    // //////uniform grid
    // accel_structure=UNIFORM_GRID;
    // AABB scene_bounding_box;getObjectsAABB(objects,scene_bounding_box);
    // int subdivision[3]={16,10,50};
    // uniform_grid=new UniformGrid(objects,scene_bounding_box,subdivision);

    //use acceleration structure
    ////aabb tree
    accel_structure=AABB_TREE;
    AABBTree* aabb_tree=new AABBTree(objects);
    aabb_trees.push_back(aabb_tree);
}

void RayTracingScene::addAreaLight(const STPoint3& min_corner, const STVector2& size, const STColor3f& color, const STColor3f& source_color, bool counterclockwise/*=true*/)
{
    rtAreaLight(STPoint3(min_corner.x, eb(min_corner.y), min_corner.z), STPoint3(min_corner.x+size.x, eb(min_corner.y), min_corner.z), STPoint3(min_corner.x, eb(min_corner.y), min_corner.z+size.y), color);
    Material light_source(/*ambient*/STColor3f(0.f,0.f,0.f),/*diffuse*/source_color,/*specular*/STColor3f(0.f,0.f,0.f),/*mirror*/STColor3f(0.f,0.f,0.f),/*shiness*/0.f);
    rtMaterial(light_source);
    if(counterclockwise){
        rtTriangleLight(min_corner,STPoint3(min_corner.x+size.x,min_corner.y,min_corner.z+size.y),STPoint3(min_corner.x+size.x,min_corner.y,min_corner.z));
        rtTriangleLight(min_corner,STPoint3(min_corner.x,min_corner.y,min_corner.z+size.y),STPoint3(min_corner.x+size.x,min_corner.y,min_corner.z+size.y));
    }
    else{
        rtTriangleLight(min_corner,STPoint3(min_corner.x+size.x,min_corner.y,min_corner.z),STPoint3(min_corner.x+size.x,min_corner.y,min_corner.z+size.y));
        rtTriangleLight(min_corner,STPoint3(min_corner.x+size.x,min_corner.y,min_corner.z+size.y),STPoint3(min_corner.x,min_corner.y,min_corner.z+size.y));
    }
}


void RayTracingScene::addGround(const STPoint3& min_corner,const STVector2& size,bool counterclockwise/*=true*/)
{
    if(counterclockwise){
        rtTriangle(min_corner,STPoint3(min_corner.x+size.x,min_corner.y,min_corner.z+size.y),STPoint3(min_corner.x+size.x,min_corner.y,min_corner.z));
        rtTriangle(min_corner,STPoint3(min_corner.x,min_corner.y,min_corner.z+size.y),STPoint3(min_corner.x+size.x,min_corner.y,min_corner.z+size.y));
    }
    else{
        rtTriangle(min_corner,STPoint3(min_corner.x+size.x,min_corner.y,min_corner.z),STPoint3(min_corner.x+size.x,min_corner.y,min_corner.z+size.y));
        rtTriangle(min_corner,STPoint3(min_corner.x+size.x,min_corner.y,min_corner.z+size.y),STPoint3(min_corner.x,min_corner.y,min_corner.z+size.y));
    }
}

void RayTracingScene::addBackgroundWall(const STPoint3& min_corner,const STVector2& size,bool counterclockwise/*=true*/)
{
    if(counterclockwise){
        rtTriangle(min_corner,STPoint3(min_corner.x+size.x,min_corner.y,min_corner.z),STPoint3(min_corner.x+size.x,min_corner.y+size.y,min_corner.z));
        rtTriangle(min_corner,STPoint3(min_corner.x+size.x,min_corner.y+size.y,min_corner.z),STPoint3(min_corner.x,min_corner.y+size.y,min_corner.z));
    }
    else{
        rtTriangle(min_corner,STPoint3(min_corner.x+size.x,min_corner.y+size.y,min_corner.z),STPoint3(min_corner.x+size.x,min_corner.y,min_corner.z));
        rtTriangle(min_corner,STPoint3(min_corner.x,min_corner.y+size.y,min_corner.z),STPoint3(min_corner.x+size.x,min_corner.y+size.y,min_corner.z));
    }
}

void RayTracingScene::addWall(const STPoint3& min_corner,const STVector3& u,const STVector3& v,bool counterclockwise/*=true*/)
{
    if(counterclockwise){
        rtTriangle(min_corner,min_corner+u,min_corner+u+v);
        rtTriangle(min_corner,min_corner+u+v,min_corner+v);
    }
    else{
        rtTriangle(min_corner,min_corner+u+v,min_corner+u);
        rtTriangle(min_corner,min_corner+v,min_corner+u+v);
    }
}