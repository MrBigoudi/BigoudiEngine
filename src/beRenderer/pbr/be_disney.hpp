#pragma once

#include "be_lights.hpp"
#include "be_mathsFcts.hpp"
#include "be_physicsConstants.hpp"
#include "be_trigonometry.hpp"
#include "be_vector3.hpp"
#include <algorithm>
#include <cmath>

namespace be{

class Disney{
    private:
        struct Input{
            Vector3 _BaseColor;
            MaterialPtr _Material;
            Vector3 _Win;
            Vector3 _Wout;
            Vector3 _H;
            Vector3 _ShadingNormal;
            Vector3 _Tangent;
            Vector3 _Bitangent;
            Vector3 _GeometricNormal;
            float _Intensity;
        };
        Disney(){}

    private:
        // HELPERS
        // Diffuse
        // f base diffuse
        static float getFD90(float roughness, const Vector3& h, const Vector3& wout){
            float hw = std::abs(Vector3::dot(h,wout));
            return 0.5f + 2.f*roughness*hw*hw;
        }
        static float getFD(float fd90, const Vector3& n, const Vector3& w){
            float nw = std::abs(Vector3::dot(n,w));
            return 1.f + (fd90 - 1.f)*(1.f - pow(nw, 5));
        }
        static Vector3 getBaseDiffuse(const Vector3& baseColor, float roughness, const Vector3& h, const Vector3& n, const Vector3& win, const Vector3& wout){
            float fd90 = getFD90(roughness, h, wout);
            float fdwin = getFD(fd90, n, win);
            float fdwout = getFD(fd90, n, wout);
            float factor = std::abs(Vector3::dot(n,wout));
            Vector3 color = baseColor / PI;
            return color * factor * fdwin * fdwout;
        }
        // f subsurface diffuse
        static float getFSS90(float roughness, const Vector3& h, const Vector3& wout){
            float hwout = std::abs(Vector3::dot(h, wout));
            return roughness * hwout * hwout;
        }
        static float getFSS(float fss90, const Vector3& n, const Vector3& w){
            float nw = std::abs(Vector3::dot(n, w));
            return 1.f + (fss90 - 1.f)*(1.f-pow(nw, 5));
        }
        static Vector3 getSubsurfaceDiffuse(const Vector3& baseColor, float roughness, const Vector3& h, const Vector3& n, const Vector3& win, const Vector3& wout){
            float fss90 = getFSS90(roughness, h, wout);
            float fsswin = getFSS(fss90, n, win);
            float fsswout = getFSS(fss90, n, wout);
            float nwin = std::abs(Vector3::dot(n, win));
            float nwout = std::abs(Vector3::dot(n, wout));
            float factor = 1.f / (nwin + nwout) - 0.5f;
            Vector3 color = 1.25f*baseColor / PI;
            return color * (fsswin * fsswout * factor + 0.5f) * nwout;
        }
        // f diffuse
        static Vector3 getDiffuse(Input i){
            Vector3 baseDiffuse = getBaseDiffuse(i._BaseColor, i._Material->_Roughness, i._H, i._ShadingNormal, i._Win, i._Wout);
            Vector3 subsurfaceDiffuse = getSubsurfaceDiffuse(i._BaseColor, i._Material->_Roughness, i._H, i._ShadingNormal, i._Win, i._Wout);
            return (1.f - i._Material->_Subsurface) * baseDiffuse + i._Material->_Subsurface * subsurfaceDiffuse;
        }


        // Clearcoat
        // Fc
        static float getR0eta(float eta){
            float num = (eta - 1)*(eta - 1);
            float den = (eta + 1)*(eta + 1);
            return num / den;
        }
        static Vector3 getFclearcoat(const Vector3& h, const Vector3& wout){
            float r0 = getR0eta(1.5f);
            float hw = pow(1.f - std::abs(Vector3::dot(h, wout)), 5);
            return r0*Vector3(1.f, 1.f, 1.f) + (1.f - r0)*hw*Vector3(1.f, 1.f, 1.f);
        }
        // Dc
        static float getAlphag(float clearcoatGloss){
            return (1.f - clearcoatGloss) * 0.1f + clearcoatGloss * 0.001;
        }
        // TODO:
        // need surface tangents to use the BSDF 
        static float getDclearcoat(float clearcoatGloss, const Vector3& h, const Vector3& n){
            // Vector3 hl = normalize(h.x*localTangent + h.y*localBiTangent + h.z*localNormal);
            float hn = Maths::clamp(Vector3::dot(n, h), 0.f, 1.f);
            float ag = getAlphag(clearcoatGloss) * getAlphag(clearcoatGloss);
            float num = ag - 1.f;
            float den = PI * log(ag) * (1.f + (ag - 1.f) * (hn * hn));
            return num / den;
        }
        // Gc
        // TODO:
        // need surface tangents to use the BSDF 
        // float getLambdaC(Vector3 w, Vector3 localNormal, Vector3 localTangent, Vector3 localBiTangent){
        static float getLambdaC(const Vector3& w, const Vector3& n){
            // Vector3 wl = normalize(w.x*localTangent + w.y*localBiTangent + w.z*localNormal);
            // float wx = (wl.x * 0.25f) * (wl.x * 0.25f);
            // float wy = (wl.y * 0.25f) * (wl.y * 0.25f);
            // float wz = wl.z * wl.z;
            float wn = Maths::clamp(Vector3::dot(n, w), 0.f, 1.f);
            float factor = sqrt(1.f + wn);
            return (factor - 1.f) / 2.f;
        }
        static float getGclearcoatW(const Vector3& w, const Vector3& n){
            // return 1.f / (1.f + getLambdaC(w, localNormal, localTangent, localBiTangent));
            return 1.f / (1.f + getLambdaC(w, n));
        }
        static float getGclearcoat(const Vector3& win, const Vector3& wout, const Vector3& n){
            float gwin = getGclearcoatW(win, n);
            float gwout = getGclearcoatW(wout, n);
            return  gwin*gwout;
        }
        // f clearcoat
        static Vector3 getClearcoat(Input i){
            Vector3 Fc = getFclearcoat(i._H, i._Wout);
            float Dc = getDclearcoat(i._Material->_ClearcoatGloss, i._H, i._ShadingNormal);
            float Gc = getGclearcoat(i._Win, i._Wout, i._ShadingNormal);
            return (Fc * Dc * Gc) / (4.f*std::abs(Vector3::dot(i._ShadingNormal, i._Win)));
        }

        // Sheen
        static Vector3 getCtint(const Vector3& baseColor, float intensity){
            if(std::abs(intensity) > EPSILON){
                return baseColor / intensity;
            }
            return Vector3(1.f, 1.f, 1.f);
        }
        static Vector3 getCsheen(const Vector3& baseColor, float intensity, float sheenTint){
            return (1.f - sheenTint) * Vector3(1.f, 1.f, 1.f) + sheenTint * getCtint(baseColor, intensity);
        }
        static Vector3 getSheen(Input i){
            Vector3 csheen = getCsheen(i._BaseColor, i._Intensity, i._Material->_SheenTint);
            float hw = pow(1.f - std::abs(Vector3::Vector3::dot(i._H, i._Wout)), 5);
            return csheen * hw * std::abs(Vector3::Vector3::dot(i._ShadingNormal, i._Wout));
        }


        // Metals
        // Fm
        static Vector3 getFmetal(const Vector3& baseColor, const Vector3& h, const Vector3& wout, float specularTint, float intensity, float specular, float metallic){
            Vector3 ks = (1.f - specularTint)*Vector3(1.f, 1.f, 1.f) + specularTint*getCtint(baseColor, intensity);
            // TODO: use eta instead of 1.5f
            Vector3 c0 = specular*getR0eta(1.5f)*(1.f - metallic)*ks + metallic*baseColor;
            float hwout = std::abs(Vector3::dot(h, wout));
            return c0 + (Vector3(1.f, 1.f, 1.f) - c0)*(1.f-pow(hwout, 5));
            // return baseColor + (Vector3::ones() - baseColor) * (1.f-pow(hwout, 5));
        }
        // Dm
        static float getAspect(float anisotropic){
            return sqrt(1.f - 0.9f*anisotropic);
        }
        static float getAlphaX(float roughness, float anisotropic){
            float alphaMin = 1e-4;
            float aspect = getAspect(anisotropic);
            return std::max(alphaMin, roughness*roughness*aspect);
        }
        static float getAlphaY(float roughness, float anisotropic){
            float alphaMin = 1e-4;
            float aspect = getAspect(anisotropic);
            return std::max(alphaMin, roughness*roughness/aspect);
        }
        // TODO:
        // need surface tangents to use the BSDF 
        static float getDmetal(float roughness, float anisotropic, const Vector3& h, const Vector3& n){
            float hn = Maths::clamp(Vector3::dot(n, h), 0.f, 1.f);
            float r = roughness;
            float alphaX = getAlphaX(r, anisotropic);
            float alphaY = getAlphaY(r, anisotropic);
            float a2 = alphaX * alphaY;

            float den = PI * Maths::sqr(1+(a2-1)*Maths::sqr(hn)) + EPSILON;
            return a2 / den;
        }
        // Gm
        static float getGmetalSmith(const Vector3& w, const Vector3& n, float alpha){
            float dotNW = Maths::clamp(Vector3::dot(n, w), 0.f, 1.f);
            float a2 = Maths::sqr(alpha);
            float den = (dotNW + (sqrt(a2 + (1-a2) * Maths::sqr(dotNW)))) + EPSILON;
            return (2.f*dotNW) / den;
        }
        // TODO:
        // need surface tangents to use the BSDF 
        static float getLambdaM(float roughness, float anisotropic, const Vector3& w, const Vector3& n){
            float wn = Maths::clamp(Vector3::dot(n, w), 0.f, 1.f);

            float alphaX = getAlphaX(roughness, anisotropic);
            float alphaY = getAlphaY(roughness, anisotropic);
            float a2 = Maths::clamp(alphaX*alphaY, 0.f, 1.f);

            float den = wn + (sqrt(a2 + (1-a2) * Maths::sqr(wn))) + EPSILON;
            return (2.f*wn) / den;
        }
        static float getGmetalW(float roughness, float anisotropic, const Vector3& w, const Vector3& n){
            // return 1.f / (1.f + getLambdaM(roughness, anisotropic, w, localNormal, localTangent, localBiTangent));
            return 1.f / (1.f + getLambdaM(roughness, anisotropic, w, n));
        }
        // float getGmetal(float roughness, float anisotropic, Vector3 win, Vector3 wout, Vector3 localNormal, Vector3 localTangent, Vector3 localBiTangent){
        static float getGmetal(float roughness, float anisotropic[[maybe_unused]], const Vector3& win, const Vector3& wout, const Vector3& n){
            // float gwin = getGmetalW(roughness, anisotropic, win, localNormal, localTangent, localBiTangent);
            // float gwout = getGmetalW(roughness, anisotropic, wout, localNormal, localTangent, localBiTangent);
            float gwin = getGmetalSmith(win, n, roughness);
            float gwout = getGmetalSmith(wout, n, roughness);
            // float gwin = getGmetalW(roughness, anisotropic, win, n);
            // float gwout = getGmetalW(roughness, anisotropic, wout, n);
            return  gwin*gwout;
        }
        // f metal
        static Vector3 getMetal(Input i){
            Vector3 Fm = getFmetal(i._BaseColor, i._H, i._Wout, i._Material->_SpecularTint, i._Intensity, i._Material->_Specular, i._Material->_Metallic);
            // float Dm = getDmetal(i._H, i._Material->_Roughness, i._Material->_Anisotropic, i._ShadingNormal, i._Tangent, i._Bitangent);
            // float Gm = getGmetal(i._Material->_Roughness, i._Material->_Anisotropic, i._Win, i._Wout, i._ShadingNormal, i._Tangent, i._Bitangent);
            float Dm = getDmetal(i._Material->_Roughness, i._Material->_Anisotropic, i._H, i._ShadingNormal);
            float Gm = getGmetal(i._Material->_Roughness, i._Material->_Anisotropic, i._Win, i._Wout, i._ShadingNormal);
            return (Fm * Dm * Gm) / (4.f*std::abs(Vector3::dot(i._ShadingNormal, i._Win)) + EPSILON);
        }

    
    public:
        static Vector3 BRDF(const Vector3& wi, const Vector3& wo, const Vector3& n,
            const Vector3& albedo, MaterialPtr material, PointLightPtr pointLight){
            Input i{};
            i._Win = wi;
            i._Wout = wo;
            i._H = Vector3::normalize(wi + wo);
            i._Material = material;
            i._ShadingNormal = n;
            i._BaseColor = albedo;
            i._Intensity = pointLight->getIntensity();
            
            
            Vector3 diffuse = (1.f - i._Material->_Specular) * (1.f - i._Material->_Metallic) * getDiffuse(i);
            Vector3 metal = (1.f - i._Material->_Specular * (1.f - i._Material->_Metallic)) * getMetal(i);
            Vector3 clearcoat = 0.25f * i._Material->_Clearcoat * getClearcoat(i);
            Vector3 sheen = (1.f - i._Material->_Metallic) * i._Material->_Sheen * getSheen(i);

            return diffuse + clearcoat + sheen + metal;
        }

        static Vector3 BRDF(const Vector3& wi, const Vector3& wo, const Vector3& n,
            const Vector3& albedo, MaterialPtr material, DirectionalLightPtr directionalLight){
            Input i{};
            i._Win = wi;
            i._Wout = wo;
            i._H = Vector3::normalize(wi + wo);
            i._Material = material;
            i._ShadingNormal = n;
            i._BaseColor = albedo;
            i._Intensity = directionalLight->getIntensity();
            
            
            Vector3 diffuse = (1.f - i._Material->_Specular) * (1.f - i._Material->_Metallic) * getDiffuse(i);
            Vector3 metal = (1.f - i._Material->_Specular * (1.f - i._Material->_Metallic)) * getMetal(i);
            Vector3 clearcoat = 0.25f * i._Material->_Clearcoat * getClearcoat(i);
            Vector3 sheen = (1.f - i._Material->_Metallic) * i._Material->_Sheen * getSheen(i);

            return diffuse + metal + clearcoat + sheen;
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