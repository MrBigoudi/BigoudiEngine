#version 450

#include "utils/includeBRDF.frag.inc.glsl"


vec3 microfacetBRDF(Material m, vec3 color, vec3 n, vec3 wo, vec3 wi){
    vec3 wh = getWh(wi, wo);

    float roughness = sqr(m._Roughness);
    float D = getGGXDistribution(wh, roughness, n);

    float reflectance = 1.f;
    vec3 f0 = 0.16f * reflectance * reflectance * (1.f - m._Metallic) + color * m._Metallic;
    vec3 F = getSchlickFresnel(f0, wi, wh);
    float G = getGGXSmithGeometric(wi, wo, n, roughness);
    float den = 4.f * clamp(dot(n, wi), 0.f, 1.f) * clamp(dot(n, wo), 0.f, 1.f) + EPSILON;
    return D*F*G / den;
    // float G = V_SmithGGXCorrelated(wi, wo, n, roughness);
    // return D*G*F;
}


void main() {
    vec3 finalColor = vec3(0.f);

    vec3 n = normalize(fViewNorm);
    vec3 worldN = normalize(fWorldNorm);

    vec3 wo = getWo(fViewPos);
    Material m = materialUbo._ObjMaterial;
    vec3 color = fCol.xyz;

    for(int i=0; i<min(MAX_NB_POINT_LIGHTS, lightsUbo._NbPointLights); i++){
        PointLight curPointLight = lightsUbo._PointLights[i];
        vec3 wi = getWi(curPointLight, fViewPos);
        
        vec3 li = getAttenuation(
                    curPointLight, 
                    vec3(cameraUbo._View * curPointLight._Position),
                    fViewPos
                );

        vec3 fd = diffuseBRDF(m, color);
        vec3 fs = microfacetBRDF(m, color, n, wo, wi);
        vec3 fr = fd + fs;

        float weight = max(0.f, dot(n, wi));
        finalColor += li * fr * weight;
    }

    for(int i=0; i<min(MAX_NB_DIRECTIONAL_LIGHTS, lightsUbo._NbDirectionalLights); i++){
        DirectionalLight curDirectionalLight = lightsUbo._DirectionalLights[i];
        vec3 wi = getWi(curDirectionalLight);

        vec3 li = getAttenuation(curDirectionalLight);

        vec3 fd = diffuseBRDF(m, color);
        vec3 fs = microfacetBRDF(m, color, n, wo, vec3(cameraUbo._View * vec4(wi,1.f)));
        vec3 fr = fd + fs;

        float weight = max(0.f, dot(worldN, wi));
        finalColor += li * fr * weight;
    }

    finalColor = getToneMap(finalColor, 1.f, 1.f);
    outColor = vec4(finalColor, 1.f);    
}