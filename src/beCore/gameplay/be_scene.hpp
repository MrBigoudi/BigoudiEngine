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

        std::vector<PointLight> _PointLights = {};
        std::vector<DirectionalLight> _DirectionalLights = {};

        LightCutsTreePtr _LightTree = nullptr;

        /**
         * A smart pointer to the vulkan application
         * @see VulkanApp
        */
        VulkanAppPtr _VulkanApp = nullptr; 

    public:
        Scene(VulkanAppPtr vulkanApp): _VulkanApp(vulkanApp){};
        // void initFromGLTF(const std::string& filepath);
        std::vector<GameObject> getObjects() const {return _GameObjects;}

        void addGameObject(GameObject obj);
        void addGamePointLight(PointLight light);
        void addGameDirectionalLight(DirectionalLight light);

        void setPointLights(const std::vector<PointLight>& pointLights){
            _PointLights.clear();
            _PointLights = pointLights;
        }

        void setDirectionalLights(const std::vector<DirectionalLight>& directionalLights){
            _DirectionalLights.clear();
            _DirectionalLights = directionalLights;
        }

        void setLights(const std::vector<PointLight>& pointLights, const std::vector<DirectionalLight>& directionalLights){
            setPointLights(pointLights);
            setDirectionalLights(directionalLights);
            std::vector<PointLightPtr> ps = {};
            std::vector<DirectionalLightPtr> ds = {};
            for(auto p : pointLights){
                ps.push_back(std::make_shared<PointLight>(p));
            }
            for(auto d : directionalLights){
                ds.push_back(std::make_shared<DirectionalLight>(d));
            }

            if(ps.empty() && ds.empty()){
                return;
            }

            // for now
            if(!_LightTree){
                _LightTree = LightCutsTreePtr(
                    new LightCutsTree(
                        ps,
                        ds,
                        {}
                    )
                );
            }
        }

        std::vector<PointLight> getPointLights() const {
            return _PointLights;
        }

        std::vector<DirectionalLight> getDirectionalLights() const {
            return _DirectionalLights;
        }

    private:
        // tinygltf::Model loadModelGLTF(const std::string& filepath);
        // void initGameObjects(const tinygltf::Model& model);
        // void addGameObject(const tinygltf::Node& node, const tinygltf::Model& model);
        
        // void addGameObjectTransform(GameObject object, const tinygltf::Node& node);
        // void addGameObjectModel(GameObject object, const tinygltf::Node& node, const tinygltf::Model& model);


        // void initCameras(const tinygltf::Model& model);
        // void initLights(const tinygltf::Model& model);
};

};