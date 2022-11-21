#include "mainGame.hpp"
#include "loadShaders.hpp"

namespace RenderEngine{

bool MainGame::initGLFW(int width, int height, const char* title){
    /* Initialize the library */
    if (!glfwInit()){
        fprintf(stderr, "Failed to initialize GLFW!\n");
        return false;
    }

    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 
    glfwWindowHint(GLFW_RESIZABLE, false);
    glfwWindowHint(GLFW_DOUBLEBUFFER, true);
    mWindow = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!mWindow){
        fprintf(stderr, "Failed to initialize the GLFW window!\n");
        return false;
    }
    // center the window
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwSetWindowPos(mWindow, (mode->width - width)/2, (mode->height - height)/2);

    /* Make the window's context current */
    glfwMakeContextCurrent(mWindow);

    return true;
}


bool MainGame::initGLEW(){
    glewExperimental=true; // Needed in core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return false;
    }
    
    glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
    return true;
}

bool MainGame::init(int width, int height, const char* title, float fps, std::string vertexShader, std::string fragmentShader){
    // init GLFW and GLEW
    if (!initGLFW(width, height, title) || !initGLEW()){
        return false;
    }

    glGenVertexArrays(1, &mVaoID);
    glBindVertexArray(mVaoID);
    
    //Check for error
    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
        return false;
    }

    mWidth = width;
    mHeight = height;
    mTitle = title;
    mFPS = fps;
    mProgramID = LoadShaders(vertexShader.c_str(), fragmentShader.c_str());

    glUseProgram(mProgramID);

    return true;
}

void MainGame::quit(){
    glfwTerminate();
}

void MainGame::run(){
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(mWindow, GLFW_STICKY_KEYS, GL_TRUE);

    // An array of 3 vectors which represents 3 vertices
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f,  1.0f,  0.0f,
         1.0f,  1.0f,  0.0f,
        -1.0f, -1.0f,  0.0f,

         1.0f,  1.0f,  0.0f,
        -1.0f, -1.0f,  0.0f,
         1.0f, -1.0f,  0.0f
    };
    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &mVboID);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, mVboID);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    /* Loop until the user closes the window */
    while (glfwGetKey(mWindow, GLFW_KEY_ESCAPE ) != GLFW_PRESS 
        && glfwWindowShouldClose(mWindow) == 0){
        /* update */
        float now = glfwGetTime();
        float dt = now - prevTime;
        prevTime = now;
        update(dt);
        render();

        /* Poll for and process events */
        glfwPollEvents();
    }
}

void MainGame::update(float dt){}

void MainGame::render(){
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    // draw stuff
    // 1st attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, mVboID);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );
    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDrawArrays(GL_TRIANGLES, 3, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDisableVertexAttribArray(0);

    /* Swap front and back buffers */
    glfwSwapBuffers(mWindow);
}

};