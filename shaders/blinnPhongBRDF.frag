#version 450

#include "utils/includeBRDF.frag.inc.glsl"


vec4 getSpecularPointLight(PointLight light, vec3 objViewPos, vec3 objNorm, vec4 objColor){
    vec3 wi = getWi(light, objViewPos);
    vec3 wo = getWo(objViewPos);
    vec3 wh = getWh(wi, wo);
    float weight = materialUbo._Materials[push._MaterialId]._Specular * light._Intensity * clamp(dot(wh, objNorm), 0.f, 1.f);
    return max(weight, 0.f) * light._Color * objColor;
}

vec4 getSpecularDirectional(DirectionalLight light, vec3 objViewPos, vec3 objNorm, vec4 objColor){
    vec3 wi = normalize(vec3(cameraUbo._View * light._Direction));
    vec3 wo = getWo(objViewPos);
    vec3 wh = getWh(wi, wo);

    float weight = materialUbo._Materials[push._MaterialId]._Specular * light._Intensity * dot(wh, objNorm);
    return max(weight, 0.f) * light._Color * objColor;
}


void main() {
    vec3 finalColor = vec3(0.f);

    vec3 n = normalize(fViewNorm);
    vec3 worldN = normalize(fWorldNorm);

    for(int i=0; i<min(MAX_NB_POINT_LIGHTS, lightsUbo._NbPointLights); i++){
        PointLight curPointLight = lightsUbo._PointLights[i];
        finalColor += getDiffusePointLight(curPointLight, fViewPos, n, fCol).xyz;
        finalColor += getSpecularPointLight(curPointLight, fViewPos, n, fCol).xyz;
    }

    for(int i=0; i<min(MAX_NB_DIRECTIONAL_LIGHTS, lightsUbo._NbDirectionalLights); i++){
        DirectionalLight curDirectionalLight = lightsUbo._DirectionalLights[i];
        finalColor += getDiffuseDirectionalLight(curDirectionalLight, worldN, fCol).xyz;
        // finalColor += getSpecularDirectional(curDirectionalLight, fViewPos, n, fCol).xyz;
    }
    outColor = vec4(finalColor, 1.f);
    
}