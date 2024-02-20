#include "be_componentMaterial.hpp"
#include "be_errorHandler.hpp"

namespace be{

const std::array<float, ComponentMaterial::COMPONENT_MATERIAL_NB_ELEMENTS> 
    ComponentMaterial::COMPONENT_MATERIAL_MIN_VALUES{
        0.f, // metallic
        0.f, // subsurface
        0.f, // specular
        0.f, // roughness
        0.f, // specularTint
        0.f, // anisotropic
        0.f, // sheen
        0.f, // sheenTint
        0.f, // clearcoat
        0.f, // clearcoatGloss
};

const std::array<float, ComponentMaterial::COMPONENT_MATERIAL_NB_ELEMENTS> 
    ComponentMaterial::COMPONENT_MATERIAL_MAX_VALUES{
        1.f, // metallic
        1.f, // subsurface
        1.f, // specular
        1.f, // roughness
        1.f, // specularTint
        1.f, // anisotropic
        1.f, // sheen
        1.f, // sheenTint
        1.f, // clearcoat
        1.f, // clearcoatGloss
};

void ComponentMaterial::check() const {
    uint32_t i = 0;

    if(_Metallic < COMPONENT_MATERIAL_MIN_VALUES[i]
        && _Metallic > COMPONENT_MATERIAL_MAX_VALUES[i]
    ){
        ErrorHandler::handle(
            ErrorCode::BAD_VALUE_ERROR,
            "Wrong material mettalicness: " 
            + std::to_string(_Metallic) 
            + "!\n"
        );
    }
    i++;

    if(_Subsurface < COMPONENT_MATERIAL_MIN_VALUES[i]
        && _Subsurface > COMPONENT_MATERIAL_MAX_VALUES[i]
    ){
        ErrorHandler::handle(
            ErrorCode::BAD_VALUE_ERROR,
            "Wrong material subsurface: " 
            + std::to_string(_Subsurface) 
            + "!\n"
        );
    }
    i++;

    if(_Specular < COMPONENT_MATERIAL_MIN_VALUES[i]
        && _Specular > COMPONENT_MATERIAL_MAX_VALUES[i]
    ){
        ErrorHandler::handle(
            ErrorCode::BAD_VALUE_ERROR,
            "Wrong material specular: " 
            + std::to_string(_Specular) 
            + "!\n"
        );
    }
    i++;

    if(_Roughness < COMPONENT_MATERIAL_MIN_VALUES[i]
        && _Roughness > COMPONENT_MATERIAL_MAX_VALUES[i]
    ){
        ErrorHandler::handle(
            ErrorCode::BAD_VALUE_ERROR,
            "Wrong material roughness: " 
            + std::to_string(_Roughness) 
            + "!\n"
        );
    }
    i++;

    if(_SpecularTint < COMPONENT_MATERIAL_MIN_VALUES[i]
        && _SpecularTint > COMPONENT_MATERIAL_MAX_VALUES[i]
    ){
        ErrorHandler::handle(
            ErrorCode::BAD_VALUE_ERROR,
            "Wrong material specular tint: " 
            + std::to_string(_SpecularTint) 
            + "!\n"
        );
    }
    i++;

    if(_Anisotropic < COMPONENT_MATERIAL_MIN_VALUES[i]
        && _Anisotropic > COMPONENT_MATERIAL_MAX_VALUES[i]
    ){
        ErrorHandler::handle(
            ErrorCode::BAD_VALUE_ERROR,
            "Wrong material anisotropy: " 
            + std::to_string(_Anisotropic) 
            + "!\n"
        );
    }
    i++;

    if(_Sheen < COMPONENT_MATERIAL_MIN_VALUES[i]
        && _Sheen > COMPONENT_MATERIAL_MAX_VALUES[i]
    ){
        ErrorHandler::handle(
            ErrorCode::BAD_VALUE_ERROR,
            "Wrong material sheen: " 
            + std::to_string(_Sheen) 
            + "!\n"
        );
    }
    i++;

    if(_SheenTint < COMPONENT_MATERIAL_MIN_VALUES[i]
        && _SheenTint > COMPONENT_MATERIAL_MAX_VALUES[i]
    ){
        ErrorHandler::handle(
            ErrorCode::BAD_VALUE_ERROR,
            "Wrong material sheen tint: " 
            + std::to_string(_SheenTint) 
            + "!\n"
        );
    }
    i++;

    if(_Clearcoat < COMPONENT_MATERIAL_MIN_VALUES[i]
        && _Clearcoat > COMPONENT_MATERIAL_MAX_VALUES[i]
    ){
        ErrorHandler::handle(
            ErrorCode::BAD_VALUE_ERROR,
            "Wrong material clearcoat: " 
            + std::to_string(_Clearcoat) 
            + "!\n"
        );
    }
    i++;

    if(_ClearcoatGloss < COMPONENT_MATERIAL_MIN_VALUES[i]
        && _ClearcoatGloss > COMPONENT_MATERIAL_MAX_VALUES[i]
    ){
        ErrorHandler::handle(
            ErrorCode::BAD_VALUE_ERROR,
            "Wrong material clearcoat gloss: " 
            + std::to_string(_ClearcoatGloss) 
            + "!\n"
        );
    }
    i++;

}

};