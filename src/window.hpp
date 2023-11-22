#pragma once

#include <cstdint>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <memory>
#include <string>

/**
 * A typedefinition for a GLFWwindow smart pointrt
*/
using GLFWWindowPtr = std::shared_ptr<GLFWwindow>;

/**
 * A custom deleter for the GLFWwindow smart pointer
*/
struct GLFWWindowPtrDeleter{
    void operator()(GLFWwindow* window) const {
        glfwDestroyWindow(window);
    }
};

/**
 * A class representing a window
*/
class Window{
    private:
        /**
         * The window's width
        */
        uint32_t _Width = 0;

        /**
         * The window's height
        */
        uint32_t _Height = 0;

        /**
         * The glfw window
        */
        GLFWWindowPtr _Window = nullptr;

    public:
        /**
         * A basic constructor
         * @param width The window's width
         * @param height The window's height
         * @param title The window's title
        */
        Window(uint32_t width, uint32_t height, const std::string& title);

        /**
         * A getter for the window's width
         * @return The window's width
        */
        uint32_t getWidth() const {
            return _Width;
        }

        /**
         * A getter for the window's height
         * @return The window's height
        */
        uint32_t getHeight() const {
            return _Height;
        }

        /**
         * A getter for the GLFW window
         * @return A pointer to the GLFW window
        */
        GLFWwindow* getWindow() const {
            return _Window.get();
        }
};

/**
 * A smart pointer for the window
*/
using WindowPtr = std::shared_ptr<Window>;