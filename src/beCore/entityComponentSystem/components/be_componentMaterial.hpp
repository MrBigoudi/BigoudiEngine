#pragma once

#include "be_material.hpp"
#include "be_gameCoordinator.hpp" // IWYU pragma: keep

namespace be{

/**
 * A component structure to store game objects material
 * @see Material
 * @see Components
*/
struct ComponentMaterial{
    MaterialPtr _Material = MaterialPtr(new Material());
    uint32_t _MaterialId = 0;

    private:
        static uint32_t _NbMaterialCreated;

    public:
        uint32_t getId() const {return _MaterialId;}


    private:
        static void checkLimit();
        static ComponentMaterial create(
            uint32_t id,
            float metallic = 0.f,
            float roughness = 0.f,
            float subsurface = 0.f,
            float specular = 0.f,
            float specularTint = 0.f,
            float anisotropic = 0.f,
            float sheen = 0.f,
            float sheenTint = 0.f,
            float clearcoat = 0.f,
            float clearcoatGloss = 0.f);
        static ComponentMaterial create(uint32_t id, MaterialPtr material);

    public:
        static void add(GameObject object, 
            float metallic = 0.f,
            float roughness = 0.f,
            float subsurface = 0.f,
            float specular = 0.f,
            float specularTint = 0.f,
            float anisotropic = 0.f,
            float sheen = 0.f,
            float sheenTint = 0.f,
            float clearcoat = 0.f,
            float clearcoatGloss = 0.f);

        static void add(GameObject object, MaterialPtr material);

        static void add(GameObject object, GameObject objectWithMaterialToCopy);

};

}