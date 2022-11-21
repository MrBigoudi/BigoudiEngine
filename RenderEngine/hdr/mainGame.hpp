#pragma once

#include "opengl.hpp"
#include "mathEngine.hpp"
#include <string>

namespace RenderEngine{

class MainGame{

    private:
        GLFWwindow* mWindow = nullptr;
        int mWidth = 0;
        int mHeight = 0;
        const char* mTitle;
        float mFPS = 0.0f;
        float prevTime = 0.0f;

        GLuint mVaoID;
        GLuint mVboID;
        GLuint mProgramID;

    public:
        bool init(int width, int height, const char* title, float fps, std::string vertexShader, std::string fragmentShader);
        void run();
        void quit();

    private:
        bool initGLFW(int width, int height, const char* title);
        bool initGLEW();
        void update(float dt);
        void render();
        void inputHandler(int key, int scanCode, int action, int mods);

    public: // GETTERS
        GLFWwindow* win(){return mWindow;};

};

};