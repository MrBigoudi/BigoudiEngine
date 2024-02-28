#pragma once

#include "be_camera.hpp"
#include "be_gameObject.hpp"
#include "be_lights.hpp"

#include "tiny_gltf.h"
#include <vector>

namespace be{

class Scene{

    private:
        std::vector<GameObject> _GameObjects = {};

        std::vector<CameraPtr> _Cameras = {};
        uint32_t _MainCamera = 0; // id of main camera

        std::vector<Light> _Lights = {};

    public:
        Scene(){};
        void initFromGLTF(const std::string& filepath);

    private:
        tinygltf::Model loadModelGLTF(const std::string& filepath);
        void initGameObjects(const tinygltf::Model& model);
        void initCameras(const tinygltf::Model& model);
        void initLights(const tinygltf::Model& model);
};

};