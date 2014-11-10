//#####################################################################
// Stanford CS148 Ray Tracer Example Code
// Copyright 2012, Ben Mildenhall
//#####################################################################

#ifndef RayTracer_AreaLight_h
#define RayTracer_AreaLight_h

#include "Light.h"
#include "Jitter.h"

class AreaLight : public Light {
public:
    AreaLight(const STPoint3& _v1, const STPoint3& _v2, const STPoint3& _v3, const STColor3f& _col, const int sampleRate = 1)
        : v1(_v1), v2(_v2), v3(_v3), col(_col) { jitter = new Jitter(); name = "area"; index = 0; setSampleRate(sampleRate); }
    ~AreaLight() { if (jitter) delete jitter; if(pretendLight) delete pretendLight;}
    
    STColor3f color(Intersection *inter, STVector3 view, const STColor3f& amb, const STColor3f& diff, const STColor3f& spec, float shine) {
        return pretendLight->color(inter, view, amb, diff, spec, shine);
    }
    
    STVector3 direction(const STPoint3& pt) {
        getNext();
        return pretendLight->direction(pt);
    }
    
    void setSampleRate(int s) {
        sample = s;
        jitter->setSize(sample);
        refresh();
        pretendLight = new PointLight(v1 + ((float)(order[index] / sample) + jitter->get()) / sample * (v2 - v1) + ((float)(order[index] % sample) + jitter->get()) / sample * (v3 - v1), col);
    }

private:
    int index;
    int sample;
    STColor3f col;
    STPoint3 v1, v2, v3;
    PointLight *pretendLight;
    Jitter *jitter;
    std::vector<int> order;

    void getNext() {
        if (index == sample * sample) refresh();
        pretendLight->point = v1 + ((float)(order[index] / sample) + jitter->get()) / sample * (v2 - v1) + ((float)(order[index] % sample) + jitter->get()) / sample * (v3 - v1);
        index++;
    }
    void refresh() {
        order.clear();
        for (int i = 0; i < sample * sample; i++) order.push_back(i);
        for (int i = 0; i < sample * sample; i++) {
            int j = rand() % (sample * sample - i);
            int temp = order[i + j];
            order[i + j] = order[i];
            order[i] = temp;
        }
        jitter->refresh();
        index = 0; 
    }
};

#endif
