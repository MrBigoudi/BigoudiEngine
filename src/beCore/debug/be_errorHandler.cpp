#include "be_errorHandler.hpp"

namespace be{

/**
 * Default error handler
 * @note if level == FATAL, exit the program
 * @note if level == WARNING, print the warning
 * @param msg The error message to display
 * @param level The error level
*/
void ErrorHandler::defaultCase(const std::string& fileName, int lineNumber, const std::string& msg, ErrorLevel level){
    switch(level){
        case FATAL:
            fprintf(stderr, "Error triggered in %s:%d\n\t", fileName.c_str(), lineNumber);
            fprintf(stderr, "%s", msg.c_str());
            fprintf(stderr, "Exiting the program!\n");
            exit(EXIT_FAILURE);
            break;
        case WARNING:
            fprintf(stderr, "Warning triggered in %s:%d\n\t", fileName.c_str(), lineNumber);
            fprintf(stderr, "%s", msg.c_str());
            fprintf(stderr, "Warning, continue the program!\n");
            break;
        default:
            break;
    }
}

/**
 * Handle an error
 * @param error The error to handle
 * @param msg The error message to display
 * @param level The error level
*/
void ErrorHandler::handle(const std::string& fileName, int lineNumber, ErrorCode error, const std::string& msg, ErrorLevel level){
    switch(error){
        case NO_ERROR:
            break;
        default:
            defaultCase(fileName, lineNumber, msg, level);
            break;
    }
}

/**
 * Handle a vulkan error
 * @param error The error to handle
 * @param msg Th error message to display
 * @param level The error level
*/
void ErrorHandler::vulkanError(const std::string& fileName, int lineNumber, VkResult result, const std::string& msg, ErrorLevel level){
    if(result == VK_SUCCESS){
        return;
    }
    handle(fileName, lineNumber, VULKAN_ERROR, "Vulkan error, code: " + std::to_string(result) + "\n\t" + msg, level);
}

/**
 * Handle a glfw error
 * @param msg Th error message to display
 * @param level The error level
*/
void ErrorHandler::glfwError(const std::string& fileName, int lineNumber, const std::string& msg, ErrorLevel level){
    handle(fileName, lineNumber, GLFW_ERROR, msg, level);
}

};