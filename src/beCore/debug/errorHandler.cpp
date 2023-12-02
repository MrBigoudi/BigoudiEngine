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
        case ZERO_DIVIDE:
            fprintf(stderr, "Can't divide by zero!\n");
            break;
        default:
            break;
    }
    switch(level){
        case FATAL:
            fprintf(stderr, "Exiting the program!\n");
            exit(EXIT_FAILURE);
            break;
        
        case WARNING:
            fprintf(stderr, "Warning, continue the program!\n");
            break;
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