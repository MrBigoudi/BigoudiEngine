#include "be_mathsFcts.hpp"
#include <algorithm>
#include <cstdlib>

namespace be{

float Maths::random_float(){
    return rand() / (RAND_MAX + 1.f);
}

float Maths::random_float(float min, float max){
    return min + (max-min)*random_float();
}

int Maths::random_int(){
    return rand();
}

int Maths::random_int(int min, int max){
    return min + (max-min)*random_int();
}

bool Maths::isZero(float f){
    const float epsilon = 1e-9;
    return (f>-epsilon) && (f<epsilon);
}

float Maths::sqr(float f){
    return f*f;
}

float Maths::clamp(float f, float min, float max){
    return std::min(std::max(f, min), max);
}



}