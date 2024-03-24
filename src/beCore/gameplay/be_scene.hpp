#pragma once

#include "be_camera.hpp"
#include "be_gameObject.hpp"
#include "be_lights.hpp"
#include "be_renderSubSystem.hpp"

// #include "tiny_gltf.h"
#include <vector>

namespace be{

/**
 * Forward declaration of the scene class
 * @see Scene
*/
class Scene;

/**
 * Smart pointer to a scene
 * @see Scene
*/
using ScenePtr = std::shared_ptr<Scene>;


/**
 * A class representing a game scene
*/
class Scene{

    private:
        /**
         * The list of game objects
        */
        std::vector<GameObject> _GameObjects = {};

        /**
         * The list of cameras
        */
        std::vector<CameraPtr> _Cameras = {};

        /**
         * The id of the main camera
        */
        uint32_t _MainCamera = 0;

        /**
         * The list of point lights
         * @see Light
        */
        std::vector<PointLightPtr> _PointLights = {};

        /**
         * The list of directional lights
         * @see Light
        */
        std::vector<DirectionalLightPtr> _DirectionalLights = {};

        /**
         * The light-tree for lightcuts acceleration
         * @see Light
        */
        LightCutsTreePtr _LightTree = nullptr;

        /**
         * A smart pointer to the vulkan application
         * @see VulkanApp
        */
        VulkanAppPtr _VulkanApp = nullptr; 

    public:
        /**
         * A basic constructor
         * @param vulkanApp A pointer to the vulkan application
        */
        Scene(VulkanAppPtr vulkanApp): _VulkanApp(vulkanApp){};

        /**
         * A getter to the list of game objects
         * @return the list of objects
        */
        std::vector<GameObject> getObjects() const {return _GameObjects;}

        /**
         * Add a game object to the scene
         * @param obj The object to add
        */
        void addGameObject(GameObject obj);

        /**
         * Add a point light to the scene
         * @param light The light to add
        */
        void addGamePointLight(PointLightPtr light);

        /**
         * Add a directional light to the scene
         * @param light The light to add
        */
        void addGameDirectionalLight(DirectionalLightPtr light);

        /**
         * Add a point light to the scene
         * @param position The light position
         * @param color The light color
         * @param intensity The light intensity
         * @param makeVisible Choose to display the point light as a sphere, default false
        */
        void addGamePointLight(const Vector3& position, const Vector3& color, float intensity, bool makeVisible=false);

        /**
         * Add a directional light to the scene
         * @param direction The light direction
         * @param color The light color
         * @param intensity The light intensity
        */
        void addGameDirectionalLight(const Vector3& direction, const Vector3& color, float intensity);

        /**
         * Getter to the scene point lights
         * @return The vector of point lights
        */
        std::vector<PointLightPtr> getPointLights() const;

        /**
         * Getter to the scene directional lights
         * @return The vector of directional lights
        */
        std::vector<DirectionalLightPtr> getDirectionalLights() const;

        void buildTree(){
            _LightTree = LightCutsTreePtr(new LightCutsTree(
                _PointLights, _DirectionalLights, {}
            ));
        }

        LightCutsTree::LightNodePtr getLightTreeRoot() const {
            return _LightTree->getRoot();
        }

    public:
        GameObject addCubeOfLight(IRenderSubSystemPtr rss, 
            const Vector3& center, float length, 
            const Vector3& color, float intensity,
            const Vector3& rotation = Vector3::zeros(),
            float step = 0.1f
        );

        GameObject addCubeOfLight(IRenderSubSystemPtr rss, 
            const Vector3& center, const Vector3& scale, 
            const Vector3& color, float intensity,
            const Vector3& rotation = Vector3::zeros(),
            float step = 0.1f
        );

        GameObject addCubeOfLight(IRenderSubSystemPtr rss, 
            const Vector3& center, const Vector3& scale, 
            const Vector3& color, float intensity,
            const Vector3& rotation = Vector3::zeros(),
            const Vector3& step = Vector3(0.1f, 0.1f, 0.1f)
        );

};

};