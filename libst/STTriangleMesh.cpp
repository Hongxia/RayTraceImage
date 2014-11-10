// STTriangleMesh.cpp
#include "STTriangleMesh.h"

/* Include-order dependency!
*
* GLEW must be included before the standard GL.h header.
* In this case, it means we must violate the usual design
* principle of always including Foo.h first in Foo.cpp.
*/
#ifdef __APPLE__
#define GLEW_VERSION_2_0 1
#include <OpenGL/gl.h>
#else
#define GLEW_STATIC
#include "GL/glew.h"
#include "GL/gl.h"
#endif

#include "STImage.h"
#include <iostream>
#include <fstream>
#include <map>
#include <math.h>
#include <string.h>
#include <algorithm>
#define PI 3.14159265

#include "tiny_obj_loader.h"
using namespace tinyobj;

const float STTriangleMesh::red[]  ={1.0f,0.0f,0.0f,1.0f};
const float STTriangleMesh::green[]={0.0f,1.0f,0.0f,1.0f};
const float STTriangleMesh::blue[] ={0.0f,0.0f,1.0f,1.0f};
const float STTriangleMesh::black[]={0.0f,0.0f,0.0f,1.0f};
const float STTriangleMesh::white[]={1.0f,1.0f,1.0f,1.0f};
int STTriangleMesh::instance_count=0;
STImage STTriangleMesh::whiteImg=STImage(256, 256, STImage::Pixel(255,255,255,255));
//STTexture* STTriangleMesh::whiteTex = 0;
//
// Initialization
//
STTriangleMesh::STTriangleMesh()
{
    for(int i=0;i<3;i++){
        mMaterialAmbient[i]=0.2f;
        mMaterialDiffuse[i]=0.8f;
        mMaterialSpecular[i]=0.1f;
    }
    mMaterialAmbient[3]=1.0f;
    mMaterialDiffuse[3]=1.0f;
    mMaterialSpecular[3]=1.0f;
    mShininess = 1.;  // # between 1 and 128.

    mDrawAxis = true;

    mSurfaceColorImg=0;
}

//
// Delete and clean up an existing image.
//
STTriangleMesh::~STTriangleMesh()
{
    for(unsigned int i=0;i<mVertices.size();i++)delete mVertices[i];
    for(unsigned int i=0;i<mTexPos.size();i++)delete mTexPos[i];
    for(unsigned int i=0;i<mNormals.size();i++)delete mNormals[i];
    for(unsigned int i=0;i<mFaces.size();i++)delete mFaces[i];
	if(mSurfaceColorImg)delete mSurfaceColorImg;
}

//
// Build topology  and calculate normals for the triangle mesh.
//
bool STTriangleMesh::Build()
{
    UpdateGeometry();
    return true;
}

bool STTriangleMesh::UpdateGeometry()
{
    mMassCenter = STPoint3(0.0f,0.0f,0.0f);
    mSurfaceArea = 0.0f;
    if(mVertices.size()>0){
        mBoundingBoxMin=mBoundingBoxMax=mVertices[0]->pt;
        for(unsigned int i=1;i<mVertices.size();i++){
            mBoundingBoxMin=STPoint3::Min(mBoundingBoxMin,mVertices[i]->pt);
            mBoundingBoxMax=STPoint3::Max(mBoundingBoxMax,mVertices[i]->pt);
        }
    }
    else{
        mBoundingBoxMin=STPoint3(0.0f,0.0f,0.0f);
        mBoundingBoxMax=STPoint3(1.0f,1.0f,1.0f);
    }
    for(unsigned int i=0;i<mFaces.size();i++){
        STFace* face=mFaces[i];
        face->normal=STVector3::Cross(face->v[0]->pt-face->v[1]->pt,face->v[0]->pt-face->v[2]->pt);
        float area=face->normal.Length();
        mSurfaceArea+=area;
        mMassCenter=mMassCenter+(face->v[0]->pt+face->v[1]->pt+face->v[2]->pt)*(area/3.0f);
    }
    mMassCenter=mMassCenter/mSurfaceArea;
    for(unsigned int i=0;i<mFaces.size();i++){
        mFaces[i]->normal.Normalize();
    }
    return true;
}

bool STTriangleMesh::CalculateTextureCoordinatesViaSphericalProxy()
{
	for(unsigned int i=0;i<mFaces.size();i++){
		STFace* face=mFaces[i];		
		for(int v=0;v<3;v++){
			STPoint3 point=face->v[v]->pt;
			float r=sqrt(pow(point.x,2)+pow(point.y,2)+pow(point.z,2));
			float theta=(float)PI-acos(point.z/r);
			float phi=atan2(point.y,point.x)+(float)PI;
			face->texPos[v]->x=phi/(float)2/(float)PI;
			face->texPos[v]->y=theta/(float)PI;
		}
		if(face->texPos[1]->x-face->texPos[0]->x>.5) face->texPos[1]->x-=1;
		else if(face->texPos[1]->x-face->texPos[0]->x<-.5) face->texPos[1]->x+=1;

		if(face->texPos[2]->x-face->texPos[0]->x>.5) face->texPos[2]->x-=1;
		else if(face->texPos[2]->x-face->texPos[0]->x<-.5) face->texPos[2]->x+=1;
	}
	return true;
}

bool STTriangleMesh::CalculateTextureCoordinatesViaCylindricalProxy(float h_min,float h_max,float center_x,float center_y,int axis_direction)
{
	for(unsigned int i=0;i<mFaces.size();i++){
		STFace* face=mFaces[i];		
		for(int v=0;v<3;v++){
			STPoint3 point=face->v[v]->pt;
			float phi;
			switch(axis_direction){
			case 1:
				phi=atan2(point.z-center_y,point.y-center_x)+(float)PI;
				face->texPos[v]->x=phi/(float)2/(float)PI;
				face->texPos[v]->y=(point.x-h_min)/(h_max-h_min);
				break;
			case 2:
				phi=atan2(point.x-center_y,point.z-center_x)+(float)PI;
				face->texPos[v]->x=phi/(float)2/(float)PI;
				face->texPos[v]->y=(point.y-h_min)/(h_max-h_min);
				break;
			case 3:
				phi=atan2(point.y-center_y,point.x-center_x)+(float)PI;
				face->texPos[v]->x=phi/(float)2/(float)PI;
				face->texPos[v]->y=(point.z-h_min)/(h_max-h_min);
				break;
			default:
				break;
			}
		}
		if(face->texPos[1]->x-face->texPos[0]->x>.5) face->texPos[1]->x-=1;
		else if(face->texPos[1]->x-face->texPos[0]->x<-.5) face->texPos[1]->x+=1;

		if(face->texPos[2]->x-face->texPos[0]->x>.5) face->texPos[2]->x-=1;
		else if(face->texPos[2]->x-face->texPos[0]->x<-.5) face->texPos[2]->x+=1;
	}
	return true;
}

std::ostream& operator <<(std::ostream& stream, const STVertex& v) {
    stream<<"["<<v.pt.x<<" "<<v.pt.y<<" "<<v.pt.z<<"]";
    return stream;
}

std::ostream& operator <<(std::ostream& stream, const STFace& f) {
    stream<<"{"<<*(f.v[0])<<" "<<*(f.v[1])<<" "<<*(f.v[2])<<"}";
    return stream;
}

std::string STTriangleMesh::LoadObj(std::vector<STTriangleMesh*>& output_meshes, const std::string& filename){
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
	std::string base;
	size_t l;
	if ((l = filename.find_last_of('/')) != std::string::npos)
		base = filename.substr(0, l+1);
	else if ((l = filename.find_last_of('\\')) != std::string::npos)
		base = filename.substr(0, l+1);
	std::string err = tinyobj::LoadObj(shapes, materials, filename.c_str(), base.c_str());
    
    std::cout<<"#shapes="<<shapes.size()<<" #materials="<<materials.size()<<std::endl;
    

    for(unsigned int mesh_id=0; mesh_id<shapes.size(); mesh_id++)
    {
        tinyobj::mesh_t& mesh=shapes[mesh_id].mesh;
        STTriangleMesh* stmesh = new STTriangleMesh();
        for(unsigned int vertex_id=0; vertex_id<mesh.positions.size()/3; vertex_id++)
            stmesh->mVertices.push_back(new STVertex(mesh.positions[vertex_id*3],
                                                     mesh.positions[vertex_id*3+1],
                                                     mesh.positions[vertex_id*3+2]));
        for(unsigned int face_id=0; face_id<mesh.indices.size()/3; face_id++){
            int indices[3];for(int j=0;j<3;j++)indices[j]=mesh.indices[face_id*3+j];
            stmesh->mFaces.push_back(new STFace(stmesh->mVertices[indices[0]],
                                                stmesh->mVertices[indices[1]],
                                                stmesh->mVertices[indices[2]]));
        }
        if(mesh.normals.size()>0){
            for(unsigned int normal_id=0; normal_id<mesh.normals.size()/3; normal_id++)
                stmesh->mNormals.push_back(new STVector3(mesh.normals[normal_id*3],
                                                         mesh.normals[normal_id*3+1],
                                                         mesh.normals[normal_id*3+2]));
            for(unsigned int face_id=0; face_id<mesh.indices.size()/3; face_id++){
                for(int j=0;j<3;j++)
                    stmesh->mFaces[face_id]->normals[j]=stmesh->mNormals[mesh.indices[face_id*3+j]];
            }
        }
        if(mesh.texcoords.size()>0){
            for(unsigned int texpos_id=0; texpos_id<mesh.texcoords.size()/2; texpos_id++)
                stmesh->mTexPos.push_back(new STPoint2(mesh.texcoords[texpos_id*2],
                                                         mesh.texcoords[texpos_id*2+1]));
            for(unsigned int face_id=0; face_id<mesh.indices.size()/3; face_id++){
                for(int j=0;j<3;j++)
                    stmesh->mFaces[face_id]->texPos[j]=stmesh->mTexPos[mesh.indices[face_id*3+j]];
            }
        }
        stmesh->Build();
		if(/*mesh.material_ids.size()>0&&*/mesh.material_ids[0]>=0){
            tinyobj::material_t& material=materials[mesh.material_ids[0]];
            for(int i=0;i<3;i++){
                stmesh->mMaterialAmbient[i]=material.ambient[i];
                stmesh->mMaterialDiffuse[i]=material.diffuse[i];
                stmesh->mMaterialSpecular[i]=material.specular[i];
            }
			std::string colorMap = material.diffuse_texname;
			if (colorMap != "") {
                std::cout<<"read texture image: "<<base+colorMap<<std::endl;
				stmesh->mSurfaceColorImg = new STImage(base+colorMap);
			}
  		
            stmesh->mShininess = 8.;  // # between 1 and 128.
        }

        output_meshes.push_back(stmesh);
    }

    return err;
}

STPoint3 STTriangleMesh::GetMassCenter(const std::vector<STTriangleMesh*>& input_meshes)
{
    STPoint3 massCenter=STPoint3(0.0,0.0,0.0);
    float totalArea=0.0f;
    for(unsigned int i = 0; i<input_meshes.size(); i++){
        massCenter = massCenter + input_meshes[i]->mMassCenter * input_meshes[i]->mSurfaceArea;
        totalArea+=input_meshes[i]->mSurfaceArea;
    }
    return massCenter/totalArea;
}

std::pair<STPoint3,STPoint3> STTriangleMesh::GetBoundingBox(const std::vector<STTriangleMesh*>& input_meshes)
{
    if(input_meshes.size()>0){
        STPoint3 boundingMin=input_meshes[0]->mBoundingBoxMin;
        STPoint3 boundingMax=input_meshes[0]->mBoundingBoxMax;
        for(unsigned int i = 0; i<input_meshes.size(); i++){
            boundingMin = STPoint3::Min(boundingMin,input_meshes[i]->mBoundingBoxMin);
            boundingMax = STPoint3::Max(boundingMin,input_meshes[i]->mBoundingBoxMax);
        }
        return std::pair<STPoint3,STPoint3>(boundingMin,boundingMax);
    }
    else
        return std::pair<STPoint3,STPoint3>(STPoint3(0.0,0.0,0.0),STPoint3(1.0,1.0,1.0));
}

void STTriangleMesh::Recenter(const STPoint3& center)
{
    STVector3 translate = STPoint3::Origin - center;
    for(unsigned int i=0;i<mVertices.size();i++){
        mVertices[i]->pt+=translate;
    }
    mMassCenter+=translate;
    mBoundingBoxMax+=translate;
    mBoundingBoxMin+=translate;
}
