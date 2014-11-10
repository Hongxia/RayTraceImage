//#####################################################################
// Stanford CS148 Ray Tracer Example Code
// Copyright 2012, Ben Mildenhall
//#####################################################################

#ifndef RayTracer_ImagePlane_h
#define RayTracer_ImagePlane_h

#include "Camera.h"
#include "Jitter.h"
#include <iostream>

class ImagePlane {
public:
    ImagePlane(int w, int h): width(w), height(h) {}
    ~ImagePlane(){}

    std::vector<Ray *>* getRays(Camera* camera, int x, int y, int sampling = 1, float focus = 0., float aperture = 0.) {
        std::vector<Ray *> * rays = new std::vector<Ray *>();
		bool use_jitter=(sampling>1);
        Jitter jitter = Jitter(sampling);
        if (focus > 0.) {
            for (int i = 0; i < sampling; i++) {
                for (int j = 0; j < sampling; j++) {
                    float u = ((float)x + .5f) / width;
                    float v = ((float)y + .5f) / height;
                    Ray *camRay = camera->generateRay(u, v);
                    STPoint3 focal = camRay->at(focus);
					float jitter_u=use_jitter?jitter.get():0;float jitter_v=use_jitter?jitter.get():0;
					float apertureOnPlane = focus >=1.f ? (focus-1.f)/focus * aperture : (1.f-focus)/focus * aperture;
                    float du = (-apertureOnPlane / 2 + apertureOnPlane * ((float)(i) + jitter_u) / sampling) / width;
                    float dv = (-apertureOnPlane / 2 + apertureOnPlane * ((float)(j) + jitter_v) / sampling) / height;
                    STPoint3 onPlane = camera->pointOnPlane(u + du, v + dv);
                    if (focus >= 1)
                        rays->push_back(new Ray(onPlane, focal - onPlane, -10000.f));
                    else rays->push_back(new Ray(focal, onPlane - focal, -2.5f));
                    delete camRay;
                }
            }
        } else {
            for (int i = 0; i < sampling; i++) {
                for (int j = 0; j < sampling; j++) {
					float jitter_u=use_jitter?jitter.get():0;float jitter_v=use_jitter?jitter.get():0;
                    rays->push_back(camera->generateRay(((float)x + ((float)(i) + jitter_u) / sampling) / width,
                                                        ((float)y + ((float)(j) + jitter_v) / sampling) / height));
                }
            }
        }
        return rays;
    }
private:
    int width, height;
};


#endif
