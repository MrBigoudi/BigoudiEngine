#pragma once

#include "be_model.hpp"
#include "be_gameCoordinator.hpp"

namespace be{

/**
 * A component structure to store game objects models (=mesh)
 * @see Components
 * @see Model
*/
struct ComponentModel{
    /**
     * A smart pointer to the model
    */
    ModelPtr _Model = nullptr;

    static ComponentModel create(VulkanAppPtr vulkanApp, const VertexDataBuilder& dataBuilder){
        return {._Model = ModelPtr(new Model(vulkanApp, dataBuilder))};
    }

    static ComponentModel create(VulkanAppPtr vulkanApp, const std::string& filePath){
        return {._Model = ModelPtr(new Model(vulkanApp, filePath))};
    }

    static void add(GameObject object, VulkanAppPtr vulkanApp, const VertexDataBuilder& dataBuilder){
        GameCoordinator::addComponent(
            object, 
            create(vulkanApp, dataBuilder)
        );
    }

    static void add(GameObject object, VulkanAppPtr vulkanApp, const std::string& filePath){
        GameCoordinator::addComponent(
            object, 
            create(vulkanApp, filePath)
        );
    }
};

};