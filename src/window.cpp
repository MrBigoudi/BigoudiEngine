#include "window.hpp"
#include "errorHandler.hpp"

/**
 * A basic constructor
 * @param width The window's width
 * @param height The window's height
 * @param title The window's title
*/
Window::Window(uint32_t width, uint32_t height, const std::string& title){
    _Window = GLFWWindowPtr(glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr),
        GLFWWindowPtrDeleter());

    if(!_Window){
        fprintf(stderr, "Failed to create a GLFW window!\n");
        ErrorHandler::handleGLFW(ErrorCodes::INIT_FAILURE);
    }

    _Width = width;
    _Height = height;
}