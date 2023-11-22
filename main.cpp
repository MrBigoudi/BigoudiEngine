#include <cstdlib>
#include <stdexcept>

#include "app.hpp"

int main() {
    App app;
    try{
        app.run();
    } catch (const std::exception& e){
        fprintf(stderr, "Error when running the app: %s!\n", e.what());
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}