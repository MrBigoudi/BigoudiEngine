#ifndef __VECTOR_3_HPP__
#define __VECTOR_3_HPP__

namespace MathEngine{

class Vector3f{

    private:
        float mVect[3];

    public: // CONSTRUCTORS
        Vector3f(float x=0.0f, float y=0.0f, float z=0.0f){
            mVect[0] = x;
            mVect[1] = y;
            mVect[2] = z;
        }

        Vector3f(Vector3f& v){
            mVect[0] = v.x();
            mVect[1] = v.y();
            mVect[2] = v.z();
        }

        ~Vector3f(){}


    public: // GETTERS
        float* getVect(){return mVect;};
        float x() const {return mVect[0];};
        float y() const {return mVect[1];};
        float z() const {return mVect[2];};


    public: // VECTORS OPERATIONS
        Vector3f operator + (Vector3f const &v){
            return * new Vector3f(x() + v.x(), y() + v.y(), z() + v.z());
        };

        Vector3f operator - (Vector3f const &v){
            return * new Vector3f(x() - v.x(), y() - v.y(), z() - v.z());
        };

        Vector3f operator * (float f){
            return * new Vector3f(x()*f, y()*f, z()*f);
        };

        Vector3f operator / (float f){
            return * new Vector3f(x()/f, y()/f, z()/f);
        };

        static float dot(Vector3f const &v1, Vector3f const &v2){
            return v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z();
        }

        static Vector3f cross(Vector3f const &v1, Vector3f const &v2){
            return * new Vector3f(
                                    v1.y()*v2.z() - v1.z()*v2.y(),
                                    v1.z()*v2.x() - v1.x()*v2.z(),
                                    v1.x()*v2.y() - v1.y()*v2.x()
                                );
        };
};

};

#endif