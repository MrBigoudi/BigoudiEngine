#pragma once

#include <cstdlib>
#include <iostream>

/**
* @enum The level of the error
*/
enum ErrorLevel{
    FATAL,
    WARNING,
};

/**
* @enum The different error codes
*/
enum ErrorCodes{
    NO_ERROR,
    INIT_FAILURE,
};

/**
* The class to handle errors
*/
class ErrorHandler{
    private:
        /**
        * Private constructor to make the class purely virtual
        */
        ErrorHandler(){};

    public:
        /**
         * Handle an error
         * @param error The error to handle
         * @param level The error level
        */
        static void handle(ErrorCodes error, ErrorLevel level = FATAL);

        /**
         * Handle a GLFW error
         * @param error The error to handle
         * @param level The error level
        */
        static void handleGLFW(ErrorCodes error, ErrorLevel level = FATAL);

        /**
         * Handle a Vulkan error
         * @param error The error to handle
         * @param level The error level
        */
        static void handleVulkan(ErrorCodes error, ErrorLevel level = FATAL);

};