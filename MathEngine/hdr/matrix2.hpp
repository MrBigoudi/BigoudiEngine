#ifndef __MATRIX_2_HPP__
#define __MATRIX_2_HPP__

#include <cmath>

namespace MathEngine {

class Matrix2{

    private:
        float mVect[4];

    public:
        Matrix2(float m00=0.0f, float m01=0.0f, float m10=0.0f, float m11=0.0f){
            mVect[0]=m00;
            mVect[1]=m01;
            mVect[2]=m10;
            mVect[3]=m11;
        };

        Matrix2(Matrix2 const &m){
            for(int i=0; i<4; i++){
                mVect[i]=m.mVect[i];
            }
        };

        ~Matrix2(){};

    public:
        static Matrix2 identity(){
            Matrix2 newMat;
            newMat.mVect[0]=1.0f;
            newMat.mVect[3]=1.0f;
            return newMat;
        };

        static Matrix2 rotation(float theta){
            Matrix2 newMat;
            newMat.mVect[0] =  cos(theta);
            newMat.mVect[1] = -sin(theta);
            newMat.mVect[2] =  sin(theta);
            newMat.mVect[3] =  cos(theta);
            return newMat;
        };

};

};

#endif