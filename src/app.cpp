#include "app.hpp"
#include "errorHandler.hpp"
#include <GLFW/glfw3.h>

/**
 * Init the window
*/
void App::initWindow(){
    int error = glfwInit();
    if(error != GLFW_TRUE){
        fprintf(stderr, "Failed to init GLFW!\n");
        ErrorHandler::handleGLFW(ErrorCodes::INIT_FAILURE);
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    _Window = WindowPtr(new Window(800, 600, "Vulkan"));
}

/**
 * init vulkan
*/
void App::initVulkan(){
    createVulkanInstance();
}

/**
 * main loop
*/
void App::mainLoop(){
    while(!glfwWindowShouldClose(_Window->getWindow())){
        glfwPollEvents();
    }
}

/**
 * cleaning
*/
void App::cleanup(){
    glfwTerminate();
}

/**
 * Run the app
*/
void App::run(){
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}