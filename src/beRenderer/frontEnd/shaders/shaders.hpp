#ifndef __SHADERS_HPP__
#define __SHADERS_HPP__

#include <memory>

#include <beCore.hpp>
#include <beMaths.hpp>

namespace beRenderer{

/**
 * A forward declaration of the shader class
 * @see Shaders
*/
class Shaders;

/**
 * A smart pointer to the Shader class
*/
using ShadersPointer = std::shared_ptr<Shaders>;

/**
 * @enum The different type of shaders
*/
enum ShaderType{VERT, FRAG, GEOM};

/**
 * A class representing shaders
*/
class Shaders{
};

}

#endif