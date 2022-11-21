#ifndef __TRIANGLE_HPP__
#define __TRIANGLE_HPP__

#include "opengl.hpp"

#include "mathEngine.hpp"
#include "vector3.hpp"

using namespace MathEngine;

namespace RenderEngine{

class Triangle{

    private:
        Vector3f mPoints[3];

    public: // CONSTRUCTORS
        Triangle(float x0, float y0, float z0,
                 float x1, float y1, float z1,
                 float x2, float y2, float z2){

            mPoints[0] = Vector3f(x0,y0,z0);
            mPoints[1] = Vector3f(x1,y1,z1);
            mPoints[2] = Vector3f(x2,y2,z2);
        };

        Triangle(Triangle &t){
            mPoints[0] = Vector3f(t.mPoints[0]);
            mPoints[1] = Vector3f(t.mPoints[1]);
            mPoints[2] = Vector3f(t.mPoints[2]);
        };

    public: // GETTERS
        Vector3f p1() { return mPoints[0];};
        Vector3f p2() { return mPoints[1];};
        Vector3f p3() { return mPoints[2];};

};

};


#endif