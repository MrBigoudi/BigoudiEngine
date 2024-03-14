#pragma once

#include "be_boundingVolume.hpp"
#include "be_ubo.hpp"
#include "be_vector3.hpp"
#include <cstdint>

namespace be{

/**
 * The maximum number of point lights in a scene
*/
static const int MAX_NB_POINT_LIGHTS = 1;

/**
 * The maximum number of directional lights in a scene
*/
static const int MAX_NB_DIRECTIONAL_LIGHTS = 1;

/**
 * The maximum number of oriented lights in a scene
*/
static const int MAX_NB_ORIENTED_LIGHTS = 1024;

/**
 * An enumeration to represent the type of lights
*/
enum LightType{
    POINT_LIGHT,
    DIRECTIONAL_LIGHT,
    ORIENTED_LIGHT,
};

/**
 * An abstract structure for a light
*/
struct Light{
    /**
     * Getter for the light intensity
     * @return The intensity as a float
    */
    virtual float getIntensity() const = 0;

    /**
     * Getter for the light bounding box
     * @return the AABB of the light
     * @see AxisAlignedBoundingBox
    */
    virtual AxisAlignedBoundingBox getAABB() const = 0;

    /**
     * Convert a light into a string
     * @return A std::string
    */
    virtual std::string toString() const = 0;

    /**
     * Getter for the light bounding cone
     * @return The bounding cone
     * @see BoundingCone
    */
    virtual BoundingCone getBoundingCone() const {return {};}

    /**
     * A virtual destructor to make the class abstract
    */
    virtual ~Light(){};

};

/**
 * A structure representing a point light
 * @extends Light
*/
struct PointLight: public Light {
    /**
     * The light position
     * @note Align as 16 for Vulkan shaders
    */
    alignas(16) Vector4 _Position{};

    /**
     * The light color
     * @note Align as 16 for Vulkan shaders
    */
    alignas(16) Vector4 _Color{};

    /**
     * The light intensity
     * @note Align as 4 for Vulkan shaders
    */
    alignas(4) float _Intensity = 1.f;

    /**
     * Getter for the light intensity
     * @return The intensity as a float
    */
    float getIntensity() const override {return _Intensity;}

    /**
     * Getter for the light bounding box
     * @return the AABB of the light
     * @see AxisAlignedBoundingBox
    */
    virtual AxisAlignedBoundingBox getAABB() const override;

    /**
     * Convert a light into a string
     * @return A std::string
    */
    std::string toString() const override;
};

struct DirectionalLight: public Light{
    /**
     * The light direction
     * @note Align as 16 for Vulkan shaders
    */
    alignas(16) Vector4 _Direction{};

    /**
     * The light color
     * @note Align as 16 for Vulkan shaders
    */
    alignas(16) Vector4 _Color{};

    /**
     * The light intensity
     * @note Align as 4 for Vulkan shaders
    */
    alignas(4) float _Intensity = 1.f;

    /**
     * Getter for the light intensity
     * @return The intensity as a float
    */
    float getIntensity() const override {return _Intensity;}

    /**
     * Getter for the light bounding box
     * @return the AABB of the light
     * @see AxisAlignedBoundingBox
    */
    virtual AxisAlignedBoundingBox getAABB() const override;

    /**
     * Convert a light into a string
     * @return A std::string
    */
    std::string toString() const override;
};

struct OrientedLight: public Light{
    /**
     * The light position
     * @note Align as 16 for Vulkan shaders
    */
    alignas(16) Vector4 _Position{};

    /**
     * The light direction
     * @note Align as 16 for Vulkan shaders
    */
    alignas(16) Vector4 _Direction{};

    /**
     * The light color
     * @note Align as 16 for Vulkan shaders
    */
    alignas(16) Vector4 _Color{};

    /**
     * The light intensity
     * @note Align as 4 for Vulkan shaders
    */
    alignas(4) float _Intensity = 1.f;

    /**
     * The light opening angle
     * @note Align as 4 for Vulkan shaders
    */
    alignas(4) float _Angle = 0.f;

    /**
     * Getter for the light intensity
     * @return The intensity as a float
    */
    float getIntensity() const override {return _Intensity;}

    /**
     * Getter for the light bounding box
     * @return the AABB of the light
     * @see AxisAlignedBoundingBox
    */
    virtual AxisAlignedBoundingBox getAABB() const override;

    /**
     * Convert a light into a string
     * @return A std::string
    */
    std::string toString() const override;

    /**
     * Getter for the light bounding cone
     * @return The bounding cone
     * @see BoundingCone
    // TODO:
    */
    virtual BoundingCone getBoundingCone() const override;

};


/**
 * Smart pointer for a light
 * @see Light
*/
using LightPtr = std::shared_ptr<Light>;

/**
 * Smart pointer for a point light
 * @see PointLight
*/
using PointLightPtr = std::shared_ptr<PointLight>;

/**
 * Smart pointer for a directional light
 * @see DirectionalLight
*/
using DirectionalLightPtr = std::shared_ptr<DirectionalLight>;

/**
 * Smart pointer for an oriented light
 * @see OrientedLight
*/
using OrientedLightPtr = std::shared_ptr<OrientedLight>;

/**
 * An UBO structure for the lights
 * @extends UboData
*/
struct LightUboData: public UboData{
    /**
     * The number of point lights
     * @note Aligned as 4 for vulkan shader
    */
    alignas(4) uint32_t _NbPointLights = 0;

    /**
     * The array of point lights
     * @note Aligned as 16 for vulkan shader
     * @see PointLight
    */
    PointLight _PointLights[MAX_NB_POINT_LIGHTS];

    /**
     * The number of directional lights
     * @note Aligned as 4 for vulkan shader
    */
    alignas(4) uint32_t _NbDirectionalLights = 0;

    /**
     * The array of directional lights
     * @note Aligned as 16 for vulkan shader
     * @see DirecionalLight
    */
    DirectionalLight _DirectionalLights[MAX_NB_DIRECTIONAL_LIGHTS];

    // TODO: add oriented lights
};


/**
 * A class to contain the light ubo
 * @extends UboContainer
*/
class LightUboContainer: public UboContainer{
    private:
        /**
         * The data in the ubo
        */
        LightUboData _UboData{};

    public:
        /**
         * Initialize the container
         * @param size The size of the container
         * @param vulkanApp A pointer to the vulkan app
        */
        virtual void init(uint32_t size, VulkanAppPtr vulkanApp) override;

        /**
         * Update the container given the frameIndex
         * @param frameIndex The index of the current frame
        */
        virtual void update(uint32_t frameIndex) override;

        /**
         * Add a point light to the ubo container
         * @param pointLight The light to add
         * @note Can't add more light than MAX_NB_POINT_LIGHTS
        */
        void addPointLight(PointLightPtr pointLight);

        void addPointLight(const Vector3& position, const Vector3& color, float intensity);
        
        
        /**
         * Add a directional light to the ubo container
         * @param directionalLight The light to add
         * @note Can't add more light than MAX_NB_DIRECTIONAL_LIGHTS
        */
        void addDirectionalLight(DirectionalLightPtr directionalLight);

        /**
         * Reset the ubo
        */
        void reset();
};

/**
 * Forward declarartion of the LightCutsTree class
 * @see LightCutsTree
*/
class LightCutsTree;

/**
 * A smart pointer to the LightCutsTree class
 * @see LightCutsTree
*/
using LightCutsTreePtr = std::shared_ptr<LightCutsTree>;

/**
 * A class representing a tree of lights
 * @see https://www.cs.cornell.edu/~kb/projects/lightcuts/
*/
class LightCutsTree{

    private:
        /**
         * Forward declaration of a light node
         * @see LightNode
        */
        struct LightNode;

        /**
         * Smart pointer to a light node
         * @see LightNode
        */
        using LightNodePtr = std::shared_ptr<LightNode>;

        /**
         * A structure representing a node in the tree
        */
        struct LightNode{
            /**
             * The total intensity of the node
            */
            float _TotalIntensity = 0.f;

            /**
             * The representative light of the node
            */
            LightPtr _Representative = nullptr;

            /**
             * The type of the representative light
            */
            LightType _Type;

            /**
             * The bounding box of the node
            */
            AxisAlignedBoundingBox _AABB = {};

            /**
             * The bounding cone of the node
            */
            BoundingCone _BoundingCone = {};

            /**
             * The relative scaling for oriented lights
             // TODO:
            */
            static constexpr float RELATIVE_SCALING_ORIENTED = 0.f;

            /**
             * The relative scaling for non oriented lights
            */
            static constexpr float RELATIVE_SCALING_NOT_ORIENTED = 0.f;

            /**
             * The left child of the node
            */
            LightNodePtr _LeftChild = nullptr;

            /**
             * The right child of the node
            */
            LightNodePtr _RightChild = nullptr;

            /**
             * Cast a tree node into a string
             * @return A std::string
            */
            std::string toString() const;

            /**
             * Tells if a node is a leaf
            */
            bool isLeaf() const;

            /**
             * Get the size metric of the node
            */
            float getSizeMetric() const;


            /**
             * Selects the representative light of a cluster
             * @param leftChild The leftchild of the node
             * @param rightChild The rightchild of the node
             * @return A pointer to either leftChild or rightChild
             * @note Use intensity in original lightcuts paper, better option using stochastic lightcuts
             * @see http://www.cemyuksel.com/research/stochasticlightcuts/
            */
            static LightNodePtr selectRepresentative(LightNodePtr leftChild, LightNodePtr rightChild);

            /**
             * Create a new node from two node
             * @param leftChild The leftchild of the node
             * @param rightChild The rightchild of the node
             * @return A new node
            */
            static LightNodePtr createParent(LightNodePtr leftChild, LightNodePtr rightChild);

            /**
             * Create a leaf node given a light
             * @param light The light from 
             * @return The node
            */
            static LightNodePtr createLeafNode(LightPtr light);

            /**
             * Merge the two best nodes, add the new node in the list and remove the old ones from the list 
             * @param allNodes The list of nodes
             * @note The given list will be modified
            */
            static void mergeTwoBestNodes(std::vector<LightNodePtr>& allNodes);
        };

    private:
        /**
         * The light tree
        */
        LightNodePtr _LightsTree = nullptr;

    public:
        /**
         * An empty constructor
        */
        LightCutsTree(){};

        /**
         * A basic constructor
         * @param pointLights All the point lights to add in the tree
         * @param directionalLights All the directional lights to add in the tree
         * @param orientedLights All the oriented lights to add in the tree
        */
        LightCutsTree(
            const std::vector<PointLightPtr>& pointLights,
            const std::vector<DirectionalLightPtr>& directionalLights,
            const std::vector<OrientedLightPtr>& orientedLights
        );

    private:
        /**
         * Create point light leaves
         * @param inputLights The lights from which to create leaves
         * @param allNodes The list of node that we will increment
         * @note The allNodes list will be modified
        */
        void createLeaves(const std::vector<PointLightPtr>& inputLights, std::vector<LightNodePtr>& allNodes) const;
        
        /**
         * Create directional light leaves
         * @param inputLights The lights from which to create leaves
         * @param allNodes The list of node that we will increment
         * @note The allNodes list will be modified
        */
        void createLeaves(const std::vector<DirectionalLightPtr>& inputLights, std::vector<LightNodePtr>& allNodes) const;
        
        /**
         * Create oriented light leaves
         * @param inputLights The lights from which to create leaves
         * @param allNodes The list of node that we will increment
         * @note The allNodes list will be modified
        */
        void createLeaves(const std::vector<OrientedLightPtr>& inputLights, std::vector<LightNodePtr>& allNodes) const;
    
};

};