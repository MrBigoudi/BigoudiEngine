#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <optional>
#include <vector>
#include <vulkan/vulkan.hpp>

#include "be_vulkanApp.hpp"

namespace be{

/**
 * Forward declaration of the Pipeline class
 * @see Pipeline
*/
class Pipeline;

/**
 * Forward declaration of the Graphics shader class
 * @see GraphicsShader
*/
class GraphicsShader;

/**
 * A smart pointer to a pipeline
 * @see Pipeline
*/
using PipelinePtr = std::shared_ptr<Pipeline>;

/**
 * A smart pointer to a graphics shader
 * @see GraphicsShader
*/
using GraphicsShaderPtr = std::shared_ptr<GraphicsShader>;

/**
 * A structure used to configure a pipeline
*/
struct PipelineConfigInfo{
    /**
     * The viewport info
    */
    VkPipelineViewportStateCreateInfo _ViewportInfo{};

    /**
     * The input assembly info
    */
    VkPipelineInputAssemblyStateCreateInfo _InputAssemblyInfo{};

    /**
     * The rasterization info
    */
    VkPipelineRasterizationStateCreateInfo _RasterizationInfo{};

    /**
     * The multisample info
    */
    VkPipelineMultisampleStateCreateInfo _MultisampleInfo{};

    /**
     * The depth stencil info
    */
    VkPipelineDepthStencilStateCreateInfo _DepthStencilInfo{};

    /**
     * The color blend attachement
    */
    VkPipelineColorBlendAttachmentState _ColorBlendAttachment{};

    /**
     * The color blending info
    */
    VkPipelineColorBlendStateCreateInfo _ColorBlendingInfo{};

    /**
     * The dynamic state info
    */
    VkPipelineDynamicStateCreateInfo _DynamicStateInfo{};

    /**
     * The list of dynamic state enabled features
    */
    std::vector<VkDynamicState> _DynamicStateEnables;

    /**
     * The pipeline layout
    */
    VkPipelineLayout _PipelineLayout{};

    /**
     * The pipeline's render pass
    */
    VkRenderPass _RenderPass{};

    /**
     * The number of subpasses
    */
    uint32_t _Subpass = 0;
};

/**
 * @enum An enumeration of the graphics shader stages
*/
enum GraphicsShaderStage{
    VERTEX_STAGE,
    FRAGMENT_STAGE,
    GEOMETRY_STAGE,
    TESSELLATION_CONTROL_STAGE,
    TESSELLATION_EVALUATION_STAGE,
};

/**
 * A class representing a graphics shader
*/
class GraphicsShader{
    private:  
        /**
         * @typedef A type representing the shader code
         * @note Shader code is an option to handle the optional shader stages
        */      
        using GraphicsShaderCode = std::optional<std::vector<char>>;

        /**
         * The path to the shader file
        */
        std::string _Path = "";

        /**
         * The shader code
         * @see GraphicsShaderCode
        */
        GraphicsShaderCode _Code;

        /**
         * The shader module
        */
        VkShaderModule _Module;

        /**
         * The shader stage
         * @see GraphicsShaderStage
        */
        GraphicsShaderStage _Stage;

    public:
        /**
         * The directory containing engine default shader codes
        */
        static const std::string SHADER_DIR;

    public:
        /**
         * A basic constructor
         * @param stage The shader stage to build
         * @param path The path to the shader
        */
        GraphicsShader(GraphicsShaderStage stage, const std::string& path = "");

        /**
         * A getter to the shader module
         * @return The module
        */
        VkShaderModule getModule() const {return _Module;}

        /**
         * Create a vulkan module for the shader
         * @param vulkanApp The vulkan application
        */
        void createModule(const VulkanAppPtr vulkanApp);
        
        /**
         * A boolean to check if the shader is valid
         * @return True if the optional code contains a value
         * @see _Code
        */
        bool exists(){return _Code.has_value();}

        /**
         * A getter to the path of the current shader
         * @return The path as a string
        */
        std::string getPath() const {return _Path;}

        /**
         * Create a shader stage info
         * @return The vulkan structure
        */
        VkPipelineShaderStageCreateInfo getShaderStageCreateInfo() const;

    private:
        /**
         * An intermediate function to read a shader from a file
         * @param filePath The path to the file containing the shader
         * @return The content of the file as a vector of char
        */
        static std::vector<char> readShaderFile(const std::string& filepath);
        
        /**
         * Init a shader from a path
         * @param path The path to the file containing the shader code
        */
        void init(const std::string& path);
};


/**
 * A class representing a complete pipeline
*/
class Pipeline{

    public:
        /**
         * A builder to create a default configuration info
         * @see PipelineConfigInfo
        */
        static PipelineConfigInfo defaultPipelineConfigInfo();

        /**
         * A builder to create a default configuration info to enable wireframe mode
         * @see PipelineConfigInfo
        */
        static PipelineConfigInfo defaultWireFramePipelineConfigInfo();

    private:
        /**
         * The vertex shader
         * @see GraphicsShader
        */
        GraphicsShaderPtr _VertexShader = GraphicsShaderPtr(new GraphicsShader(VERTEX_STAGE));
        
        /**
         * The fragment shader
         * @see GraphicsShader
        */
        GraphicsShaderPtr _FragmentShader = GraphicsShaderPtr(new GraphicsShader(FRAGMENT_STAGE));
        
        /**
         * The tesselation control shader
         * @see GraphicsShader
        */
        GraphicsShaderPtr _TesselationControlShader = GraphicsShaderPtr(new GraphicsShader(TESSELLATION_CONTROL_STAGE));
        
        /**
         * The tesselation evaluation shader
         * @see GraphicsShader
        */
        GraphicsShaderPtr _TesselationEvaluationShader = GraphicsShaderPtr(new GraphicsShader(TESSELLATION_EVALUATION_STAGE));
        
        /**
         * The geometry shader
         * @see GraphicsShader
        */
        GraphicsShaderPtr _GeometryShader = GraphicsShaderPtr(new GraphicsShader(GEOMETRY_STAGE));

        /**
         * A pointer to the vulkan application
         * @see VulkanApp
        */
        VulkanAppPtr _VulkanApp = nullptr;

        /**
         * The vulkan graphics pipeline
        */
        VkPipeline _GraphicsPipeline = nullptr;

    public:
        Pipeline(VulkanAppPtr vulkanApp) : _VulkanApp(vulkanApp){
        };

        /**
         * Copy the shaders of a given pipeline
         * @param pipeline The pipeline from which to copy shaders
        */
        void copyShaders(PipelinePtr pipeline);

        /**
         * Initiate basic passthrough shaders 
         * @note Shaders return the interpolated input colors as outputs 
         * @see shaders/passThroughColor.vert
         * @see shaders/passThrough.frag
        */
        void initColorPassThroughShaders();

        /**
         * Initiate basic passthrough shaders 
         * @note Shaders return the interpolated input normals as outputs 
         * @see shaders/passThroughNormal.vert
         * @see shaders/passThrough.frag
        */
        void initNormalPassThroughShaders();

        void initRayTracingShaders();

        /**
         * Initiate shaders for the Lambert BRDF
         * @see shaders/lambertBRDF.vert
         * @see shaders/lambertBRDF.frag
        */
        void initLambertShaders();

        /**
         * Initiate shaders for the Blinn-Phong BRDF
         * @see shaders/blinnPhongBRDF.vert
         * @see shaders/blinnPhongBRDF.frag
        */
        void initBlinnPhongShaders();

        /**
         * Initiate shaders for the Microfacet BRDF
         * @see shaders/ggxBRDF.vert
         * @see shaders/ggxBRDF.frag
        */
        void initMicroFacetsShaders();

        /**
         * Initiate shaders for the Disney GGX BSDF
         * @see shaders/ggxBSDF.vert
         * @see shaders/ggxBSDF.frag
        */
        void initDisneyShaders();
        
        /**
         * Init the vertex shader given the path to its file implementation
         * @param vert The path to the vertex shader
        */
        void initVertexShader(const std::string& vert);

        /**
         * Init the fragment shader given the path to its file implementation
         * @param frag The path to the fragment shader
        */
        void initFragmentShader(const std::string& frag);

        /**
         * Init the tessellations shaders given the path to their file implementations
         * @param cont The path to the tesselation control shader
         * @param eval The path to the tesselation evaluation shader
        */
        void initTessellationShaders(const std::string& cont, const std::string& eval);

        /**
         * Init the geometry shader given the path to its file implementation
         * @param geom The path to the geometry shader
        */
        void initGeometryShader(const std::string& geom);
        
        /**
         * Init a graphics pipeline given a configuration info structure
         * @param configInfo The configuration info structure
         * @see PipelineConfigInfo
        */
        void init(PipelineConfigInfo configInfo);

        /**
         * Clean up a graphics pipeline
        */
        void cleanUp();   

        /**
         * Bind a graphics pipeline
         * @param commandBuffer The vulkan command buffer for which to bind the pipeline
        */
        void bind(VkCommandBuffer commandBuffer);     

    private:
        /**
         * An intermediate function to create the graphics pipeline
         * @param configInfo The configuration info structure
         * @see PipelineConfigInfo
        */
        void createGraphicsPipeline(PipelineConfigInfo configInfo);
        
        /**
         * An intermediate function to create the shader stages
         * @return A vector of vulkan structure
        */
        std::vector<VkPipelineShaderStageCreateInfo> createShaderStages() const;

        /**
         * Getter for the number of shader stages
         * @return The number of shader stages
        */
        uint32_t getNbShaderStages() const;
};

};