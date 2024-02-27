#pragma once

#include "be_ubo.hpp"
#include "be_vector3.hpp"
#include <cstdint>

namespace be{

static const int MAX_NB_POINT_LIGHTS = 1024;
static const int MAX_NB_DIRECTIONAL_LIGHTS = 1;

struct Light{};

struct PointLight: public Light {
    alignas(16) Vector3 _Position{};
    alignas(16) Vector3 _Color{};
    alignas(4) float _Intensity = 1.f;
};

struct DirectionalLight: public Light{
    alignas(16) Vector3 _Direction{};
    alignas(16) Vector3 _Color{};
    alignas(4) float _Intensity = 1.f;
};

struct LightUboData: public UboData{
    alignas(4) uint32_t _NbPointLights = 0;
    alignas(16) PointLight _PointLights[MAX_NB_POINT_LIGHTS];
    alignas(4) uint32_t _NbDirectionalLights = 0;
    alignas(16) DirectionalLight _DirectionalLights[MAX_NB_DIRECTIONAL_LIGHTS];
};

class LightUboContainer: public UboContainer{
    private:
        LightUboData _UboData{};

    public:
        virtual void init(uint32_t size, VulkanAppPtr vulkanApp) override;
        virtual void update(uint32_t frameIndex) override;

        void addPointLight(const Vector3& position, const Vector3& color, float intensity);
        void addDirectionalLight(const Vector3& direction, const Vector3& color, float intensity);
        void reset();
};

};