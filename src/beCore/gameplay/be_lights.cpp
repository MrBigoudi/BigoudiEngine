#include "be_lights.hpp"

#include "be_errorHandler.hpp"
#include "be_mathsFcts.hpp"

#include <algorithm>

namespace be{


/**
 * Getter for the light bounding box
 * @return the AABB of the light
 * @see AxisAlignedBoundingBox
*/
AxisAlignedBoundingBox PointLight::getAABB() const {
    return AxisAlignedBoundingBox(
        _Position.x(),
        _Position.x(),
        _Position.y(),
        _Position.y(),
        _Position.z(),
        _Position.z()
    );
}

/**
 * Convert a light into a string
 * @return A std::string
*/
std::string PointLight::toString() const {
    return "{position: " + _Position.toString() 
        + ", color: " + _Color.toString() 
        + ", intensity: " + std::to_string(_Intensity) 
        + "}";
}


/**
 * Getter for the light bounding box
 * @return the AABB of the light
 * @see AxisAlignedBoundingBox
*/
AxisAlignedBoundingBox DirectionalLight::getAABB() const {
    return AxisAlignedBoundingBox(
        -0.5f, 0.5f,
        -0.5f, 0.5f,
        -0.5f, 0.5f
    );
}

/**
 * Convert a light into a string
 * @return A std::string
*/
std::string DirectionalLight::toString() const {
    return "{direction: " + _Direction.toString() 
        + ", color: " + _Color.toString() 
        + ", intensity: " + std::to_string(_Intensity) 
        + "}";
}

/**
 * Getter for the light bounding box
 * @return the AABB of the light
 * @see AxisAlignedBoundingBox
*/
AxisAlignedBoundingBox OrientedLight::getAABB() const {
    return AxisAlignedBoundingBox(
        _Position.x(),
        _Position.x(),
        _Position.y(),
        _Position.y(),
        _Position.z(),
        _Position.z()
    );
}

/**
 * Convert a light into a string
 * @return A std::string
*/
std::string OrientedLight::toString() const {
    return "{direction: " + _Direction.toString()
        + ", position: " + _Position.toString() 
        + ", color: " + _Color.toString() 
        + ", intensity: " + std::to_string(_Intensity) 
        + ", angle: " + std::to_string(_Intensity) 
        + "}";
}

/**
 * Getter for the light bounding cone
 * @return The bounding cone
 * @see BoundingCone
 // TODO:
*/
BoundingCone OrientedLight::getBoundingCone() const {
    return {};
};


/**
 * Update the container given the frameIndex
 * @param frameIndex The index of the current frame
*/
void LightUboContainer::update(uint32_t frameIndex){
    checkFrameIndex(frameIndex);
    _Ubos[frameIndex]->writeToBuffer(&_UboData);
}

/**
 * Initialize the container
 * @param size The size of the container
 * @param vulkanApp A pointer to the vulkan app
*/
void LightUboContainer::init(uint32_t size, VulkanAppPtr vulkanApp){
    UboContainer::init(size, vulkanApp);
    _UboData = LightUboData{};
    for(uint32_t i = 0; i<_Size; i++){
        _Ubos[i] = be::BufferPtr(new be::Buffer(
            vulkanApp, 
            sizeof(LightUboData),
            1,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            vulkanApp->getProperties().limits.minUniformBufferOffsetAlignment
        ));
        _Ubos[i]->map();
    }
}

/**
 * Reset the ubo
*/
void LightUboContainer::reset(){
    _UboData._NbPointLights = 0;
    _UboData._NbDirectionalLights = 0;
}

/**
 * Add a point light to the ubo container
 * @param pointLight The light to add
 * @note Can't add more light than MAX_NB_POINT_LIGHTS
*/
void LightUboContainer::addPointLight(PointLightPtr pointLight){
    if(_UboData._NbPointLights >= MAX_NB_POINT_LIGHTS){
        ErrorHandler::handle(__FILE__, __LINE__, 
            ErrorCode::OUT_OF_RANGE_ERROR,
            "Can't add more point light in the UBO!\n",
            ErrorLevel::WARNING
        );
        return;
    }
    _UboData._PointLights[_UboData._NbPointLights] = *pointLight;
    _UboData._NbPointLights++;
}

void LightUboContainer::addPointLight(const Vector3& position, const Vector3& color, float intensity){
    if(_UboData._NbPointLights >= MAX_NB_POINT_LIGHTS){
        ErrorHandler::handle(__FILE__, __LINE__, 
            ErrorCode::OUT_OF_RANGE_ERROR,
            "Can't add more point light in the UBO!\n",
            ErrorLevel::WARNING
        );
        return;
    }
    PointLight newPointLight{};
    newPointLight._Color = Vector4(color, 1.f);
    newPointLight._Intensity = intensity;
    newPointLight._Position = Vector4(position, 1.f);
    // fprintf(stdout, "new light: %s\n", newPointLight.toString().c_str());
    _UboData._PointLights[_UboData._NbPointLights] = newPointLight;
    _UboData._NbPointLights++;
}

/**
 * Add a directional light to the ubo container
 * @param directionalLight The light to add
 * @note Can't add more light than MAX_NB_DIRECTIONAL_LIGHTS
*/
void LightUboContainer::addDirectionalLight(DirectionalLightPtr directionalLight){
    if(_UboData._NbDirectionalLights >= MAX_NB_DIRECTIONAL_LIGHTS){
        ErrorHandler::handle(__FILE__, __LINE__, 
            ErrorCode::OUT_OF_RANGE_ERROR,
            "Can't add more directional light in the UBO!\n",
            ErrorLevel::WARNING
        );
        return;
    }
    _UboData._DirectionalLights[_UboData._NbDirectionalLights] = *directionalLight;
    _UboData._NbDirectionalLights++;
}



};


// Lightcut tree
namespace be{

/**
 * Cast a tree node into a string
 * @return A std::string
*/
std::string LightCutsTree::LightNode::toString() const {
    if(isLeaf()){
        return "{ repr: "+ _Representative->toString() 
                + ", intensity: " + std::to_string(_TotalIntensity) 
                + ", color: " + _TotalColor.toString()
                + ", aabb: " + _AABB.toString()
                + " }";
    }
    return "{\n repr: " 
        + _Representative->toString()
        + ", intensity: " + std::to_string(_TotalIntensity) 
        + ", color: " + _TotalColor.toString()
        + ", aabb: " + _AABB.toString()
        + "\n\tleft: " + _LeftChild->toString() 
        + "\n\tright:" + _RightChild->toString()
        + "\n}";
}

/**
 * Tells if a node is a leaf
*/
bool LightCutsTree::LightNode::isLeaf() const {
    return _LeftChild == nullptr && _RightChild == nullptr;
}

/**
 * Get the size metric of the node
*/
float LightCutsTree::LightNode::getSizeMetric() const{
    float i_c = _TotalIntensity;
    float alpha_c = _AABB.getDiagonalLength();
    float beta_c = _BoundingCone.getHalfAngle();
    float c = _Type == ORIENTED_LIGHT ? RELATIVE_SCALING_ORIENTED : RELATIVE_SCALING_NOT_ORIENTED;

    return (i_c*i_c)*(alpha_c*alpha_c + c*c*(1-cos(beta_c))*(1-cos(beta_c)));
}

/**
 * Selects the representative light of a cluster
 * @param leftChild The leftchild of the node
 * @param rightChild The rightchild of the node
 * @return A pointer to either leftChild or rightChild
 * @note Use intensity in original lightcuts paper, better option using stochastic lightcuts
 * @see http://www.cemyuksel.com/research/stochasticlightcuts/
*/
LightCutsTree::LightNodePtr LightCutsTree::LightNode::selectRepresentative(LightNodePtr leftChild, LightNodePtr rightChild){
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

/**
 * Create a new node from two node
 * @param leftChild The leftchild of the node
 * @param rightChild The rightchild of the node
 * @return A new node
*/
LightCutsTree::LightNodePtr LightCutsTree::LightNode::createParent(LightNodePtr leftChild, LightNodePtr rightChild){
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
    // node color = sum child colors
    newNode->_TotalColor = leftChild->_TotalColor + rightChild->_TotalColor;
    // set the children
    newNode->_LeftChild = leftChild;
    newNode->_RightChild = rightChild;
    // select representative light
    LightNodePtr representativeNode = selectRepresentative(leftChild, rightChild);
    if(representativeNode.get() == leftChild.get()){
        newNode->_IsLeftChildSame = true;
    }
    newNode->_Representative = representativeNode->_Representative;
    newNode->_Type = representativeNode->_Type;
    return newNode;
}

/**
 * Create a leaf node given a light
 * @param light The light from 
 * @return The node
*/
LightCutsTree::LightNodePtr LightCutsTree::LightNode::createLeafNode(LightPtr light){
    LightNodePtr newNode = LightNodePtr(new LightNode());
    newNode->_Representative = light;
    newNode->_TotalIntensity = light->getIntensity();
    newNode->_AABB = light->getAABB();
    newNode->_BoundingCone = light->getBoundingCone();
    newNode->_TotalColor = light->getColor();
    return newNode;
}

/**
 * Merge the two best nodes, add the new node in the list and remove the old ones from the list 
 * @param allNodes The list of nodes
 * @note The given list will be modified
*/
void LightCutsTree::LightNode::mergeTwoBestNodes(std::vector<LightNodePtr>& allNodes){
    // find two best node to merge
    LightNodePtr leftChild = nullptr;
    LightNodePtr rightChild = nullptr;
    LightNodePtr newNode = nullptr;
    float minClusterMetric = INFINITY;
    // test all possible new cluster
    for(auto n1 : allNodes){
        for(auto n2 : allNodes){
            if(n1 == n2) continue; // can't use same child
            // get the metric
            LightNodePtr newNodeTmp = createParent(n1, n2);
            float curClusterMetric = newNodeTmp->getSizeMetric();
            // keep the best one
            if(curClusterMetric < minClusterMetric){
                leftChild = n1;
                rightChild = n2;
                newNode = newNodeTmp;
                minClusterMetric = curClusterMetric;
            }
        }
    }

    // remove two best nodes from list
    allNodes.erase(std::find(allNodes.begin(), allNodes.end(), leftChild));
    allNodes.erase(std::find(allNodes.begin(), allNodes.end(), rightChild));
    // add new node to list
    allNodes.push_back(newNode);
}

/**
 * A basic constructor
 * @param pointLights All the point lights to add in the tree
 * @param directionalLights All the directional lights to add in the tree
 * @param orientedLights All the oriented lights to add in the tree
*/
LightCutsTree::LightCutsTree(
        const std::vector<PointLightPtr>& pointLights,
        const std::vector<DirectionalLightPtr>& directionalLights,
        const std::vector<OrientedLightPtr>& orientedLights
    ){

    // create an empty list of leaf nodes
    std::vector<LightCutsTree::LightNodePtr> allNodes = {};
    // add all the lights as leaves
    createLeaves(pointLights, allNodes);
    createLeaves(directionalLights, allNodes);
    createLeaves(orientedLights, allNodes);

    // check if there is at leas a light
    if(allNodes.size() == 0){
        ErrorHandler::handle(
            __FILE__, __LINE__,
            ErrorCode::NOT_INITIALIZED_ERROR,
            "Can't create a light-tree without lights!\n"
        );
    }

    // build the tree from bottom-up
    while(allNodes.size() != 1){
        LightCutsTree::LightNode::mergeTwoBestNodes(allNodes);
    }
    _LightsTree = allNodes[0];
    
    // display the tree if in debug mode
    #ifndef NDEBUG
    // fprintf(stdout, "tree:\n%s\n", _LightsTree->toString().c_str());
    #endif
}

/**
 * Create point light leaves
 * @param inputLights The lights from which to create leaves
 * @param allNodes The list of node that we will increment
 * @note The allNodes list will be modified
*/
void LightCutsTree::createLeaves(const std::vector<PointLightPtr>& inputLights, std::vector<LightNodePtr>& allNodes) const {
    for(auto& light : inputLights){
        LightCutsTree::LightNodePtr newNode = LightCutsTree::LightNode::createLeafNode(light);
        newNode->_Type = POINT_LIGHT;
        allNodes.push_back(newNode);
    }
}

/**
 * Create directional light leaves
 * @param inputLights The lights from which to create leaves
 * @param allNodes The list of node that we will increment
 * @note The allNodes list will be modified
*/
void LightCutsTree::createLeaves(const std::vector<DirectionalLightPtr>& inputLights, std::vector<LightNodePtr>& allNodes) const {
    for(auto& light : inputLights){
        LightCutsTree::LightNodePtr newNode = LightCutsTree::LightNode::createLeafNode(light);
        newNode->_Type = DIRECTIONAL_LIGHT;
        allNodes.push_back(newNode);
    }
}

/**
 * Create oriented light leaves
 * @param inputLights The lights from which to create leaves
 * @param allNodes The list of node that we will increment
 * @note The allNodes list will be modified
*/
void LightCutsTree::createLeaves(const std::vector<OrientedLightPtr>& inputLights, std::vector<LightNodePtr>& allNodes) const {
    for(auto& light : inputLights){
        LightCutsTree::LightNodePtr newNode = LightCutsTree::LightNode::createLeafNode(light);
        newNode->_Type = ORIENTED_LIGHT;
        allNodes.push_back(newNode);
    }
}


float LightCutsTree::LightNode::getVisibility() const {
    // all light are potentially visible
    return 1.f;
}

float LightCutsTree::LightNode::getGeometricBound(const Vector3& pointToShade) const{
    // get the closest point in the bounding box of the cluster
    Vector3 closestPoint = _AABB.getClosestPoint(pointToShade);
    // get the distance between this point and the point to shade
    float d2 = (closestPoint-pointToShade).getSquaredNorm();
    // bound the distance
    d2 = std::max(1.f, d2);
    return 1.f / d2;
}

float LightCutsTree::LightNode::getGeometric(const Vector3& pointToShade) const{
    switch(_Type){
        case POINT_LIGHT:{
            PointLightPtr pointLight = std::dynamic_pointer_cast<PointLight>(_Representative);
            Vector4 lightPos = pointLight->_Position;
            return 1.f / (lightPos.xyz() - pointToShade).getSquaredNorm();
        }
        // TODO:
        default:
            return 0.f;
    }
}


}