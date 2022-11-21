#ifndef __MATRIX_HPP__
#define __MATRIX_HPP__

#include <cmath>

namespace MathEngine{

class Matrix3{

    private:
        float mVect[9];

    public:
        Matrix3(float m00=0.0f, float m01=0.0f, float m02=0.0f,
                float m10=0.0f, float m11=0.0f, float m12=0.0f,
                float m20=0.0f, float m21=0.0f, float m22=0.0f)
        {
            mVect[0]=m00;
            mVect[1]=m01;
            mVect[2]=m02;
            mVect[3]=m10;
            mVect[4]=m11;
            mVect[5]=m12;
            mVect[6]=m20;
            mVect[7]=m21;
            mVect[8]=m22;
        };

        Matrix3(Matrix3 const &m){
            for(int i=0; i<9; i++){
                mVect[i]=m.mVect[i];
            }
        };

        ~Matrix3(){};

    public:
        static Matrix3 identity(){
            Matrix3 newMat;
            newMat.mVect[0]=1.0f;
            newMat.mVect[4]=1.0f;
            newMat.mVect[8]=1.0f;

            return newMat;
        };

        static Matrix3 rotationX(float theta){
            Matrix3 newMat;
            newMat.mVect[0] =  1.0f;
            newMat.mVect[4] =  cos(theta);
            newMat.mVect[5] = -sin(theta);
            newMat.mVect[7] =  sin(theta);
            newMat.mVect[8] =  cos(theta);
            return newMat;
        };

        static Matrix3 rotationY(float theta){
            Matrix3 newMat;
            newMat.mVect[0] =  cos(theta);
            newMat.mVect[2] =  sin(theta);
            newMat.mVect[4] =  1.0f;
            newMat.mVect[6] = -sin(theta);
            newMat.mVect[8] =  cos(theta);
            return newMat;
        };

        static Matrix3 rotationZ(float theta){
            Matrix3 newMat;
            newMat.mVect[0] =  cos(theta);
            newMat.mVect[1] = -sin(theta);
            newMat.mVect[3] =  sin(theta);
            newMat.mVect[4] =  cos(theta);
            newMat.mVect[8] =  1.0f;
            return newMat;
        };

};

};

#endif