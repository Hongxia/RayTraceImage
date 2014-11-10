//#####################################################################
// Stanford CS148 Ray Tracer Example Code
// Copyright 2012, Ben Mildenhall
//#####################################################################	
#ifndef RayTracer_Jitter_h
#define RayTracer_Jitter_h

class Jitter {
public:
    Jitter(int s = 1): size(s) { refresh(); index = 0; }
    ~Jitter(){}

    void setSize(int s) { size = s; }
    void refresh() {
        index = 0;
        rands.clear();
        for (int i = 0; i < size * size * 2; i++) rands.push_back((float)rand() / RAND_MAX);
    }
    float get(int x, int y, bool which) { return rands[y * size * 2 + x * 2 + which]; }
    float get(int r, bool which) { return rands[r * 2 + which]; }
    float get() {
        if (index >= size * size * 2) refresh();
        float result = rands[index];
        index++;
        return result;
    }
private:
    int index;
    int size;
    std::vector<float> rands;
};

#endif
