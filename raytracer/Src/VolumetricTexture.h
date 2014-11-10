#ifndef VolumetricTexture_h
#define VolumetricTexture_h

#include <iostream>

#include "st.h"

class VolumetricTexture
{
public:
    VolumetricTexture(/*const int _counts[3], */const std::string& file_name)
        : min_corner(0.f,0.f,0.f), max_corner(1.f,1.f,1.f)
    {
        parseFile(file_name);
    }
    
    float Value(const STPoint3& uvw)
    {
        int idx[3];float frac[3];getCellWithFrac(uvw,idx,frac);
        for(int d=0;d<3;d++)if(idx[d]<0 || idx[d]>=counts[d])std::cout<<"hey!"<<std::endl;

        return (1.f-frac[0])*(1.f-frac[1])*(1.f-frac[2])*values[getIndex(idx[0]  ,idx[1]  ,idx[2])]+
               (    frac[0])*(1.f-frac[1])*(1.f-frac[2])*values[getIndex(idx[0]+1,idx[1]  ,idx[2])]+
               (1.f-frac[0])*(    frac[1])*(1.f-frac[2])*values[getIndex(idx[0]  ,idx[1]+1,idx[2])]+
               (1.f-frac[0])*(1.f-frac[1])*(    frac[2])*values[getIndex(idx[0]  ,idx[1]  ,idx[2]+1)]+
               (    frac[0])*(    frac[1])*(1.f-frac[2])*values[getIndex(idx[0]+1,idx[1]+1,idx[2])]+
               (1.f-frac[0])*(    frac[1])*(    frac[2])*values[getIndex(idx[0]  ,idx[1]+1,idx[2]+1)]+
               (    frac[0])*(1.f-frac[1])*(    frac[2])*values[getIndex(idx[0]+1,idx[1]  ,idx[2]+1)]+
               (    frac[0])*(    frac[1])*(    frac[2])*values[getIndex(idx[0]+1,idx[1]+1,idx[2]+1)];
    }

private:
    std::vector<float> values;
    int counts[3];
    const STPoint3 min_corner;
    const STPoint3 max_corner;
    float dx[3];
    float one_over_dx[3];

    void getCell(const STPoint3& p, /*result*/int idx[3])
    {
        for(int d=0;d<3;d++){
            idx[d]=(int)floor((p.Component(d)-min_corner.Component(d))*one_over_dx[d]);
        }
    }

    void getCellWithFrac(const STPoint3& p, /*result*/int idx[3], /*result*/float frac[3])
    {
        for(int d=0;d<3;d++){
            float s=(p.Component(d)-min_corner.Component(d))*one_over_dx[d];
            idx[d]=(int)floor(s);
            if(idx[d]>=counts[d]-1)idx[d]=counts[d]-2;//sort of hacky, but keeps idx from being out of bounds in Value function
            frac[d]=s-(float)idx[d];
            if(frac[d]>1.f)frac[d]=1.f;
        }
    }

    bool isValidCell(const int idx[3])
    {
        return idx[0]>=0 && idx[0]<counts[0] && idx[1]>=0 && idx[1]<counts[1] && idx[2]>=0 && idx[2]<counts[2];
    }

    bool isValidCell(const int i,const int j,const int k)
    {
        return i>=0 && i<counts[0] && j>=0 && j<counts[1] && k>=0 && k<counts[2];
    }

    int getIndex(const int idx[3])
    {
        return getIndex(idx[0],idx[1],idx[2]);	
    }

    int getIndex(const int i,const int j,const int k)
    {
        return k*counts[0]*counts[1]+j*counts[0]+i;
    }

    void updateGrid(const int _counts[3])
    {
        for(int d=0;d<3;d++){
            counts[d]=_counts[d];
            dx[d]=(max_corner.Component(d)-min_corner.Component(d))/(float)counts[d];
            one_over_dx[d]=1.f/dx[d];
        }

        values.resize(counts[0]*counts[1]*counts[2],0.f);
    }

    void parseFile(const std::string& file_name);
};

#endif
