#version 450

layout(location = 0) in vec3 fPos;
layout(location = 1) in vec4 fCol;
layout(location = 2) in vec3 fNorm;
layout(location = 3) in vec2 fTex;

layout(location = 0) out vec4 outColor;

const int MAX_NB_POINT_LIGHTS = 10;

struct Material {
    float _Metallic;
    float _Subsurface;
    float _Specular;
    float _Roughness;
    float _SpecularTint;
    float _Anisotropic;
    float _Sheen;
    float _SheenTint;
    float _Clearcoat;
    float _ClearcoatGloss;
};

struct PointLight{
    vec3 _Position;
    vec3 _Color;
    float _Intensity;
};

struct DirectionalLight{
    vec3 _Direction;
    vec3 _Color;
    float _Intensity;
};

layout(set = 0, binding = 0) uniform CameraUbo{
    mat4 _View;
    mat4 _Proj;
} cameraUbo;

layout(set = 1, binding = 0) uniform LightsUbo{
    uint _NbPointLights;
    PointLight _PointLights[MAX_NB_POINT_LIGHTS];
} lightsUbo;

layout(set = 2, binding = 0) uniform MaterialUbo{
    Material _ObjMaterial;
} materialUbo;


/**
 * Get the diffuse model for the point light
 * @param light The current point light
 * @param objViewPos The view position of the current vertex
 * @param objNorm The norm of the current vertex
 * @param objColor The vertex color
 * @return The color value of the response
*/
vec4 getDiffusePointLight(PointLight light, vec3 objViewPos, vec3 objNorm, vec4 objColor){
    vec3 lightViewPos = vec3(cameraUbo._View * vec4(light._Position, 1.f));
    vec3 lightDir = normalize(lightViewPos - objViewPos);
    float weight = light._Intensity * dot(lightDir, objNorm);
    return max(weight, 0.f) * vec4(light._Color, 1.f) * objColor;
}


void main() {
    vec3 finalColor = vec3(0.f);

    vec3 n = normalize(fNorm);

    for(int i=0; i<lightsUbo._NbPointLights; i++){
        PointLight curPointLight = lightsUbo._PointLights[i];
        finalColor += getDiffusePointLight(curPointLight, fPos, n, fCol).xyz;
    }
    outColor = vec4(finalColor, 1.f);
    
}