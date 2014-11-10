//#####################################################################
// Stanford CS148 Ray Tracer Example Code
// Copyright 2012, Ben Mildenhall
//#####################################################################

#ifndef RayTracer_Material_h
#define RayTracer_Material_h

#include "AABB.h"
#include "STColor3f.h"
#include "STVector3.h"
#include "Light.h"
#include "Intersection.h"
#include "st.h"
#include "Ray.h"
#include "VolumetricTexture.h"

class Material {
public:
    STColor3f ambient, diffuse, specular, mirror, refract;
    float shininess, snell;
    VolumetricTexture* volumetric_texture;

    Material(const STColor3f& amb = STColor3f(0,0,0), const STColor3f& diff = STColor3f(0,0,0), const STColor3f& spec = STColor3f(0,0,0), const STColor3f& mirr = STColor3f(0,0,0), float shine = 0.f, const STColor3f& refr = STColor3f(0,0,0), float sn = 0., VolumetricTexture* vt = 0)
    : ambient(amb), diffuse(diff), specular(spec), mirror(mirr), shininess(shine), refract(refr), snell(sn), volumetric_texture(vt) {}
    //~Material() {if(volumetric_texture!=0)delete volumetric_texture;}
    STColor3f shade(Intersection* inter, STVector3 view, std::vector<Light *>& lights, const STColor3f& mirrorCol = STColor3f(), const STColor3f& refractCol = STColor3f());
	STColor3f shade(Intersection* inter, STVector3 view, std::vector<Light *>& lights, std::vector<STColor3f>& attenuation, const STColor3f& mirrorCol = STColor3f(), const STColor3f& refractCol = STColor3f());
	Ray *refracted(Ray into, Intersection *inter, float bias = 0.);
    Ray *refracted(STVector3 d, Intersection *inter, float bias = 0.);
    bool isOpaque() { return !(refract.r || refract.g || refract.b); }
    bool isMatte() { return !(mirror.r || mirror.g || mirror.b); }
    bool isParticipatingMedia() { return volumetric_texture!=0; }

    float participatingMediaAttenuation(const STPoint3& p1, const STPoint3& p2, const AABB& aabb);
};

#endif
