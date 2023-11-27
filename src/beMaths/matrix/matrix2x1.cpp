#include "matrix2x1.hpp"
#include "errorHandler.hpp"

#include <beCore.hpp>

namespace beMaths{

/**
 * An empty constructor
*/
Matrix2x1f::Matrix2x1f()
    : Matrix2x1f(0.f,0.f){}

/**
 * A basic constructor
 * @param v The value for all the elements in the matrix
*/
Matrix2x1f::Matrix2x1f(float v)
    : Matrix2x1f(v,v){}

/**
 * A basic constructor
 * @param x The first value
 * @param y The second value
*/
Matrix2x1f::Matrix2x1f(float x, float y){
    _Mat = {x,y};
}

/**
 * Create a matrix fill with ones
*/
Matrix2x1f Matrix2x1f::ones(){
    return Matrix2x1f(1.f,1.f);
}

/**
 * Create a matrix fill with zeros
*/
Matrix2x1f Matrix2x1f::zeros(){
    return Matrix2x1f(0.f,0.f);
}

/**
 * Cast the matrix into a string
 * @return The string
*/
const std::string Matrix2x1f::toString() const {
    return "{" 
        + std::to_string(x())
        + ","
        + std::to_string(x())
        + "}"
    ; 
}

/**
 * Get the x value
 * @return x
*/
float Matrix2x1f::x() const{
    return _Mat[0];
}

/**
 * Get the y value
 * @return y
*/
float Matrix2x1f::y() const{
    return _Mat[1];
}

/**
 * Set the x value
 * @param x The new value
*/
void Matrix2x1f::x(float x){
    _Mat[0] = x;
}

/**
 * Set the y value
 * @param y The new value
*/
void Matrix2x1f::y(float y){
    _Mat[1] = y;
}

/**
 * Getter for the matrices elements
 * @param index The index of the element to access
 * @return The wanted value
*/
float Matrix2x1f::operator[](int index) const{
    if(index < 0 || index >= static_cast<int>(_Mat.size())){
        fprintf(stderr, "Index %d out of range for matrix2x1!\n", index);
        beCore::ErrorHandler::handle(beCore::ErrorCode::BAD_VALUE, beCore::ErrorLevel::WARNING);
    }
    return _Mat[index];
}

/**
 * Setter for the matrices elements
 * @param index The index of the element to set
 * @return The wanted value
*/
float& Matrix2x1f::operator[](int index){
    if(index < 0 || index >= static_cast<int>(_Mat.size())){
        fprintf(stderr, "Index %d out of range for matrix2x1!\n", index);
        beCore::ErrorHandler::handle(beCore::ErrorCode::BAD_VALUE, beCore::ErrorLevel::WARNING);
    }
    return _Mat[index];
}

};