#pragma once

#include <vulkan/vulkan.hpp>

namespace be{

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
enum ErrorCode{
    NO_ERROR, 

    GLFW_ERROR,
    VULKAN_ERROR,
    TINYOBJ_ERROR,
    STB_IMAGE_ERROR,
    TINYGLTF_ERROR,

    IO_ERROR,
    UNIMPLEMENTED_ERROR,
    NOT_INITIALIZED_ERROR,
    UNKNOWN_VALUE_ERROR,
    BAD_VALUE_ERROR,
    OUT_OF_RANGE_ERROR,
    UNEXPECTED_VALUE_ERROR,
    ZERO_DIVIDE_ERROR,

    SYSTEM_ERROR,
};

/**
 * The class to handle errors
*/
class ErrorHandler{
    private:
        /**
         * Private constructor to make the class purely virtual
        */
        ErrorHandler();

    public:
        /**
         * Handle an error
         * @param error The error to handle
         * @param msg The error message to display
         * @param level The error level
        */
        static void handle(const std::string& fileName, int lineNumber, ErrorCode error, const std::string& msg = "", ErrorLevel level = FATAL);

        /**
         * Handle a vulkan error
         * @param error The error to handle
         * @param msg Th error message to display
         * @param level The error level
        */
        static void vulkanError(const std::string& fileName, int lineNumber, VkResult result, const std::string& msg = "", ErrorLevel level = FATAL);

        /**
         * Handle a glfw error
         * @param msg Th error message to display
         * @param level The error level
        */
        static void glfwError(const std::string& fileName, int lineNumber, const std::string& msg, ErrorLevel level = FATAL);

    private:
        /**
         * Default error handler
         * @note if level == FATAL, exit the program
         * @note if level == WARNING, print the warning
         * @param msg The error message to display
         * @param level The error level
        */
        static void defaultCase(const std::string& fileName, int lineNumber, const std::string& msg = "", ErrorLevel level = FATAL);

};

}