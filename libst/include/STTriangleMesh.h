// STTriangleMesh.h
#ifndef __STTRIANGLEMESH_H__
#define __STTRIANGLEMESH_H__

#include "STPoint2.h"
#include "STPoint3.h"

#include <string>
#include <vector>
#include <iostream>

struct STFace;

struct STVertex{
    STVertex(float x, float y, float z, float u=0, float v=0){
        pt=STPoint3(x,y,z);
    }
    STPoint3 pt;
};
inline std::ostream& operator <<(std::ostream& stream, const STVertex& v);

struct STFace{
    STFace(STVertex* v0, STVertex* v1, STVertex* v2){
        v[0]=v0;v[1]=v1;v[2]=v2;
        normals[0]=0;normals[1]=0;normals[2]=0;
        texPos[0]=0;texPos[1]=0;texPos[2]=0;
    }
    STVertex *v[3];
    //STFace *adjF[3];
    STVector3 normal;
    STVector3 *normals[3];
    STPoint2 *texPos[3];
};
inline std::ostream& operator <<(std::ostream& stream, const STFace& f);

/**
* STTriangleMesh use a simple data structure to represent a triangle mesh.
*/
class STTriangleMesh
{
public:
    //
    // Initialization
    //
    STTriangleMesh();

    //
    // Delete and clean up.
    //
    ~STTriangleMesh();

    //
    // Build topology and calculate normals for the triangle mesh.
    //
    bool Build();
    bool UpdateGeometry();
	bool CalculateTextureCoordinatesViaSphericalProxy();
	bool CalculateTextureCoordinatesViaCylindricalProxy(float h_min,float h_max,float center_x,float center_y,int axis_direction);

    //
    // Local members
    //
    std::vector<STVertex*> mVertices;
    std::vector<STVector3*> mNormals;
    std::vector<STPoint2*> mTexPos;
    std::vector<STFace*> mFaces;
    
    static std::string LoadObj(std::vector<STTriangleMesh*>& output_meshes, const std::string& filename);
    
    float mMaterialAmbient[4];
    float mMaterialDiffuse[4];
    float mMaterialSpecular[4];
    float mShininess;  // # between 1 and 128.
	STImage * mSurfaceColorImg;
	//STTexture * mSurfaceColorTex;

    static STPoint3 GetMassCenter(const std::vector<STTriangleMesh*>& input_meshes);
    static std::pair<STPoint3,STPoint3> GetBoundingBox(const std::vector<STTriangleMesh*>& input_meshes);
    void Recenter(const STPoint3& center);
    float mSurfaceArea;
    STPoint3 mMassCenter;
    STPoint3 mBoundingBoxMax;
    STPoint3 mBoundingBoxMin;

    bool mDrawAxis;
    const static float red[];
    const static float green[];
    const static float blue[];
    const static float black[];
    const static float white[];
    static int instance_count;
	static STImage whiteImg;
	//static STTexture* whiteTex;
};

#endif  // __STTRIANGLEMESH_H__

