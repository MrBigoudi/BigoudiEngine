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

void Scene::addGameObjectTransform(GameObject object, const tinygltf::Node& node){
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

    ComponentTransform::add(object, position, rotation, scaling); 
}

void Scene::addGameObjectModel(GameObject object, const tinygltf::Node& node, const tinygltf::Model& model){
    const tinygltf::Mesh& mesh = model.meshes[node.mesh];
    VertexDataBuilder builder{};
    
    // Iterate over the primitives in the mesh
    for (const tinygltf::Primitive& primitive : mesh.primitives) {
        // Check if the primitive has both vertex and index data
        if (primitive.indices < 0 || primitive.attributes.count("POSITION") == 0) {
            continue;
        }

        // Retrieve the vertex data
        const tinygltf::Accessor& positionAccessor = model.accessors[primitive.attributes.at("POSITION")];
        const tinygltf::BufferView& positionBufferView = model.bufferViews[positionAccessor.bufferView];
        const tinygltf::Buffer& positionBuffer = model.buffers[positionBufferView.buffer];

        const float* positions = reinterpret_cast<const float*>(&positionBuffer.data[positionAccessor.byteOffset + positionBufferView.byteOffset]);
        const uint32_t positionCount = static_cast<uint32_t>(positionAccessor.count);
        const uint32_t positionComponentCount = positionAccessor.type == TINYGLTF_TYPE_VEC3 ? 3 : 2;

        // Retrieve the color data if available
        const float* colors = nullptr;
        uint32_t colorCount = 0;
        uint32_t colorComponentCount = 0;
        if (primitive.attributes.count("COLOR_0") > 0) {
            const tinygltf::Accessor& colorAccessor = model.accessors[primitive.attributes.at("COLOR_0")];
            const tinygltf::BufferView& colorBufferView = model.bufferViews[colorAccessor.bufferView];
            const tinygltf::Buffer& colorBuffer = model.buffers[colorBufferView.buffer];

            colors = reinterpret_cast<const float*>(&colorBuffer.data[colorAccessor.byteOffset + colorBufferView.byteOffset]);
            colorCount = static_cast<uint32_t>(colorAccessor.count);
            colorComponentCount = colorAccessor.type == TINYGLTF_TYPE_VEC3 ? 3 : 4;
        }

        // Retrieve the normal data if available
        const float* normals = nullptr;
        uint32_t normalCount = 0;
        uint32_t normalComponentCount = 0;
        if (primitive.attributes.count("NORMAL") > 0) {
            const tinygltf::Accessor& normalAccessor = model.accessors[primitive.attributes.at("NORMAL")];
            const tinygltf::BufferView& normalBufferView = model.bufferViews[normalAccessor.bufferView];
            const tinygltf::Buffer& normalBuffer = model.buffers[normalBufferView.buffer];

            normals = reinterpret_cast<const float*>(&normalBuffer.data[normalAccessor.byteOffset + normalBufferView.byteOffset]);
            normalCount = static_cast<uint32_t>(normalAccessor.count);
            normalComponentCount = 3;
        }

        // Retrieve the UV data if available
        const float* uvs = nullptr;
        uint32_t uvCount = 0;
        uint32_t uvComponentCount = 0;
        if (primitive.attributes.count("TEXCOORD_0") > 0) {
            const tinygltf::Accessor& uvAccessor = model.accessors[primitive.attributes.at("TEXCOORD_0")];
            const tinygltf::BufferView& uvBufferView = model.bufferViews[uvAccessor.bufferView];
            const tinygltf::Buffer& uvBuffer = model.buffers[uvBufferView.buffer];

            uvs = reinterpret_cast<const float*>(&uvBuffer.data[uvAccessor.byteOffset + uvBufferView.byteOffset]);
            uvCount = static_cast<uint32_t>(uvAccessor.count);
            uvComponentCount = uvAccessor.type == TINYGLTF_TYPE_VEC2 ? 2 : 3;
        }

        // Retrieve the index data
        const tinygltf::Accessor& indexAccessor = model.accessors[primitive.indices];
        const tinygltf::BufferView& indexBufferView = model.bufferViews[indexAccessor.bufferView];
        const tinygltf::Buffer& indexBuffer = model.buffers[indexBufferView.buffer];

        const unsigned char* indicesData = &indexBuffer.data[indexAccessor.byteOffset + indexBufferView.byteOffset];
        const uint32_t* indices = reinterpret_cast<const uint32_t*>(indicesData);
        const uint32_t indexCount = static_cast<uint32_t>(indexAccessor.count);

        uint32_t nbVertices = positionCount / positionComponentCount;
        // if((colorCount > 0 && nbVertices != colorCount / colorComponentCount)
        //     || (normalCount > 0 && nbVertices != normalCount / normalComponentCount)
        //     || (uvCount > 0 && nbVertices != uvCount / uvComponentCount)){
            
        //     fprintf(stderr, "nbVert:%d\n \\
        //             positionCount: %d, positionComponentCount: %d\n \\
        //             colorCount: %d, colorComponentCount: %d\n \\
        //             normalCount: %d, normalComponentCount: %d\n \\
        //             uvCount: %d, uvComponentCount: %d\n",
        //             nbVertices, 
        //             positionCount, positionComponentCount, 
        //             colorCount, colorComponentCount, 
        //             normalCount, normalComponentCount, 
        //             uvCount, uvComponentCount
        //     );
        //     ErrorHandler::handle(
        //         ErrorCode::BAD_VALUE_ERROR,
        //         "Failed to create object from gltf scene, number of vertex elements are different!\n"
        //     );
        // }

        // Create a Mesh object and set its vertex and index data
        builder._Indices.resize(indexCount);
        builder._Vertices.resize(nbVertices);
        // init vertices
        for(uint32_t i = 0; i<nbVertices; i++){
            auto& v = builder._Vertices[i];
            // position
            for(uint32_t j=0; j<positionComponentCount; j++){
                v._Pos[j] = positions[i*positionComponentCount+j];
            }
            // // color
            // for(uint32_t j=0; j<colorComponentCount; j++){
            //     v._Col[j] = colors[i*colorComponentCount+j];
            // }
            // // normal
            // for(uint32_t j=0; j<normalComponentCount; j++){
            //     v._Norm[j] = normals[i*normalComponentCount+j];
            // }
            // // uv
            // for(uint32_t j=0; j<uvComponentCount; j++){
            //     v._Tex[j] = uvs[i*uvComponentCount+j];
            // }
        }
        // init indices
        for(uint32_t i = 0; i<indexCount; i++){
            builder._Indices[i] = indices[i];
        }

    }
    ComponentModel::add(object, _VulkanApp, builder);
}


void Scene::addGameObject(const tinygltf::Node& node, const tinygltf::Model& model){
    auto newObject = GameCoordinator::createObject();
    addGameObjectTransform(newObject, node);
    addGameObjectModel(newObject, node, model);
    _GameObjects.push_back(newObject);
}


void Scene::initGameObjects(const tinygltf::Model& model){
    const tinygltf::Scene& scene = model.scenes[model.defaultScene];

    for(const int nodeIndex : scene.nodes){
        const tinygltf::Node& node = model.nodes[nodeIndex];
        addGameObject(node, model);
    }
}

void Scene::initCameras(const tinygltf::Model& model[[maybe_unused]]){
    // TODO:
}

void Scene::initLights(const tinygltf::Model& model[[maybe_unused]]){
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