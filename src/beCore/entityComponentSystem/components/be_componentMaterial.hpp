#pragma once

#include "be_material.hpp"
#include "be_gameCoordinator.hpp"

namespace be{

/**
 * A component structure to store game objects material
 * @see Material
 * @see Components
*/
struct ComponentMaterial{
    MaterialPtr _Material = nullptr;

    static ComponentMaterial create(
        float metallic = 0.f,
        float roughness = 0.f,
        float subsurface = 0.f,
        float specular = 0.f,
        float specularTint = 0.f,
        float anisotropic = 0.f,
        float sheen = 0.f,
        float sheenTint = 0.f,
        float clearcoat = 0.f,
        float clearcoatGloss = 0.f){
        Material material = {};
        material._Metallic = metallic;
        material._Roughness = roughness;
        material._Subsurface = subsurface;
        material._Specular = specular;
        material._SpecularTint = specularTint;
        material._Anisotropic = anisotropic;
        material._Sheen = sheen;
        material._SheenTint = sheenTint;
        material._Clearcoat = clearcoat;
        material._ClearcoatGloss = clearcoatGloss;
        return {._Material = MaterialPtr(&material)};
    }

    static ComponentMaterial create(Material material){
        return {._Material = MaterialPtr(&material)};
    }

    static ComponentMaterial create(MaterialPtr material){
        return {._Material = material};
    }

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
        float clearcoatGloss = 0.f){
        GameCoordinator::addComponent(
            object, 
            create(
                    metallic, roughness, subsurface, 
                    specular, specularTint, anisotropic, 
                    sheen, sheenTint, clearcoat, 
                    clearcoatGloss
                )
        );
    }

    static void add(GameObject object, Material material){
        GameCoordinator::addComponent(
            object, 
            create(material)
        );
    }

    static void add(GameObject object, MaterialPtr material){
        GameCoordinator::addComponent(
            object, 
            create(material)
        );
    }

};

}