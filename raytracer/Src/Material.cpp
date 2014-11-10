//#####################################################################
// Stanford CS148 Ray Tracer Example Code
// Copyright 2012, Ben Mildenhall
//#####################################################################

#include "Material.h"

STColor3f Material::shade(Intersection *inter, STVector3 view, std::vector<Light *>& lights, const STColor3f& mirrorCol, const STColor3f& refractCol) {
    STColor3f result = mirrorCol * mirror + refractCol * refract;
    for (int i = 0; i < (int)lights.size(); i++) result += lights[i]->color(inter, view, ambient, diffuse, specular, shininess);
    return result;
}

STColor3f Material::shade(Intersection* inter, STVector3 view, std::vector<Light *>& lights, std::vector<STColor3f>& attenuation, const STColor3f& mirrorCol /*= STColor3f()*/, const STColor3f& refractCol /*= STColor3f()*/)
{
	STColor3f result = mirrorCol * mirror + refractCol * refract;
	for (int i = 0; i < (int)lights.size(); i++){
		//std::cout<<"atten: "<<i<<": "<<attenuation[i].r<<", "<<attenuation[i].g<<", "<<attenuation[i].b<<std::endl;
		result += lights[i]->color(inter, view, ambient, diffuse, specular, shininess) * attenuation[i];
	}
	return result;
}

Ray *Material::refracted(STVector3 d, Intersection *inter, float bias) {
    d.Normalize();
    float cos_theta = STVector3::Dot(inter->normal, d);
    if (snell) {
        float sin_theta = (cos_theta > 0.f ? snell : 1.f / snell) * sqrt(1 - cos_theta * cos_theta);
        if (sin_theta >= 1) return NULL;
        STVector3 perp = d - STVector3::Dot(inter->normal, d) * inter->normal;
        perp.Normalize();
        return new Ray(inter->point, perp * sin_theta + (cos_theta > 0.f ? 1.f : -1.f) * inter->normal * sqrt(1.f - sin_theta * sin_theta), bias);
    }
    return NULL;
}

Ray *Material::refracted(Ray into, Intersection *inter, float bias) {
    return refracted(into.d, inter, bias);
}

float Material::participatingMediaAttenuation(const STPoint3& p1, const STPoint3& p2, const AABB& aabb)
{
    float step=0.05;//TODO: adjust this parameter for your scene
    float coef_density_to_alpha = .05f;//TODO: ajust this parameter for your scene
    STVector3 direction=(p2-p1);
    direction.Normalize();
    STVector3 range(aabb.xmax - aabb.xmin, aabb.ymax - aabb.ymin, aabb.zmax - aabb.zmin);
    STPoint3 aabb_min(aabb.xmin, aabb.ymin, aabb.zmin);
    int n=(int)((p2-p1).Length()/step);
    float alpha = 0.f;
    for(int i=0; i<n; i++) {
        STPoint3 pos=p1 + i*step*direction;
        STPoint3 normalized_pos = STPoint3((pos.x-aabb_min.x)/range.x, (pos.y-aabb_min.y)/range.y, (pos.z-aabb_min.z)/range.z);
        for(int d=0;d<3;d++){if(normalized_pos.Component(d)<0)normalized_pos.Component(d)=0.f;if(normalized_pos.Component(d)>1)normalized_pos.Component(d)=1.f-(1e-5);}
        float density = volumetric_texture->Value(normalized_pos);
        float sampled_alpha = 1.f - exp(-coef_density_to_alpha * density);

        alpha=(1.f - sampled_alpha)*alpha + sampled_alpha;
    }

    return 1-alpha;
}
