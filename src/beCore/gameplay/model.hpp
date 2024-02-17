#pragma once

#include "vector2.hpp"
#include "vector3.hpp"
#include "vector4.hpp"
#include <map>
#include <vulkan/vulkan.hpp>
#include <cstdint>
#include <vector>
#include <memory>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "buffer.hpp"

namespace be{

/**
 * Forward definition of the model class
 * @see Model
*/
class Model;

/**
 * Shared pointer to a model
 * @see Model
*/
using ModelPtr = std::shared_ptr<Model>;

/**
 * A structure handling the data send to vertex shaders
*/
struct VertexData{
    /**
     * The vertex position
    */
    Vector3 _Pos = {0.f,0.f,0.f};

    /**
     * The vertex color
    */
    Vector4 _Col = {1.f,1.f,1.f,1.f};

    /**
     * The vertex normal
    */
    Vector3 _Norm = {0.f,0.f,0.f};

    /**
     * The vertex texture coordinates
    */
    Vector2 _Tex = {0.f,0.f};

    /**
     * The number of layout in the vertex data
    */
    static const uint32_t NB_LAYOUTS = 4;

    /**
     * A list containing all data formats
    */
    static const std::vector<VkFormat> FORMATS;

    /**
     * A list containgin all offsets sizes
    */
    static const std::vector<size_t> OFFSETS;

    /**
     * A comparison operator
     * @param other The other vertex data
     * @return True if the two vertices are equal
    */
    bool operator==(const VertexData& other) const {
        return _Pos == other._Pos 
            && _Col == other._Col 
            && _Norm == other._Norm 
            && _Tex == other._Tex;
    }

    /**
     * A builder for the binding descriptions
     * @return A list of the binding descriptions
    */
    static std::vector<VkVertexInputBindingDescription> getBindingDescriptions(){
        std::vector<VkVertexInputBindingDescription> bindingDescription(1);
        bindingDescription[0].binding = 0;
        bindingDescription[0].stride = sizeof(VertexData);
        bindingDescription[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescription;
    }

    /**
     * A builder for the attribute descriptions
     * @return A list of the attribute description for each vertex attribute
    */
    static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions(){
        std::vector<VkVertexInputAttributeDescription> attributeDescription(NB_LAYOUTS);
        for(int i=0; i<int(NB_LAYOUTS); i++){
            attributeDescription[i].binding = 0;
            attributeDescription[i].location = i;
            attributeDescription[i].format = FORMATS[i];
            attributeDescription[i].offset = OFFSETS[i];
        }
        return attributeDescription;
    }

};

/**
 * A vertex data builder
*/
struct VertexDataBuilder{
    /**
     * The list of vertices
    */
    std::vector<VertexData> _Vertices{};

    /**
     * The list of indices
    */
    std::vector<uint32_t> _Indices{};

    /**
     * A function to load a .off model
     * @param filePath The path to the model
    */
    void loadOffModel(const std::string& filePath);

    /**
     * A function to load a .obj model
     * @param filePath The path to the model
     * @note This function depends on the tinyObjectLoader library
    */
    void loadObjModel(const std::string& filePath);

    /**
     * A function to create a triangle model
     * @param v0 The first vertex position
     * @param v1 The second vertex position
     * @param v2 The third vertex position
     * @param c The uniform color of the triangle
     * @return A builder
    */
    static VertexDataBuilder primitiveTriangle(
        const Vector3& v0,
        const Vector3& v1,
        const Vector3& v2,
        const Vector4& c = Vector4{1.f,1.f,1.f,1.f}
    );

    /**
     * A function to create a rectangle model
     * @param height The rectangle height
     * @param width The rectangle width
     * @param c The uniform color of the rectangle
     * @return A builder
    */
    static VertexDataBuilder primitiveRectangle(
        float height,
        float width,
        const Vector4& c = Vector4{1.f,1.f,1.f,1.f}
    );

    /**
     * A function to create a cube model
     * @param length The edges length
     * @param c The uniform color of the cube
     * @return A builder
    */
    static VertexDataBuilder primitiveCube(
        float length,
        const Vector4& c = Vector4{1.f,1.f,1.f,1.f}
    );

    /**
     * A function to create a sphere model
     * @param resolution The sphere's resolution
     * @param c The uniform color of the sphere
     * @return A builder
    */
    static VertexDataBuilder primitiveSphere(
        size_t resolution = 16,
        const Vector4& c = Vector4{1.f,1.f,1.f,1.f}
    );

    /**
     * A function to create a frame model
     * @return A builder
    */
    static VertexDataBuilder primitiveFrame();
};


/**
 * A class to represent a 3D model
*/
class Model{

    private:
        /**
         * The supported extensions
        */
        enum ModelExtension{
            OFF,
            OBJ,
        };

        static const std::map<std::string, ModelExtension> MODEL_EXTENSIONS_MAP;

    public:
        static const uint32_t MIN_VERTEX_COUNT = 3;

    private:
        VulkanAppPtr _VulkanApp = nullptr; 

        BufferPtr _VertexBuffer = nullptr;
        uint32_t _VertexCount = 0;

        bool _HasIndexBuffer = false;
        BufferPtr _IndexBuffer = nullptr;
        uint32_t _IndexCount = 0;

    public:
        Model(VulkanAppPtr vulkanApp, const VertexDataBuilder& dataBuilder) 
            : _VulkanApp(vulkanApp){
            createVertexBuffer(dataBuilder._Vertices);
            createIndexBuffer(dataBuilder._Indices);
        }

        Model(VulkanAppPtr vulkanApp, const std::string& filePath);

        void bind(VkCommandBuffer commandBuffer){
            VkBuffer buffers[] = {_VertexBuffer->getBuffer()};
            VkDeviceSize offsets[] = {0};
            vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
            if(_HasIndexBuffer){
                vkCmdBindIndexBuffer(commandBuffer, _IndexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
            }
        }

        void draw(VkCommandBuffer commandBuffer){
            if(_HasIndexBuffer){
                vkCmdDrawIndexed(commandBuffer, _IndexCount, 1, 0, 0, 0);
            } else {
                vkCmdDraw(commandBuffer, _VertexCount, 1, 0, 0);
            }
        }
        
        void cleanUp(){
            if(_VertexBuffer != nullptr){
                _VertexBuffer->cleanUp();
                _VertexBuffer = nullptr;
            }
            if(_IndexBuffer != nullptr){
                _IndexBuffer->cleanUp();
                _IndexBuffer = nullptr;
            }
        }

    private:
        void createVertexBuffer(const std::vector<VertexData>& vertices);
        void createIndexBuffer(const std::vector<uint32_t>& indices);
};

};