#include "errorHandler.hpp"

#include <GL/gl.h>
#include <cstdlib>
#include <iostream>
#include <stdarg.h>

namespace beCore{

/**
 * Private constructor to make the class purely virtual
*/
ErrorHandler::ErrorHandler(){
}

/**
 * Handle the error
 * @param error The error to handle
 * @param level The error level
*/
void ErrorHandler::handle(ErrorCode error, ErrorLevel level){
    switch(error){
        case NO_ERROR:
            break;
        default:
            if(level == FATAL){
                std::cerr << "Exiting the program!" << std::endl;
                exit(EXIT_FAILURE);
                break;
            }
            if(level == WARNING){
                std::cerr << "Warning, continue the program!" << std::endl;
                break;
            }
    }
}

/**
 * Handle an OpenGL error
 * @param format The error message
*/
void ErrorHandler::handleGL(const char* format, ...){
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        va_list args;
        va_start(args, format);
        vfprintf(stderr, format, args);
        va_end(args);
        handle(GL_ERROR);
    }
}

};