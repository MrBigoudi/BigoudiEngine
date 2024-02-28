#include "be_scene.hpp"
#include "be_gameCoordinator.hpp"
#include "be_components.hpp" // IWYU pragma: keep

namespace be{

void Scene::initFromGLTF(const std::string& filepath){
    tinygltf::Model gltfModel = loadModelGLTF(filepath);
    initGameObjects(gltfModel);
    initCameras(gltfModel);
    initLights(gltfModel);
}

void Scene::initGameObjects(const tinygltf::Model& model){
    const tinygltf::Scene& scene = model.scenes[model.defaultScene];

    for(const int nodeIndex : scene.nodes){
        const tinygltf::Node& node = model.nodes[nodeIndex];

        auto newObject = GameCoordinator::createObject();
        Vector3 position{};
        Vector3 rotation{};
        Vector3 scaling{};

        if(!node.translation.empty()){
            position = {
                static_cast<float>(node.translation[0]), 
                static_cast<float>(node.translation[1]), 
                static_cast<float>(node.translation[2])
            };
        }

        if(!node.rotation.empty()){
            // TODO: quaternion to euler angles
        }
        if(!node.scale.empty()){
            scaling = {
                static_cast<float>(node.scale[0]),
                static_cast<float>(node.scale[1]),
                static_cast<float>(node.scale[2])
            };
        }

        ComponentTransform::add(newObject, position, rotation, scaling);
    }
}

void Scene::initCameras(const tinygltf::Model& model){
    // TODO:
}

void Scene::initLights(const tinygltf::Model& model){
    // TODO:
}


tinygltf::Model Scene::loadModelGLTF(const std::string& filepath){
    tinygltf::Model gltfModel;
    tinygltf::TinyGLTF gltfLoader;
    std::string err;
    std::string warn;

    bool res = gltfLoader.LoadASCIIFromFile(&gltfModel, &err, &warn, filepath);
    if (!warn.empty()) {
        ErrorHandler::handle(
            ErrorCode::TINYGLTF_ERROR,
            "Warning loading gltf file `" + filepath + "':\n\t"+warn+"\n",
            ErrorLevel::WARNING
        );
    }

    if (!err.empty()) {
        ErrorHandler::handle(
            ErrorCode::TINYGLTF_ERROR,
            "Error loading gltf file `" + filepath + "':\n\t"+err+"\n"
        );
    }

    if (!res){
        ErrorHandler::handle(
            ErrorCode::TINYGLTF_ERROR,
            "Failed to load gltf file `" + filepath + "'\n"
        );
    }

    return gltfModel;
}

};