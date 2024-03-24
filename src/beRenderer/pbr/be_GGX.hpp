#pragma once

#include "be_lights.hpp"
#include "be_mathsFcts.hpp"
#include "be_physicsConstants.hpp"
#include "be_trigonometry.hpp"
#include "be_vector3.hpp"
#include <algorithm>
#include <cmath>

namespace be{

class GGX{
    private:
        GGX(){};

    private:
        // HELPERS
        static float getGgxDistribution(const Vector3& wh, float alpha, const Vector3& n){
            float a2 = Maths::sqr(alpha);
            float den = PI * Maths::sqr(1+(a2-1)*Maths::sqr(Maths::clamp(Vector3::dot(n, wh), 0.f, 1.f))) + EPSILON;
            return a2 / den;
        }

        static Vector3 getSchlickFresnel(Vector3 f0, Vector3 wi, Vector3 wh){
            float LoH = Maths::clamp(Vector3::dot(wi, wh), 0.f, 1.f);
            return f0 + (Vector3::ones() - f0) * std::pow(1.f - LoH, 5.f);
        }

        static float getG1Smith(Vector3 w, Vector3 objNorm, float alpha){
            float dotNW = Maths::clamp(Vector3::dot(objNorm, w), 0.f, 1.f);
            float a2 = Maths::sqr(alpha);
            float den = (dotNW + (sqrt(a2 + (1-a2) * Maths::sqr(dotNW)))) + EPSILON;
            return (2.f*dotNW) / den;
        }

        static float getGGXSmithGeometric(Vector3 wi, Vector3 wo, Vector3 objNorm, float alpha){
            return getG1Smith(wi, objNorm, alpha) * getG1Smith(wo, objNorm, alpha);
        }

        static Vector3 getDiffuse(float metallic, const Vector3& albedo){
            return (1.f - metallic) * albedo / PI;
        }

        static Vector3 getSpecular(const Vector3& wi, const Vector3& wo, const Vector3& n,  const Vector3& albedo, float roughness, float metallic){
            Vector3 wh = Vector3::normalize(wi + wo);
            float roughness2 = Maths::sqr(roughness);

            float D = getGgxDistribution(wh, roughness2, n);

            float reflectance = 1.f;
            Vector3 f0 = 0.16f * Maths::sqr(reflectance) * (1.f - metallic) * Vector3::ones() + albedo * metallic;

            Vector3 F = getSchlickFresnel(f0, wi, wh);
            float G = getGGXSmithGeometric(wi, wo, n, roughness2);

            float den = 4.f * Maths::clamp(Vector3::dot(n, wi), 0.f, 1.f) * Maths::clamp(Vector3::dot(n, wo), 0.f, 1.f) + EPSILON;
            return D*F*G / den;
        }

    
    public:
        static Vector3 BRDF(const Vector3& wi, const Vector3& wo, const Vector3& n,  const Vector3& albedo, float roughness, float metallic){
            Vector3 diffuse = getDiffuse(metallic, albedo);
            Vector3 specular = getSpecular(wi, wo, n, albedo, roughness, metallic);
            return diffuse + specular;
        }

        static Vector3 getAttenuation(PointLightPtr pointLight, const Vector3& shadePos){
            float d2 = (pointLight->_Position.xyz() - shadePos).getSquaredNorm();
            d2 += EPSILON;
            return pointLight->_Intensity * pointLight->_Color.xyz() / d2;
        }

        static Vector3 getAttenuation(DirectionalLightPtr directionalLight){
            return directionalLight->_Intensity * directionalLight->_Color.xyz();
        }

};

};