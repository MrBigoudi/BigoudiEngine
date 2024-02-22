#include "be_pipeline.hpp"
#include "be_errorHandler.hpp"
#include "be_vulkanApp.hpp"

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <vulkan/vulkan_core.h>

#include "be_model.hpp"

namespace be{

const std::string GraphicsShader::SHADER_DIR = 
            std::string(ENGINE_DIRECTORY_PATH) 
            + std::string("/shaders/compiled/")
        ;

GraphicsShader::GraphicsShader(GraphicsShaderStage stage, const std::string& path){
    _Stage = stage;
    if(path != "")
        init(path);
}

VkPipelineShaderStageCreateInfo GraphicsShader::getShaderStageCreateInfo() const {
    VkPipelineShaderStageCreateInfo shaderStageInfo{};
    shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageInfo.module = _Module;
    shaderStageInfo.pName = "main";
    shaderStageInfo.flags = 0;
    shaderStageInfo.pSpecializationInfo = nullptr;
    shaderStageInfo.pNext = nullptr;
    switch(_Stage){
        case VERTEX_STAGE:
            shaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
            break;
        case FRAGMENT_STAGE:
            shaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
            break;
        case GEOMETRY_STAGE:
            shaderStageInfo.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
            break;
        case TESSELLATION_CONTROL_STAGE:
            shaderStageInfo.stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
            break;
        case TESSELLATION_EVALUATION_STAGE:
            shaderStageInfo.stage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
            break;
        default:
            ErrorHandler::handle(ErrorCode::UNKNOWN_VALUE_ERROR, "Unkown shader stage!\n");
            break;
    }
    return shaderStageInfo;
}

void GraphicsShader::init(const std::string& path){
    _Path = path,
    _Code = readShaderFile(path);
}

void Pipeline::createGraphicsPipeline(PipelineConfigInfo configInfo){
    if(!_VertexShader->exists() || !_FragmentShader->exists()){
        ErrorHandler::handle(
            ErrorCode::NOT_INITIALIZED_ERROR,
            "Can't create a graphics pipeline without a vertex and a fragment shader!\n"
        );
    }

    if(configInfo._PipelineLayout == VK_NULL_HANDLE){
        ErrorHandler::handle(
            ErrorCode::NOT_INITIALIZED_ERROR,
            "Can't create a graphics pipeline without a pipeline layout!\n"
        );
    }

    if(configInfo._RenderPass == VK_NULL_HANDLE){
        ErrorHandler::handle(
            ErrorCode::NOT_INITIALIZED_ERROR,
            "Can't create a graphics pipeline without a render pass!\n"
        );
    }

    // create vertex stages
    const uint32_t nbShaderStages = getNbShaderStages();
    VkPipelineShaderStageCreateInfo shaderStages[nbShaderStages];
    std::vector<VkPipelineShaderStageCreateInfo> shaderStagesVector = createShaderStages();
    for(int i=0; i<int(nbShaderStages); i++){
        shaderStages[i] = shaderStagesVector[i];
    }

    auto bindingDescriptions = VertexData::getBindingDescriptions();
    auto attributeDescriptions = VertexData::getAttributeDescriptions();

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = nbShaderStages;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &configInfo._InputAssemblyInfo;
    pipelineInfo.pViewportState = &configInfo._ViewportInfo;
    pipelineInfo.pRasterizationState = &configInfo._RasterizationInfo;
    pipelineInfo.pMultisampleState = &configInfo._MultisampleInfo;
    pipelineInfo.pDepthStencilState = &configInfo._DepthStencilInfo;
    pipelineInfo.pColorBlendState = &configInfo._ColorBlendingInfo;
    pipelineInfo.pDynamicState = &configInfo._DynamicStateInfo;
    pipelineInfo.layout = configInfo._PipelineLayout;
    pipelineInfo.renderPass = configInfo._RenderPass;
    pipelineInfo.subpass = configInfo._Subpass;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
    pipelineInfo.basePipelineIndex = -1; // Optional

    VkResult result = vkCreateGraphicsPipelines(
        _VulkanApp->getDevice(), 
        VK_NULL_HANDLE, 
        1, 
        &pipelineInfo, 
        nullptr, 
        &_GraphicsPipeline
    );
    ErrorHandler::vulkanError(result, "Failed to create graphics pipeline!\n");

}

void Pipeline::init(PipelineConfigInfo configInfo){
    createGraphicsPipeline(configInfo);
}

void Pipeline::cleanUp(){
    vkDestroyShaderModule(_VulkanApp->getDevice(), _VertexShader->getModule(), nullptr);
    vkDestroyShaderModule(_VulkanApp->getDevice(), _FragmentShader->getModule(), nullptr);
    if(_TesselationControlShader->exists()){
        vkDestroyShaderModule(_VulkanApp->getDevice(), _TesselationControlShader->getModule(), nullptr);
        vkDestroyShaderModule(_VulkanApp->getDevice(), _TesselationEvaluationShader->getModule(), nullptr);
    }
    if(_GeometryShader->exists()){
        vkDestroyShaderModule(_VulkanApp->getDevice(), _GeometryShader->getModule(), nullptr);
    }
    
    
    vkDestroyPipeline(_VulkanApp->getDevice(), _GraphicsPipeline, nullptr);
}

PipelineConfigInfo Pipeline::defaultPipelineConfigInfo(){
    PipelineConfigInfo configInfo = {};
    configInfo._InputAssemblyInfo = {};
    configInfo._InputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    configInfo._InputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    configInfo._InputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

    configInfo._ViewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    configInfo._ViewportInfo.viewportCount = 1;
    configInfo._ViewportInfo.pViewports = nullptr;
    configInfo._ViewportInfo.scissorCount = 1;
    configInfo._ViewportInfo.pScissors = nullptr;

    configInfo._RasterizationInfo = {};
    configInfo._RasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    configInfo._RasterizationInfo.depthClampEnable = VK_FALSE;
    configInfo._RasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
    configInfo._RasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
    // configInfo._RasterizationInfo.polygonMode = VK_POLYGON_MODE_LINE;
    configInfo._RasterizationInfo.lineWidth = 1.0f;
    configInfo._RasterizationInfo.cullMode = VK_CULL_MODE_NONE;
    // configInfo._RasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    configInfo._RasterizationInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    configInfo._RasterizationInfo.depthBiasEnable = VK_FALSE;
    configInfo._RasterizationInfo.depthBiasConstantFactor = 0.f; // optional
    configInfo._RasterizationInfo.depthBiasClamp = 0.f; // optional
    configInfo._RasterizationInfo.depthBiasSlopeFactor = 0.f; // optional

    configInfo._MultisampleInfo = {};
    configInfo._MultisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    configInfo._MultisampleInfo.sampleShadingEnable = VK_FALSE;
    configInfo._MultisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    configInfo._MultisampleInfo.minSampleShading = 1.0f; // Optional
    configInfo._MultisampleInfo.pSampleMask = nullptr; // Optional
    configInfo._MultisampleInfo.alphaToCoverageEnable = VK_FALSE; // Optional
    configInfo._MultisampleInfo.alphaToOneEnable = VK_FALSE; // Optional

    configInfo._ColorBlendAttachment = {};
    configInfo._ColorBlendAttachment.colorWriteMask = 
                                      VK_COLOR_COMPONENT_R_BIT 
                                    | VK_COLOR_COMPONENT_G_BIT 
                                    | VK_COLOR_COMPONENT_B_BIT 
                                    | VK_COLOR_COMPONENT_A_BIT;
    configInfo._ColorBlendAttachment.blendEnable = VK_FALSE;
    configInfo._ColorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    configInfo._ColorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    configInfo._ColorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
    configInfo._ColorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    configInfo._ColorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    configInfo._ColorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

    configInfo._ColorBlendingInfo = {};
    configInfo._ColorBlendingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    configInfo._ColorBlendingInfo.logicOpEnable = VK_FALSE;
    configInfo._ColorBlendingInfo.logicOp = VK_LOGIC_OP_COPY; // Optional
    configInfo._ColorBlendingInfo.attachmentCount = 1;
    configInfo._ColorBlendingInfo.pAttachments = &configInfo._ColorBlendAttachment;
    configInfo._ColorBlendingInfo.blendConstants[0] = 0.0f; // Optional
    configInfo._ColorBlendingInfo.blendConstants[1] = 0.0f; // Optional
    configInfo._ColorBlendingInfo.blendConstants[2] = 0.0f; // Optional
    configInfo._ColorBlendingInfo.blendConstants[3] = 0.0f; // Optional

    configInfo._DepthStencilInfo = {};
    configInfo._DepthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    configInfo._DepthStencilInfo.depthTestEnable = VK_TRUE;
    configInfo._DepthStencilInfo.depthWriteEnable = VK_TRUE;
    configInfo._DepthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
    configInfo._DepthStencilInfo.depthBoundsTestEnable = VK_FALSE;
    configInfo._DepthStencilInfo.minDepthBounds = 0.f; // optional
    configInfo._DepthStencilInfo.maxDepthBounds = 1.f; // optional
    configInfo._DepthStencilInfo.stencilTestEnable = VK_FALSE;
    configInfo._DepthStencilInfo.front = {}; // optional
    configInfo._DepthStencilInfo.back = {}; // optional

    configInfo._DynamicStateEnables = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    configInfo._DynamicStateInfo = {};
    configInfo._DynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    configInfo._DynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(configInfo._DynamicStateEnables.size());
    configInfo._DynamicStateInfo.pDynamicStates = configInfo._DynamicStateEnables.data();
    configInfo._DynamicStateInfo.flags = 0;

    return configInfo;
}

PipelineConfigInfo Pipeline::defaultWireFramePipelineConfigInfo(){
    auto configInfo = defaultPipelineConfigInfo();
    configInfo._RasterizationInfo.polygonMode = VK_POLYGON_MODE_LINE;
    return configInfo;
}


std::vector<char> GraphicsShader::readShaderFile(const std::string& filepath){
    std::ifstream file(filepath, std::ios::ate | std::ios::binary);
    if(!file.is_open()){
        ErrorHandler::handle(
            ErrorCode::IO_ERROR, 
            "Failed to open the file " + filepath + "!\n"
        );
    }
    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}

void GraphicsShader::createModule(const VulkanAppPtr vulkanApp){
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = _Code->size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(_Code->data());

    VkShaderModule shaderModule;
    VkResult result = vkCreateShaderModule(vulkanApp->getDevice(), &createInfo, nullptr, &shaderModule);
    ErrorHandler::vulkanError(result, "Failed to create shader module!\n");

    _Module = shaderModule;
}

void Pipeline::bind(VkCommandBuffer commandBuffer){
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _GraphicsPipeline);    
}  

void Pipeline::copyShaders(PipelinePtr pipeline){
    if(pipeline->_VertexShader->exists()){
        initVertexShader(pipeline->_VertexShader->getPath());
    }
    if(pipeline->_FragmentShader->exists()){
        initFragmentShader(pipeline->_FragmentShader->getPath());
    }
    if(pipeline->_GeometryShader->exists()){
        initGeometryShader(pipeline->_GeometryShader->getPath());
    }
    if(pipeline->_TesselationControlShader->exists()
        && pipeline->_TesselationEvaluationShader->exists()){
        initTessellationShaders(
            pipeline->_TesselationControlShader->getPath(),
            pipeline->_TesselationEvaluationShader->getPath()
        );
    }
}

void Pipeline::initColorPassThroughShaders(){
    initVertexShader(
        GraphicsShader::SHADER_DIR
        + std::string("passThroughColor.vert.spv")
    );
    initFragmentShader(
        GraphicsShader::SHADER_DIR
        + std::string("passThrough.frag.spv")
    );
}

void Pipeline::initNormalPassThroughShaders(){
    initVertexShader(
        GraphicsShader::SHADER_DIR
        + std::string("passThroughNormal.vert.spv")
    );
    initFragmentShader(
        GraphicsShader::SHADER_DIR
        + std::string("passThrough.frag.spv")
    );
}

void Pipeline::initLambertShaders(){
    initVertexShader(
        GraphicsShader::SHADER_DIR
        + std::string("lambertBRDF.vert.spv")
    );
    initFragmentShader(
        GraphicsShader::SHADER_DIR
        + std::string("lambertBRDF.frag.spv")
    );
}

void Pipeline::initBlinnPhongShaders(){
    initVertexShader(
        GraphicsShader::SHADER_DIR
        + std::string("blinnPhongBRDF.vert.spv")
    );
    initFragmentShader(
        GraphicsShader::SHADER_DIR
        + std::string("blinnPhongBRDF.frag.spv")
    );
}

void Pipeline::initVertexShader(const std::string& vert){
    _VertexShader = GraphicsShaderPtr(
                        new GraphicsShader(VERTEX_STAGE, vert)
                    );
    _VertexShader->createModule(_VulkanApp);
} 

void Pipeline::initFragmentShader(const std::string& frag){
    _FragmentShader = GraphicsShaderPtr(
                        new GraphicsShader(FRAGMENT_STAGE, frag)
                    );
    _FragmentShader->createModule(_VulkanApp);
} 

void Pipeline::initTessellationShaders(const std::string& cont, const std::string& eval){
    _TesselationControlShader = GraphicsShaderPtr(
                                    new GraphicsShader(TESSELLATION_CONTROL_STAGE, cont)
                                );
    _TesselationControlShader->createModule(_VulkanApp);
    _TesselationEvaluationShader = GraphicsShaderPtr(
                                        new GraphicsShader(TESSELLATION_EVALUATION_STAGE, eval)
                                    );
    _TesselationEvaluationShader->createModule(_VulkanApp);
}

void Pipeline::initGeometryShader(const std::string& geom){
    _GeometryShader = GraphicsShaderPtr(
                        new GraphicsShader(GEOMETRY_STAGE, geom)
                    );
    _GeometryShader->createModule(_VulkanApp);
}

std::vector<VkPipelineShaderStageCreateInfo> Pipeline::createShaderStages() const {
    std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
    shaderStages.push_back(_VertexShader->getShaderStageCreateInfo());
    shaderStages.push_back(_FragmentShader->getShaderStageCreateInfo());
    if(_TesselationControlShader->exists()){
        shaderStages.push_back(_TesselationControlShader->getShaderStageCreateInfo());
        shaderStages.push_back(_TesselationEvaluationShader->getShaderStageCreateInfo());
    }
    if(_GeometryShader->exists()){
        shaderStages.push_back(_GeometryShader->getShaderStageCreateInfo());
    }
    return shaderStages;
};

uint32_t Pipeline::getNbShaderStages() const {
    return _VertexShader->exists()
        + _FragmentShader->exists()
        + _GeometryShader->exists()
        + _TesselationControlShader->exists()
        + _TesselationEvaluationShader->exists();
}

};