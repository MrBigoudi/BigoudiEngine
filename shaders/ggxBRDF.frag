#version 450

layout(location = 0) in vec3 fPos;
layout(location = 1) in vec4 fCol;
layout(location = 2) in vec3 fNorm;
layout(location = 3) in vec2 fTex;

layout(location = 0) out vec4 outColor;

const int MAX_NB_POINT_LIGHTS = 10;
const int MAX_NB_DIRECTIONAL_LIGHTS = 10;
const float PI = 3.14159265358979323846;

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
    uint _NbDirectionalLights;
    DirectionalLight _DirectionalLights[MAX_NB_DIRECTIONAL_LIGHTS];
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
    float weight = light._Intensity * dot(lightDir, objNorm) / PI;
    return max(weight, 0.f) * vec4(light._Color, 1.f) * objColor;
}

/**
 * Get the diffuse model for the directional light
 * @param light The current directional light
 * @param objNorm The norm of the current vertex
 * @param objColor The vertex color
 * @return The color value of the response
*/
vec4 getDiffuseDirectionalLight(DirectionalLight light, vec3 objNorm, vec4 objColor){
    vec3 lightDir = vec3(cameraUbo._View * vec4(light._Direction, 1.f));
    float weight = light._Intensity * dot(-lightDir, objNorm) / PI;
    return max(weight, 0.f) * vec4(light._Color, 1.f) * objColor;
}

vec3 getWi(PointLight light, vec3 objViewPos){
    vec3 lightViewPos = vec3(cameraUbo._View * vec4(light._Position, 1.f));
    return normalize(lightViewPos - objViewPos);
}

vec3 getWi(DirectionalLight light){
    return normalize(vec3(cameraUbo._View * vec4(light._Direction, 1.f)));
}

vec3 getWo(vec3 objViewPos){
    return normalize(-objViewPos);
}

vec3 getWh(vec3 wi, vec3 wo){
    return normalize(wi+wo);
}

float sqr(float a){
    return a*a;
}

float getGGXDistribution(vec3 wh, float alpha, vec3 objNorm){
    float alpha_sqr = sqr(alpha);
    float den = PI * sqr(1+(alpha_sqr-1)*sqr(dot(objNorm, wh)));
    return alpha_sqr / den;
}


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