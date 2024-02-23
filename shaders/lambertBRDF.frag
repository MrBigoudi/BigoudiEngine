#version 450

#include "utils/includeBRDF.frag.inc.glsl"

void main() {
    vec3 finalColor = vec3(0.f);

    vec3 n = normalize(fNorm);

    for(int i=0; i<min(MAX_NB_POINT_LIGHTS, lightsUbo._NbPointLights); i++){
        PointLight curPointLight = lightsUbo._PointLights[i];
        finalColor += getDiffusePointLight(curPointLight, fPos, n, fCol).xyz;
    }

    for(int i=0; i<min(MAX_NB_DIRECTIONAL_LIGHTS, lightsUbo._NbDirectionalLights); i++){
        DirectionalLight curDirectionalLight = lightsUbo._DirectionalLights[i];
        finalColor += getDiffuseDirectionalLight(curDirectionalLight, n, fCol).xyz;
    }
    outColor = vec4(finalColor, 1.f);
    
}