#pragma once

#include "be_boundingVolume.hpp"
#include "be_mathsFcts.hpp"
#include "be_ubo.hpp"
#include "be_vector3.hpp"
#include <cstdint>

namespace be{

static const int MAX_NB_POINT_LIGHTS = 1024;
static const int MAX_NB_DIRECTIONAL_LIGHTS = 1024;
static const int MAX_NB_ORIENTED_LIGHTS = 1024;

enum LightType{
    POINT_LIGHT,
    DIRECTIONAL_LIGHT,
    ORIENTED_LIGHT,
};

struct Light{
    virtual float getIntensity() const = 0;
    virtual AxisAlignedBoundingBox getAABB() const = 0;
    virtual std::string toString() const = 0;
    virtual BoundingCone getBoundingCone() const {return {};}
    virtual ~Light(){};

};

struct PointLight: public Light {
    alignas(16) Vector3 _Position{};
    alignas(16) Vector3 _Color{};
    alignas(4) float _Intensity = 1.f;

    float getIntensity() const override {return _Intensity;}
    virtual AxisAlignedBoundingBox getAABB() const override {
        return AxisAlignedBoundingBox(
            _Position.x(),
            _Position.x(),
            _Position.y(),
            _Position.y(),
            _Position.z(),
            _Position.z()
        );
    }

    std::string toString() const override {
        return "{position: " + _Position.toString() 
            + ", color: " + _Color.toString() 
            + ", intensity: " + std::to_string(_Intensity) 
            + "}";
    }
};

struct DirectionalLight: public Light{
    alignas(16) Vector3 _Direction{};
    alignas(16) Vector3 _Color{};
    alignas(4) float _Intensity = 1.f;

    float getIntensity() const override {return _Intensity;}
    virtual AxisAlignedBoundingBox getAABB() const override {
        return AxisAlignedBoundingBox(
            _Direction.x(),
            _Direction.x(),
            _Direction.y(),
            _Direction.y(),
            _Direction.z(),
            _Direction.z()
        );
    }

    std::string toString() const override {
        return "{direction: " + _Direction.toString() 
            + ", color: " + _Color.toString() 
            + ", intensity: " + std::to_string(_Intensity) 
            + "}";
    }
};

struct OrientedLight: public Light{
    alignas(16) Vector3 _Position{};
    alignas(16) Vector3 _Direction{};
    alignas(16) Vector3 _Color{};
    alignas(4) float _Intensity = 1.f;
    alignas(4) float _Angle = 0.f;

    float getIntensity() const override {return _Intensity;}
    virtual AxisAlignedBoundingBox getAABB() const override {
        return AxisAlignedBoundingBox(
            _Position.x(),
            _Position.x(),
            _Position.y(),
            _Position.y(),
            _Position.z(),
            _Position.z()
        );
    }

    std::string toString() const override {
        return "{direction: " + _Direction.toString()
            + ", position: " + _Position.toString() 
            + ", color: " + _Color.toString() 
            + ", intensity: " + std::to_string(_Intensity) 
            + ", angle: " + std::to_string(_Intensity) 
            + "}";
    }

    // TODO:
    virtual BoundingCone getBoundingCone() const override {
        return {};
    };
};


using LightPtr = std::shared_ptr<Light>;
using PointLightPtr = std::shared_ptr<PointLight>;
using DirectionalLightPtr = std::shared_ptr<DirectionalLight>;
using OrientedLightPtr = std::shared_ptr<OrientedLight>;


struct LightUboData: public UboData{
    alignas(4) uint32_t _NbPointLights = 0;
    alignas(16) PointLight _PointLights[MAX_NB_POINT_LIGHTS];
    alignas(4) uint32_t _NbDirectionalLights = 0;
    alignas(16) DirectionalLight _DirectionalLights[MAX_NB_DIRECTIONAL_LIGHTS];
    // TODO: add oriented lights
};

class LightUboContainer: public UboContainer{
    private:
        LightUboData _UboData{};

    public:
        virtual void init(uint32_t size, VulkanAppPtr vulkanApp) override;
        virtual void update(uint32_t frameIndex) override;

        void addPointLight(const PointLight& pointLight);
        void addPointLight(const Vector3& position, const Vector3& color, float intensity);
        void addDirectionalLight(const DirectionalLight& directionalLight);
        void addDirectionalLight(const Vector3& direction, const Vector3& color, float intensity);
        void reset();

        std::vector<PointLight> getPointLights() const;
        std::vector<DirectionalLight> getDirectionalLights() const;
};

class LightCutsTree;
using LightCutsTreePtr = std::shared_ptr<LightCutsTree>;

class LightCutsTree{

    private:
        struct LightNode;
        using LightNodePtr = std::shared_ptr<LightNode>;
        struct LightNode{
            float _TotalIntensity = 0.f;
            LightPtr _Representative;
            LightType _Type;

            AxisAlignedBoundingBox _AABB = {};
            BoundingCone _BoundingCone = {};
            // TODO: add relative scaling for oriented lights
            static constexpr float RELATIVE_SCALING_ORIENTED = 0.f;
            static constexpr float RELATIVE_SCALING_NOT_ORIENTED = 0.f;

            LightNodePtr _LeftChild = nullptr;
            LightNodePtr _RightChild = nullptr;

            std::string toString() const {
                if(isLeaf()){
                    return "{ "+ _Representative->toString() + " }";
                }
                return "{\n " 
                    + _Representative->toString()
                    + "\n\tleft: " + _LeftChild->toString() 
                    + "\n\tright:" + _RightChild->toString()
                    + "\n}";
            }

            bool isLeaf() const {
                return _LeftChild == nullptr && _RightChild == nullptr;
            }

            float getSizeMetric() const{
                float i_c = _TotalIntensity;
                float alpha_c = _AABB.getDiagonalLength();
                float beta_c = _BoundingCone.getHalfAngle();
                float c = _Type == ORIENTED_LIGHT ? RELATIVE_SCALING_ORIENTED : RELATIVE_SCALING_NOT_ORIENTED;

                return (i_c*i_c)*(alpha_c*alpha_c + c*c*(1-cos(beta_c))*(1-cos(beta_c)));
            }

            static float compareCluster(LightNodePtr n1, LightNodePtr n2){
                if(n1 == nullptr || n2 == nullptr){
                    ErrorHandler::handle(
                        __FILE__, __LINE__, ErrorCode::NOT_INITIALIZED_ERROR,
                        "Can't compare null light-tree nodes!\n"
                    );
                }
                return n1->getSizeMetric() < n2->getSizeMetric();
            }

            // Random for original lightcuts, better for stochastic lightcuts
            static LightNodePtr selectRepresentative(LightNodePtr leftChild, LightNodePtr rightChild){
                if(leftChild == nullptr || rightChild == nullptr){
                    ErrorHandler::handle(
                        __FILE__, __LINE__, ErrorCode::NOT_INITIALIZED_ERROR,
                        "Can't select a light-tree representative from null children!\n"
                    );
                }
                float i1 = leftChild->_Representative->getIntensity();
                float i2 = rightChild->_Representative->getIntensity();
                float proba_i1 = i1 / (i1+i2);

                return (Maths::random_float(0.f, 1.f) < proba_i1) ? leftChild : rightChild; 
            }

            static LightNodePtr createParent(LightNodePtr leftChild, LightNodePtr rightChild){
                if(leftChild == nullptr || rightChild == nullptr){
                    ErrorHandler::handle(
                        __FILE__, __LINE__, ErrorCode::NOT_INITIALIZED_ERROR,
                        "Can't create a light-tree node from null children!\n"
                    );
                }
                // create the new node
                LightNodePtr newNode = LightNodePtr(new LightNode());
                newNode->_TotalIntensity = leftChild->_TotalIntensity + rightChild->_TotalIntensity;
                newNode->_AABB = AxisAlignedBoundingBox::merge(leftChild->_AABB, rightChild->_AABB);
                newNode->_BoundingCone = BoundingCone::merge(leftChild->_BoundingCone, rightChild->_BoundingCone);
                // set the children
                newNode->_LeftChild = leftChild;
                newNode->_RightChild = rightChild;
                // select representative light
                LightNodePtr representativeNode = selectRepresentative(leftChild, rightChild);
                newNode->_Representative = representativeNode->_Representative;
                newNode->_Type = representativeNode->_Type;
                return newNode;
            }

            static LightNodePtr createNode(LightPtr light){
                LightNodePtr newNode = LightNodePtr(new LightNode());
                newNode->_Representative = light;
                newNode->_TotalIntensity = light->getIntensity();
                newNode->_AABB = light->getAABB();
                newNode->_BoundingCone = light->getBoundingCone();
                return newNode;
            }

            /**
             * Merge the two best nodes, add the new node in the list and remove the old ones from the list 
            */
            static void mergeTwoBestNodes(std::vector<LightNodePtr>& allNodes){
                // find two best node to merge
                LightNodePtr leftChild = nullptr;
                LightNodePtr rightChild = nullptr;
                float minClusterMetric = INFINITY;
                for(auto n1 : allNodes){
                    for(auto n2 : allNodes){
                        if(n1 == n2) continue; // can't use same child
                        float curClusterMetric = compareCluster(n1, n2);
                        if(curClusterMetric < minClusterMetric){
                            leftChild = n1;
                            rightChild = n2;
                        }
                    }
                }

                // create a new node
                LightNodePtr newNode = createParent(leftChild, rightChild);
                // remove two best nodes from list
                allNodes.erase(std::find(allNodes.begin(), allNodes.end(), leftChild));
                allNodes.erase(std::find(allNodes.begin(), allNodes.end(), rightChild));
                // add new node to list
                allNodes.push_back(newNode);
            }
        };

    private:
        LightNodePtr _LightsTree = nullptr;

    public:
        LightCutsTree(){};
        LightCutsTree(
            const std::vector<PointLightPtr>& pointLights,
            const std::vector<DirectionalLightPtr>& directionalLights,
            const std::vector<OrientedLightPtr>& orientedLights
        );

    private:
        void createLeaves(const std::vector<PointLightPtr>& inputLights, std::vector<LightNodePtr>& allNodes) const;
        void createLeaves(const std::vector<DirectionalLightPtr>& inputLights, std::vector<LightNodePtr>& allNodes) const;
        void createLeaves(const std::vector<OrientedLightPtr>& inputLights, std::vector<LightNodePtr>& allNodes) const;
    
};

};