#include <stdlib.h>
#include <fstream>
#include <iostream>

#include "VolumetricTexture.h"

void VolumetricTexture::parseFile(const std::string& file_name)
{
    std::ifstream fin(file_name.c_str());
    if(fin.fail()){
        std::cout<<"error--failed to open file: "<<file_name<<std::endl;
        exit(1);
    }

    fin>>counts[0]>>counts[1]>>counts[2];

    updateGrid(counts);

    for(int z=0; z<counts[2]; z++){
        for(int y=0; y<counts[1]; y++){
            for(int x=0; x<counts[0]; x++){
                fin>>values[getIndex(x,y,z)];
                if(fin.fail()){
                    std::cout<<"error encountered while reading from file: "<<file_name<<std::endl;
                    exit(1);
                }
            }
        }
    }

    fin.close();
}
