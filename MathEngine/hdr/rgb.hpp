#ifndef __RGB_HPP__
#define __RGB_HPP__

#include <algorithm>

namespace MathEngine {

class RGB{

    private: // VALUES BETWEEN 0 AND 1
        float mVect[3];
        static const float MIN_RGB;
        static const float MAX_RGB;


    private: // UTILS
        static float inRange(float val) {
            return std::min(MAX_RGB, std::max(MIN_RGB, val));
        };

    public: // CONSTRUCTORS
        RGB(float r=0.0f, float g=0.0f, float b=0.0f){
            mVect[0] = inRange(r);
            mVect[1] = inRange(g);
            mVect[2] = inRange(b);
        };

        RGB(RGB const &c){
            mVect[0] = c.r();
            mVect[1] = c.b();
            mVect[2] = c.g();
        };

        ~RGB(){};

    public: // GETTERS
        float* getVect(){return mVect;};
        float r() const {return mVect[0];};
        float g() const {return mVect[1];};
        float b() const {return mVect[2];};


    public: // VECTORS OPERATIONS
        RGB operator + (RGB const &c){
            return * new RGB(
                        inRange(r() + c.r()), 
                        inRange(g() + c.g()), 
                        inRange(b() + c.b())
                        );
        };

        RGB operator - (RGB const &c){
            return * new RGB(
                        inRange(r() - c.r()), 
                        inRange(g() - c.g()), 
                        inRange(b() - c.b())
                        );
        };

        RGB operator * (float f){
            return * new RGB(
                        inRange(r()*f), 
                        inRange(g()*f), 
                        inRange(b()*f)
                        );
        };

        RGB operator / (float f){
            return * new RGB(
                        inRange(r()/f), 
                        inRange(g()/f), 
                        inRange(b()/f)
                        );
        };

};

};

#endif