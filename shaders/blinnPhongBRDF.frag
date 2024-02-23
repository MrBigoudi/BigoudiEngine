#version 450

#include "utils/includeBRDF.frag.inc.glsl"


vec4 getSpecularPointLight(PointLight light, vec3 objViewPos, vec3 objNorm, vec4 objColor){
    vec3 lightViewPos = vec3(cameraUbo._View * vec4(light._Position, 1.f));
    vec3 wi = normalize(lightViewPos - objViewPos);
    vec3 wo = normalize(-objViewPos);
    vec3 wh = normalize(wi+wo);
    float weight = materialUbo._ObjMaterial._Specular * light._Intensity * dot(wh, objNorm);
    return max(weight, 0.f) * vec4(light._Color, 1.f) * objColor;
}

vec4 getSpecularDirectional(DirectionalLight light, vec3 objViewPos, vec3 objNorm, vec4 objColor){
    vec3 wi = normalize(vec3(cameraUbo._View * vec4(light._Direction, 1.f)));
    vec3 wo = normalize(-objViewPos);
    vec3 wh = normalize(wi+wo);

    float weight = materialUbo._ObjMaterial._Specular * light._Intensity * dot(wh, objNorm);
    return max(weight, 0.f) * vec4(light._Color, 1.f) * objColor;
}


void main() {
    vec3 finalColor = vec3(0.f);

    vec3 n = normalize(fNorm);

    for(int i=0; i<min(MAX_NB_POINT_LIGHTS, lightsUbo._NbPointLights); i++){
        PointLight curPointLight = lightsUbo._PointLights[i];
        finalColor += getDiffusePointLight(curPointLight, fPos, n, fCol).xyz;
        finalColor += getSpecularPointLight(curPointLight, fPos, n, fCol).xyz;
    }

    for(int i=0; i<min(MAX_NB_DIRECTIONAL_LIGHTS, lightsUbo._NbDirectionalLights); i++){
        DirectionalLight curDirectionalLight = lightsUbo._DirectionalLights[i];
        finalColor += getDiffuseDirectionalLight(curDirectionalLight, n, fCol).xyz;
        finalColor += getSpecularDirectional(curDirectionalLight, fPos, n, fCol).xyz;
    }
    outColor = vec4(finalColor, 1.f);
    
}