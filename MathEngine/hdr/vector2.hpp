#ifndef __VECTOR_2_HPP__
#define __VECTOR_2_HPP__

namespace MathEngine{

class Vector2f{

    private:
        float mVect[2];

    public: // CONSTRUCTORS
        Vector2f(float x=0.0f, float y=0.0f){
            mVect[0] = x;
            mVect[1] = y;
        }

        Vector2f(Vector2f& v){
            mVect[0] = v.x();
            mVect[1] = v.y();
        }

        ~Vector2f(){}


    public: // GETTERS
        float* getVect(){return mVect;};
        float x() const {return mVect[0];};
        float y() const {return mVect[1];};


    public: // VECTORS OPERATIONS
        Vector2f operator + (Vector2f const &v){
            return * new Vector2f(x() + v.x(), y() + v.y());
        };

        Vector2f operator - (Vector2f const &v){
            return * new Vector2f(x() - v.x(), y() - v.y());
        };

        Vector2f operator * (float f){
            return * new Vector2f(x()*f, y()*f);
        };

        Vector2f operator / (float f){
            return * new Vector2f(x()/f, y()/f);
        };

        static float dot(Vector2f const &v1, Vector2f const &v2){
            return v1.x()*v2.x() + v1.y()*v2.y();
        }

        static float cross(Vector2f const &v1, Vector2f const &v2){
            return v1.x()*v2.y() - v1.y()*v2.x();
        };
};

};

#endif