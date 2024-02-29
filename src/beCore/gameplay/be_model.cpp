#include "be_model.hpp"

#include "be_buffer.hpp"
#include "be_errorHandler.hpp"
#include <cstdint>
#include <unordered_map>
#include <fstream>
#include <sstream>

#include "be_trigonometry.hpp"
#include "be_vector3.hpp"
#include "be_vulkanApp.hpp"

#include "be_utilityFunctions.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "be_dep.hpp"

#include "be_hash.hpp" // IWYU pragma: keep

template<>
struct std::hash<be::VertexData>{
    size_t operator()(const be::VertexData& vert) const {
        size_t seed = 0;
        be::hashCombine(seed, vert._Pos, vert._Col, vert._Norm, vert._Tex);
        return seed;
    }
};

namespace be{

void Model::createVertexBuffer(const std::vector<VertexData>& vertices){
    _VertexCount = static_cast<uint32_t>(vertices.size());
    if(_VertexCount < MIN_VERTEX_COUNT){
        ErrorHandler::handle(__FILE__, __LINE__, ErrorCode::BAD_VALUE_ERROR, "Vertex count must be at least 3!\n");
    }

    uint32_t vertexSize = sizeof(vertices[0]);

    // buffer to staging
    Buffer stagingBuffer{
        _VulkanApp,
        vertexSize,
        _VertexCount,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    };

    stagingBuffer.map();
    stagingBuffer.writeToBuffer((void*)vertices.data());

    // staging to host
    _VertexBuffer = BufferPtr(new Buffer(
        _VulkanApp,
        vertexSize,
        _VertexCount,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    ));
    VkDeviceSize bufferSize = sizeof(vertices[0]) * _VertexCount;
    _VulkanApp->copyBuffer(stagingBuffer.getBuffer(), _VertexBuffer->getBuffer(), bufferSize);

    // cleaning staging buffer
    stagingBuffer.cleanUp();
}

void Model::createIndexBuffer(const std::vector<uint32_t>& indices){
    _IndexCount = static_cast<uint32_t>(indices.size());
    if(_IndexCount == 0){
        _HasIndexBuffer = false;
        return;
    }
    _HasIndexBuffer = true;

    uint32_t indexSize = sizeof(indices[0]);

    // buffer to staging
    Buffer stagingBuffer{
        _VulkanApp,
        indexSize,
        _IndexCount,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT  
    };

    stagingBuffer.map();
    stagingBuffer.writeToBuffer((void*)indices.data());

    // staging to host
    _IndexBuffer = BufferPtr(new Buffer(
        _VulkanApp,
        indexSize,
        _IndexCount,
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    ));
    VkDeviceSize bufferSize = sizeof(uint32_t) * _IndexCount;
    _VulkanApp->copyBuffer(stagingBuffer.getBuffer(), _IndexBuffer->getBuffer(), bufferSize);

    // cleaning staging buffer
    stagingBuffer.cleanUp();
}

const std::vector<VkFormat> VertexData::FORMATS = {
    VK_FORMAT_R32G32B32_SFLOAT,
    VK_FORMAT_R32G32B32A32_SFLOAT,
    VK_FORMAT_R32G32B32_SFLOAT,
    VK_FORMAT_R32G32_SFLOAT
};

const std::vector<size_t> VertexData::OFFSETS = {
    offsetof(VertexData, _Pos),
    offsetof(VertexData, _Col),
    offsetof(VertexData, _Norm),
    offsetof(VertexData, _Tex),
};

void VertexDataBuilder::loadObjModel(const std::string& filePath){
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn;
    std::string error;

    auto result = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &error, filePath.c_str());
    if(!result){
        ErrorHandler::handle(__FILE__, __LINE__, 
            ErrorCode::TINYOBJ_ERROR,
            "Failed to load .obj model: " + filePath + "!\n"
        );
    }

    _Vertices.clear();
    _Indices.clear();

    std::unordered_map<VertexData, uint32_t> uniqueVertices{};

    for(const auto& shape : shapes){
        for(const auto& index : shape.mesh.indices){
            VertexData vertex{};

            if(index.vertex_index >= 0){
                vertex._Pos = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };

                vertex._Col = {
                    attrib.colors[3 * index.vertex_index + 0],
                    attrib.colors[3 * index.vertex_index + 1],
                    attrib.colors[3 * index.vertex_index + 2],
                    1.f
                };
            }

            if(index.normal_index >= 0){
                vertex._Norm = {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]
                };
            }

            if(index.texcoord_index >= 0){
                vertex._Tex = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    attrib.texcoords[2 * index.texcoord_index + 1]
                };
            }

            if(uniqueVertices.count(vertex) == 0){
                uniqueVertices[vertex] = static_cast<uint32_t>(_Vertices.size());
                _Vertices.push_back(vertex);
            }

            _Indices.push_back(uniqueVertices[vertex]);
        }
    }
}

void VertexDataBuilder::loadOffModel(const std::string& filePath){
	std::ifstream file(filePath.c_str());
    if(!file){
        ErrorHandler::handle(__FILE__, __LINE__, 
            ErrorCode::IO_ERROR,
            "Failed to load off model: " + filePath + "!\n"
        );
    }
	
    std::string line = "";
    std::getline(file, line);
    if(line.find("OFF") == std::string::npos){
        ErrorHandler::handle(__FILE__, __LINE__, 
            ErrorCode::BAD_VALUE_ERROR,
            "Trying to load a non .off file: " + filePath + "!\n"
        );
    }

    std::getline(file, line);

    std::istringstream iss(line);
    int numVertices = 0;
    int numFaces = 0;
    int numCells = 0;
    iss >> numVertices >> numFaces >> numCells;

    if(!_Vertices.empty()) _Vertices.clear();
    if(!_Indices.empty()) _Indices.clear();

    // vertices
    for(int i = 0; i < numVertices; i++){
        VertexData vertex{};
        std::getline(file, line);
        std::istringstream vertIss(line);
        float x = 0.f;
        float y = 0.f;
        float z = 0.f;
        vertIss >> x >> y >> z;
        vertex._Pos = {x, y, z};
        _Vertices.push_back(vertex);
    }

    // indices
    for(int i=0; i<numFaces; i++){
        int numIndicesPerFace = 0;
        file >> numIndicesPerFace;
        for(int j=0; j<numIndicesPerFace; j++){
            int idx = 0;
            file >> idx;
            _Indices.push_back(idx);
        }
    }

    // normals
	for(int i=0; i<int(_Indices.size()); i+=3){
        // assuming triangle meshes
        int v0 = _Indices[i];
        int v1 = _Indices[i+1];
        int v2 = _Indices[i+2];

		Vector3 e0 = _Vertices[v1]._Pos - _Vertices[v0]._Pos;
		Vector3 e1 = _Vertices[v2]._Pos - _Vertices[v0]._Pos;
		Vector3 n = Vector3::normalize(Vector3::cross(e0, e1));
		for(int j=0; j<3; j++){
            _Vertices[_Indices[i+j]]._Norm += n;
        }
	}
	for(auto& vert : _Vertices){
        vert._Norm.normalize(); 
    }

    file.close();
}


VertexDataBuilder VertexDataBuilder::primitiveTriangle(
    const Vector3& v0,
    const Vector3& v1,
    const Vector3& v2,
    const Vector4& c){
    VertexDataBuilder builder{};
    builder._Vertices = {
        {._Pos = v0, ._Col = c},
        {._Pos = v1, ._Col = c},
        {._Pos = v2, ._Col = c},
    };
    builder._Indices = {0,1,2};
    return builder;
}

VertexDataBuilder VertexDataBuilder::primitiveRectangle(
    float height,
    float width,
    const Vector4& c){
    Vector3 v0{-width/2.f, -height/2.f, 0.f};
    Vector3 v1{width/2.f, -height/2.f, 0.f};
    Vector3 v2{-width/2.f, height/2.f, 0.f};
    Vector3 v3{width/2.f, height/2.f, 0.f};
    VertexDataBuilder builder{};
    builder._Vertices = {
        {._Pos = v0, ._Col = c},
        {._Pos = v1, ._Col = c},
        {._Pos = v2, ._Col = c},
        {._Pos = v3, ._Col = c},
    };
    builder._Indices = {
        0,1,2,
        2,1,3
    };
    return builder;
}


VertexDataBuilder VertexDataBuilder::primitiveCube(
    float length,
    const Vector4& c){
    float hl = length / 2.f;
    VertexDataBuilder builder{};
    builder._Vertices = {
        {._Pos = {-hl, -hl, hl}, ._Col = c, ._Norm = {0, 0, 1}},
        {._Pos = {hl, -hl, hl}, ._Col = c, ._Norm = {0, 0, 1}},
        {._Pos = {-hl, hl, hl}, ._Col = c, ._Norm = {0, 0, 1}},
        {._Pos = {hl, hl, hl}, ._Col = c, ._Norm = {0, 0, 1}},

        {._Pos = {-hl, -hl, -hl}, ._Col = c, ._Norm = {0, 0, -1}},
        {._Pos = {hl, -hl, -hl}, ._Col = c, ._Norm = {0, 0, -1}},
        {._Pos = {-hl, hl, -hl}, ._Col = c, ._Norm = {0, 0, -1}},
        {._Pos = {hl, hl, -hl}, ._Col = c, ._Norm = {0, 0, -1}},
        
        {._Pos = {-hl, -hl, hl}, ._Col = c, ._Norm = {-1, 0, 0}},
        {._Pos = {-hl, hl, hl}, ._Col = c, ._Norm = {-1, 0, 0}},
        {._Pos = {-hl, -hl, -hl}, ._Col = c, ._Norm = {-1, 0, 0}},
        {._Pos = {-hl, hl, -hl}, ._Col = c, ._Norm = {-1, 0, 0}},
        
        {._Pos = {hl, -hl, hl}, ._Col = c, ._Norm = {1, 0, 0}},
        {._Pos = {hl, hl, hl}, ._Col = c, ._Norm = {1, 0, 0}},
        {._Pos = {hl, -hl, -hl}, ._Col = c, ._Norm = {1, 0, 0}},
        {._Pos = {hl, hl, -hl}, ._Col = c, ._Norm = {1, 0, 0}},
        
        {._Pos = {-hl, hl, hl}, ._Col = c, ._Norm = {0, 1, 0}},
        {._Pos = {hl, hl, hl}, ._Col = c, ._Norm = {0, 1, 0}},
        {._Pos = {-hl, hl, -hl}, ._Col = c, ._Norm = {0, 1, 0}},
        {._Pos = {hl, hl, -hl}, ._Col = c, ._Norm = {0, 1, 0}},
        
        {._Pos = {-hl, -hl, hl}, ._Col = c, ._Norm = {0, -1, 0}},
        {._Pos = {hl, -hl, hl}, ._Col = c, ._Norm = {0, -1, 0}},
        {._Pos = {-hl, -hl, -hl}, ._Col = c, ._Norm = {0, -1, 0}},
        {._Pos = {hl, -hl, -hl}, ._Col = c, ._Norm = {0, -1, 0}}
    };
    
    builder._Indices = {
        // front face
        0, 1, 2,
        2, 1, 3,
        // back face
        4, 5, 6,
        6, 5, 7,
        // left face
        8, 9, 10,
        10, 9, 11,
        // right face
        12, 13, 14,
        14, 13, 15,
        // top face
        16, 17, 18,
        18, 17, 19,
        // bottom face
        20, 21, 22,
        22, 21, 23
    };
    
    return builder;
}


VertexDataBuilder VertexDataBuilder::primitiveSphere(
    size_t resolution,
    const Vector4& c){
    VertexDataBuilder builder{};
    // vertices
    std::vector<Vector3> vertices{};
    {
        const float stepPhi = PI / resolution;
        const float stepTheta = 2 * PI / resolution;
        float phi = 0.0f;
        for (int i = 0; i <= int(resolution); i++) {
            phi += stepPhi;
            float theta = 0.0f;
            for (int j = 0; j <= int(resolution); j++) {
                theta += stepTheta;

                float x = sin(theta)*sin(phi);
                float y = cos(phi);
                float z = cos(theta)*sin(phi);

                vertices.push_back({x,y,z});
            }
        }
    }

    // normals
    std::vector<Vector3> normals;
    {
        for(int i=0; i<int(vertices.size()); i++){
            normals.push_back(Vector3::normalize(vertices[i]));
        }
    }

    for(int i=0; i<int(vertices.size()); i++){
        builder._Vertices.push_back({
            ._Pos = vertices[i],
            ._Col = c,
            ._Norm = normals[i]
            }
        );
    }

    // indices
    {
        for (int i = 0; i < int(resolution); i++) {
            for (int j = 0; j < int(resolution); j++) {
                int first = i * (resolution + 1) + j;
                int second = first + resolution + 1;

                builder._Indices.push_back(first);
                builder._Indices.push_back(second);
                builder._Indices.push_back(first + 1);

                builder._Indices.push_back(second);
                builder._Indices.push_back(second + 1);
                builder._Indices.push_back(first + 1);
            }
        }
    }
    return builder;    
}


VertexDataBuilder VertexDataBuilder::primitiveFrame(){
    VertexDataBuilder builder{};

    float width = 0.02f;    
    builder._Vertices = {
        // x axis
        {._Pos = {0.f, -width, -width}, ._Col = {1.f,0.f,0.f,1.f}},
        {._Pos = {1.f, -width, -width}, ._Col = {1.f,0.f,0.f,1.f}},
        {._Pos = {0.f, -width, width}, ._Col = {1.f,0.f,0.f,1.f}},
        {._Pos = {1.f, -width, width}, ._Col = {1.f,0.f,0.f,1.f}},
        {._Pos = {0.f, width, -width}, ._Col = {1.f,0.f,0.f,1.f}},
        {._Pos = {1.f, width, -width}, ._Col = {1.f,0.f,0.f,1.f}},
        {._Pos = {0.f, width, width}, ._Col = {1.f,0.f,0.f,1.f}},
        {._Pos = {1.f, width, width}, ._Col = {1.f,0.f,0.f,1.f}},
        
        // y axis
        {._Pos = {-width, 0.f, -width}, ._Col = {0.f,1.f,0.f,1.f}},
        {._Pos = {width, 0.f, -width}, ._Col = {0.f,1.f,0.f,1.f}},
        {._Pos = {-width, 1.f, -width}, ._Col = {0.f,1.f,0.f,1.f}},
        {._Pos = {width, 1.f, -width}, ._Col = {0.f,1.f,0.f,1.f}},
        {._Pos = {-width, 0.f, width}, ._Col = {0.f,1.f,0.f,1.f}},
        {._Pos = {width, 0.f, width}, ._Col = {0.f,1.f,0.f,1.f}},
        {._Pos = {-width, 1.f, width}, ._Col = {0.f,1.f,0.f,1.f}},
        {._Pos = {width, 1.f, width}, ._Col = {0.f,1.f,0.f,1.f}},

        // z axis
        {._Pos = {-width, -width, 0.f}, ._Col = {0.f,0.f,1.f,1.f}},
        {._Pos = {width, -width, 0.f}, ._Col = {0.f,0.f,1.f,1.f}},
        {._Pos = {-width, -width, 1.f}, ._Col = {0.f,0.f,1.f,1.f}},
        {._Pos = {width, -width, 1.f}, ._Col = {0.f,0.f,1.f,1.f}},
        {._Pos = {-width, width, 0.f}, ._Col = {0.f,0.f,1.f,1.f}},
        {._Pos = {width, width, 0.f}, ._Col = {0.f,0.f,1.f,1.f}},
        {._Pos = {-width, width, 1.f}, ._Col = {0.f,0.f,1.f,1.f}},
        {._Pos = {width, width, 1.f}, ._Col = {0.f,0.f,1.f,1.f}}
    };

    builder._Indices = {
        // x-axis faces
        0, 1, 2,
        1, 3, 2,
        4, 5, 6,
        5, 7, 6,
        4, 0, 6,
        0, 2, 6,
        1, 5, 3,
        5, 7, 3,
        2, 3, 6,
        3, 7, 6,
        0, 4, 1,
        4, 5, 1,
        
        // y-axis faces
        8, 10, 9,
        10, 11, 9,
        12, 13, 14,
        13, 15, 14,
        8, 12, 9,
        12, 13, 9,
        10, 14, 11,
        14, 15, 11,
        9, 13, 11,
        13, 15, 11,
        8, 10, 12,
        10, 14, 12,
        
        // z-axis faces
        16, 17, 18,
        17, 19, 18,
        20, 21, 22,
        21, 23, 22,
        16, 20, 17,
        20, 21, 17,
        18, 19, 22,
        19, 23, 22,
        17, 21, 19,
        21, 23, 19,
        16, 18, 20,
        18, 22, 20
    };

    return builder;
}

const std::map<std::string, Model::ModelExtension> Model::MODEL_EXTENSIONS_MAP = {
    {"off", OFF},
    {"obj", OBJ},
};

void Model::bind(VkCommandBuffer commandBuffer){
    VkBuffer buffers[] = {_VertexBuffer->getBuffer()};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
    if(_HasIndexBuffer){
        vkCmdBindIndexBuffer(commandBuffer, _IndexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
    }
}

void Model::draw(VkCommandBuffer commandBuffer){
    if(_HasIndexBuffer){
        vkCmdDrawIndexed(commandBuffer, _IndexCount, 1, 0, 0, 0);
    } else {
        vkCmdDraw(commandBuffer, _VertexCount, 1, 0, 0);
    }
}

void Model::cleanUp(){
    if(_VertexBuffer != nullptr){
        _VertexBuffer->cleanUp();
        _VertexBuffer = nullptr;
    }
    if(_IndexBuffer != nullptr){
        _IndexBuffer->cleanUp();
        _IndexBuffer = nullptr;
    }
}

Model::Model(VulkanAppPtr vulkanApp, const VertexDataBuilder& dataBuilder) 
    : _VulkanApp(vulkanApp){
    createVertexBuffer(dataBuilder._Vertices);
    createIndexBuffer(dataBuilder._Indices);
}

Model::Model(VulkanAppPtr vulkanApp, const std::string& filePath)
    : _VulkanApp(vulkanApp){
    // check extension type
    size_t dotPosition = filePath.find_last_of(".");
    std::string extension = "";
    if(dotPosition != std::string::npos 
        && dotPosition != filePath.length() - 1){
        extension = filePath.substr(dotPosition + 1);
    }
    if(extension.empty()){
        ErrorHandler::handle(__FILE__, __LINE__, 
            ErrorCode::BAD_VALUE_ERROR,
            "Trying to load a model from a file without extension: " + filePath +"!\n"
        );
    }
    
    auto it = MODEL_EXTENSIONS_MAP.find(extension);
    if(it == MODEL_EXTENSIONS_MAP.end()){
        ErrorHandler::handle(__FILE__, __LINE__, 
            ErrorCode::BAD_VALUE_ERROR,
            "Trying to load a model from a file with an unkown extension: " + filePath +"!\n"
        );
    }
    ModelExtension extensionFormat = it->second; 
    VertexDataBuilder builder{};
    switch(extensionFormat) {
        case OFF:
            builder.loadOffModel(filePath);
            break;
        case OBJ:
            builder.loadObjModel(filePath);
            break;
        default:
            ErrorHandler::handle(__FILE__, __LINE__, 
                ErrorCode::SYSTEM_ERROR,
                "Error creating a model from a file, this error shouldn't have occured!\n"
            );
    }
    createVertexBuffer(builder._Vertices);
    createIndexBuffer(builder._Indices);
}

};