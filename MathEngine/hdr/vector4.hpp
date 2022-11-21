#ifndef __VECTOR_4_HPP__
#define __VECTOR_4_HPP__

namespace MathEngine{

class Vector4{

    private:
        float mVect[4];

    public: // CONSTRUCTORS
        Vector4(float x=0.0f, float y=0.0f, float z=0.0f, float w=0.0f){
            mVect[0] = x;
            mVect[1] = y;
            mVect[2] = z;
            mVect[3] = w;
        };

        Vector4(Vector4 const &v){
            mVect[0] = v.x();
            mVect[1] = v.y();
            mVect[2] = v.z();
            mVect[3] = v.w();
        };

        ~Vector4(){};

    public: // GETTERS
        float* getVect(){return mVect;};
        float x() const {return mVect[0];};
        float y() const {return mVect[1];};
        float z() const {return mVect[2];};
        float w() const {return mVect[3];};

};

};

#endif