#include "renderEngine.hpp"
#include <cstdlib>

using namespace RenderEngine;

int main(void)
{
    
    MainGame game;
    /* Initialize the library */
    if (!game.init(640, 480, "Hello World", 60, "GameTest/src/shaders/vertex.glsl", "GameTest/src/shaders/fragment.glsl"))
        exit(EXIT_FAILURE);
    game.run();
    game.quit();
    exit(EXIT_SUCCESS);
}