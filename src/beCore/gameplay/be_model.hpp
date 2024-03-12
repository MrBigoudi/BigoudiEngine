#pragma once

#include "be_material.hpp"
#include "be_matrix4x4.hpp"
#include "be_vector2.hpp"
#include "be_vector3.hpp"
#include "be_vector4.hpp"
#include <map>
#include <vulkan/vulkan.hpp>
#include <cstdint>
#include <vector>
#include <memory>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "be_buffer.hpp"

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
     * @param c The uniform color of the triangle
     * @return A builder
    */
    static VertexDataBuilder primitiveTriangle(
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
        float height = 1.f,
        float width = 1.f,
        const Vector4& c = Vector4{1.f,1.f,1.f,1.f}
    );

    /**
     * A function to create a cube model
     * @param length The edges length
     * @param c The uniform color of the cube
     * @return A builder
    */
    static VertexDataBuilder primitiveCube(
        float length = 1.f,
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


struct Triangle{
    MaterialPtr _Material = nullptr;
    Matrix4x4 _Model = {};

    Vector3 _Pos0{};    
    Vector3 _WorldPos0{};
    Vector4 _Col0{};
    Vector3 _Norm0{};
    Vector2 _Tex0{};

    Vector3 _Pos1{};    
    Vector3 _WorldPos1{};
    Vector4 _Col1{};
    Vector3 _Norm1{};
    Vector2 _Tex1{};

    Vector3 _Pos2{};    
    Vector3 _WorldPos2{};
    Vector4 _Col2{};
    Vector3 _Norm2{};
    Vector2 _Tex2{};

    std::string toString() const {
        return
            "{\np0: " + _Pos0.toString() 
            + "\np1: " + _Pos1.toString()
            + "\np2: " + _Pos2.toString()
            + "\nwp0: " + _WorldPos0.toString()
            + "\nwp1: " + _WorldPos1.toString()
            + "\nwp2: " + _WorldPos2.toString()
            + "\nc0: " + _Col0.toString()
            + "\nc1: " + _Col1.toString()
            + "\nc2: " + _Col2.toString()
            + "\nn0: " + _Norm0.toString()
            + "\nn1: " + _Norm1.toString()
            + "\nn2: " + _Norm2.toString()
            + "\nuv0: " + _Tex0.toString()
            + "\nuv1: " + _Tex1.toString()
            + "\nuv2: " + _Tex2.toString()
            + "\n}";
    }

    bool isWorldP0LeftOfPlane(const Vector3& planePosition, const Vector3& planeNormal) const;
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

        /**
         * A map to bind string representation of the format to the model extension enum
         * @see ModelExtension
        */
        static const std::map<std::string, ModelExtension> MODEL_EXTENSIONS_MAP;

    public:
        /**
         * The minimum number of vertices required to represent a model
        */
        static const uint32_t MIN_VERTEX_COUNT = 3;

    private:
        /**
         * A smart pointer to the vulkan application
         * @see VulkanApp
        */
        VulkanAppPtr _VulkanApp = nullptr; 

        /**
         * A smart pointer to the buffer containing the vertices informations
         * @see Buffer
        */
        BufferPtr _VertexBuffer = nullptr;

        /**
         * The number of vertices in the model
        */
        uint32_t _VertexCount = 0;

        /**
         * Boolean to tell if the model has an index buffer
        */
        bool _HasIndexBuffer = false;

        /**
         * A smart pointer to the index buffer of the model
         * @see Buffer
        */
        BufferPtr _IndexBuffer = nullptr;

        /**
         * The number of indices in the model
        */
        uint32_t _IndexCount = 0;

        /**
         * The vertex builder
        */
        VertexDataBuilder _VertexDataBuilder{};

    public:
        /**
         * Build a model from a vulkan application and a vertex data builder
         * @param vulkanApp The vulkan application
         * @param dataBuilder The vertex data builder
        */
        Model(VulkanAppPtr vulkanApp, const VertexDataBuilder& dataBuilder);

        /**
         * Build a model from a vulkan application and an object file
         * @param vulkanApp The vulkan application
         * @param filePath The path to the file containing the model
         * @note The sile must have a supported extension
         * @see ModelExtension
        */
        Model(VulkanAppPtr vulkanApp, const std::string& filePath);

        /**
         * Register a bind command for the current model
         * @param commandBuffer The current command buffer
         * @see _VertexBuffer
         * @see _IndexBuffer
        */
        void bind(VkCommandBuffer commandBuffer);

        /**
         * Register a draw command for the current model
         * @param commandBuffer The current command buffer
        */
        void draw(VkCommandBuffer commandBuffer);
        
        /**
         * Cleanup the model
        */
        void cleanUp();

        /**
         * Get the list of triangles in the model
         * @return A vector of triangle
        */
        std::vector<Triangle> getTrianglePrimitives() const;

    private:
        /**
         * Fill the vertex buffer from a list of vertices
         * @param vertices The list of vertices
         * @see VertexData
         * @see _VertexBuffer
        */
        void createVertexBuffer(const std::vector<VertexData>& vertices);

        /**
         * Fill the index buffer from a list of indices
         * @param indices The list of indices
         * @see _IndexBuffer
        */
        void createIndexBuffer(const std::vector<uint32_t>& indices);
};

};