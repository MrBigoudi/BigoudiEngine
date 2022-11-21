#pragma once

#include "opengl.hpp"
#include "mathEngine.hpp"

#include "triangle.hpp"

#include <vector>

namespace RenderEngine{

class Mesh{

    private:
        std::vector<Triangle*> mTris;

    public:
        Mesh(){};

        Mesh(Mesh &m){
            for(int i=0 ; i<(int)m.mTris.size(); i++){
                mTris.push_back(new Triangle(*m.mTris[i]));
            }
        };

};

};