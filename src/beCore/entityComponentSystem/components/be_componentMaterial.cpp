#include "be_componentMaterial.hpp"

namespace be{

uint32_t ComponentMaterial::_NbMaterialCreated = 1; // material 0 is default material

ComponentMaterial ComponentMaterial::create(
    uint32_t id,
    float metallic,
    float roughness,
    float subsurface,
    float specular,
    float specularTint,
    float anisotropic,
    float sheen,
    float sheenTint,
    float clearcoat,
    float clearcoatGloss)
    {

    ComponentMaterial newComponent{};
    newComponent._MaterialId = id;

    newComponent._Material = MaterialPtr(new Material());
    newComponent._Material->_Metallic = metallic;
    newComponent._Material->_Roughness = roughness;
    newComponent._Material->_Subsurface = subsurface;
    newComponent._Material->_Specular = specular;
    newComponent._Material->_SpecularTint = specularTint;
    newComponent._Material->_Anisotropic = anisotropic;
    newComponent._Material->_Sheen = sheen;
    newComponent._Material->_SheenTint = sheenTint;
    newComponent._Material->_Clearcoat = clearcoat;
    newComponent._Material->_ClearcoatGloss = clearcoatGloss;

    return newComponent;
}

ComponentMaterial ComponentMaterial::create(uint32_t id, MaterialPtr material){
    ComponentMaterial newComponent{};
    newComponent._Material = material;
    newComponent._MaterialId = id;
    return newComponent;
}

void ComponentMaterial::checkLimit(){
    if(_NbMaterialCreated >= MAX_NB_MATERIALS){
        ErrorHandler::handle(
            __FILE__, __LINE__, ErrorCode::OUT_OF_RANGE_ERROR,
            "Can't create more materials: limit of " 
            + std::to_string(MAX_NB_MATERIALS)
            + "reached!\n"
        );
    }
}

void ComponentMaterial::add(GameObject object, 
    float metallic,
    float roughness,
    float subsurface,
    float specular,
    float specularTint,
    float anisotropic,
    float sheen,
    float sheenTint,
    float clearcoat,
    float clearcoatGloss)
    {
    checkLimit();
    GameCoordinator::addComponent(
        object, 
        ComponentMaterial::create(
                _NbMaterialCreated,
                metallic, roughness, subsurface, 
                specular, specularTint, anisotropic, 
                sheen, sheenTint, clearcoat, 
                clearcoatGloss
            )
    );

    _NbMaterialCreated++;
}

void ComponentMaterial::add(GameObject object, MaterialPtr material){
    checkLimit();
    GameCoordinator::addComponent(
        object, 
        ComponentMaterial::create(_NbMaterialCreated, material)
    );
    _NbMaterialCreated++;
}

void ComponentMaterial::add(GameObject object, GameObject objectWithMaterialToCopy){
    auto objectMaterial = be::GameCoordinator::getComponent<be::ComponentMaterial>(objectWithMaterialToCopy);
    GameCoordinator::addComponent(
        object, 
        ComponentMaterial::create(objectMaterial.getId(), objectMaterial._Material)
    );
}


}