#pragma once

#include "be_camera.hpp"
#include "be_gameObject.hpp"
#include "be_lights.hpp"

#include "tiny_gltf.h"
#include <vector>

namespace be{

class Scene;
using ScenePtr = std::shared_ptr<Scene>;

class Scene{

    private:
        std::vector<GameObject> _GameObjects = {};

        std::vector<CameraPtr> _Cameras = {};
        uint32_t _MainCamera = 0; // id of main camera

        std::vector<Light> _Lights = {};

        /**
         * A smart pointer to the vulkan application
         * @see VulkanApp
        */
        VulkanAppPtr _VulkanApp = nullptr; 

    public:
        Scene(VulkanAppPtr vulkanApp): _VulkanApp(vulkanApp){};
        void initFromGLTF(const std::string& filepath);
        std::vector<GameObject> getObjects() const {return _GameObjects;}

    private:
        tinygltf::Model loadModelGLTF(const std::string& filepath);
        void initGameObjects(const tinygltf::Model& model);
        void addGameObject(const tinygltf::Node& node, const tinygltf::Model& model);
        
        void addGameObjectTransform(GameObject object, const tinygltf::Node& node);
        void addGameObjectModel(GameObject object, const tinygltf::Node& node, const tinygltf::Model& model);


        void initCameras(const tinygltf::Model& model);
        void initLights(const tinygltf::Model& model);
};

};