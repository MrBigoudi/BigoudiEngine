#include "be_mathsFcts.hpp"
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


}