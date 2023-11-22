#include "errorHandler.hpp"
#include <GLFW/glfw3.h>

/**
 * Handle an error
 * @param error The error to handle
 * @param level The error level
*/
void ErrorHandler::handle(ErrorCodes error, ErrorLevel level){
    switch(error){
        case NO_ERROR:
            break;
        default:
            if(level == FATAL){
                fprintf(stderr, "Exiting the program!\n");
                exit(EXIT_FAILURE);
                break;
            }
            if(level == WARNING){
                fprintf(stderr, "Warning, continuing the program!\n");
                break;
            }
    }
}

/**
 * Handle a GLFW error
 * @param error The error to handle
 * @param level The error level
*/
void ErrorHandler::handleGLFW(ErrorCodes error, ErrorLevel level){
    fprintf(stderr, "GLFW error!\n");
    handle(error, level);
}

/**
 * Handle a Vulkan error
 * @param error The error to handle
 * @param level The error level
*/
void ErrorHandler::handleVulkan(ErrorCodes error, ErrorLevel level){
    fprintf(stderr, "Vulkan error!\n");
    handle(error, level);
}