#pragma once

#include "frameInfo.hpp"
#include "gameObject.hpp"
#include "pipeline.hpp"
#include "vulkanApp.hpp"

namespace be{

/**
 * Forward declaration of the IRenderSubSystem interface
 * @see IRenderSubSystem
*/
class IRenderSubSystem;

/**
 * Smart pointer to a render sub sytstem
*/
using IRenderSubSystemPtr = std::shared_ptr<IRenderSubSystem>;

/**
 * @brief An interface to represent a render sub system
 * This interface should be implemented for every type of rendering the user want (color pass through, lambert, ...)
*/
class IRenderSubSystem{

    protected:
        /**
         * A smart pointer to a vulkan application
         * @see VulkanApp
        */
        VulkanAppPtr _VulkanApp = nullptr;

        /**
         * A smart pointer to a graphics pipeline
         * @see Pipeline
        */
        PipelinePtr _Pipeline = nullptr;

        /**
         * The pipline layout associated to the current pipeline
        */
        VkPipelineLayout _PipelineLayout = nullptr;

        /**
         * A list of descriptor sets
        */
        std::vector<VkDescriptorSet> _DescriptorSets{};

    public:
        /**
         * A basic constructor
         * @param vulkanApp A pointer to a vulkanApp
         * @param renderPass The render pass used to create the graphics pipeline
        */
        IRenderSubSystem(VulkanAppPtr vulkanApp, VkRenderPass renderPass);

        /**
         * A virtual destructor to make the class purely virtual
        */
        virtual ~IRenderSubSystem() = default;

        /**
         * A function to render game objects
         * @param framInfo The current frame
         * @see FrameInfo
        */
        virtual void renderGameObjects(FrameInfo& frameInfo) = 0;

        /**
         * A clean up function
        */
        virtual void cleanUp();

        /**
         * The function representing how an object should be rendererd
         * @param object The object to render
         * @see GameObject
         * @note This is the most important function of the interface, it describes how and when an object should be rendered
        */
        virtual void renderingFunction(GameObject object) = 0;

    public:
        /**
         * A getter to the current pipeline
         * @return The associated pipeline
        */
        PipelinePtr getPipeline() const;

        /**
         * A getter to the pipeline layout
         * @return The associated pipeline layout
        */
        VkPipelineLayout getPipelineLayout() const;

        /**
         * A getter to the descriptor sets
         * @return The associated descriptor sets
        */
        std::vector<VkDescriptorSet> getDescriptorSets() const;


    protected:
        /**
         * An initializer for the pipeline layout
        */
        virtual void initPipelineLayout() = 0;

        /**
         * An initializer for the pipeline
         * @param renderPass The render pass associated to the pipeline
        */
        virtual void initPipeline(VkRenderPass renderPass) = 0;

        /**
         * A cleaner for the pipeline layout
        */
        virtual void cleanUpPipelineLayout() = 0;

        /**
         * A cleaner for the pipeline
        */
        virtual void cleanUpPipeline();

};

};