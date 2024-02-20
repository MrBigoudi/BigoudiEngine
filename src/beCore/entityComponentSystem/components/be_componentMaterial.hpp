#pragma once

#include <array>
#include <cstdint>

namespace be{

/**
 * A component structure to store game objects material
 * @see Components
 * @note This structure is based on disney's brdfs implementation
 * @see https://github.com/wdas/brdf/blob/main/src/brdfs/disney.brdf
*/
struct ComponentMaterial{
    /**
     * The metallicness value
     * @note default to 0
     * @note minimum 0
     * @note maximum 1
    */
    float _Metallic = 0.f;

    /**
     * The subsurface value
     * @note default to 0
     * @note minimum 0
     * @note maximum 1
    */
    float _Subsurface = 0.f;

    /**
     * The specular value
     * @note default to 0.5
     * @note minimum 0
     * @note maximum 1
    */
    float _Specular = .5f;

    /**
     * The roughness value
     * @note default to 0.5
     * @note minimum 0
     * @note maximum 1
    */
    float _Roughness = .5f;

    /**
     * The specular tint
     * @note default to 0
     * @note minimum 0
     * @note maximum 1
    */
    float _SpecularTint = 0.f;

    /**
     * The anisotropy
     * @note default to 0
     * @note minimum 0
     * @note maximum 1
    */
    float _Anisotropic = 0.f;

    /**
     * The sheen value
     * @note default to 0
     * @note minimum 0
     * @note maximum 1
    */
    float _Sheen = 0.f;

    /**
     * The sheen tint
     * @note default to 0.5
     * @note minimum 0
     * @note maximum 1
    */
    float _SheenTint = .5f;

    /**
     * The clearcoat value
     * @note default to 0
     * @note minimum 0
     * @note maximum 1
    */
    float _Clearcoat = 0.f;

    /**
     * The clearcoat gloss
     * @note default to 1
     * @note minimum 0
     * @note maximum 1
    */
    float _ClearcoatGloss = 1.f;


    // EXTREMUM VALUES
    /**
     * The number of material elements
    */
    static const uint32_t COMPONENT_MATERIAL_NB_ELEMENTS = 10;

    /**
     * The minimum value for each element of the material
    */
    static const std::array<float, COMPONENT_MATERIAL_NB_ELEMENTS> COMPONENT_MATERIAL_MIN_VALUES;

    /**
     * The maximum value for each element of the material
    */
    static const std::array<float, COMPONENT_MATERIAL_NB_ELEMENTS> COMPONENT_MATERIAL_MAX_VALUES;

    /**
     * Check if all values are correct
    */
    void check() const;
};

}