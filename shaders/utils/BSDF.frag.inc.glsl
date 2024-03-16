// input world space position
layout(location = 0) in vec3 fWorldPos;
// input view space position
layout(location = 1) in vec3 fViewPos;
// input color
layout(location = 2) in vec4 fCol;
// input normal in the world space
layout(location = 3) in vec3 fWorldNorm;
// input normal in the view space
layout(location = 4) in vec3 fViewNorm;
// input texture coords
layout(location = 5) in vec2 fTex;

// output fragment color
layout(location = 0) out vec4 outColor;

// maximum number of point lights
const int MAX_NB_POINT_LIGHTS = 10;
// maximum number of directional lights
const int MAX_NB_DIRECTIONAL_LIGHTS = 1;
// maximum number of materials
const int MAX_NB_MATERIALS = 10;

// pi constant
const float PI = 3.14159265358979323846;

// epsilon to avoid zero division
const float EPSILON = 1e-5;

/**
 * A structure representing a material
 * @see be::Material
*/
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

/**
 * A structure representing a point light
*/
struct PointLight{
    vec4 _Position;
    vec4 _Color;
    float _Intensity;
};

/**
 * A structure representing a directional light
*/
struct DirectionalLight{
    vec4 _Direction;
    vec4 _Color;
    float _Intensity;
};

/**
 * The camera ubo
*/
layout(set = 0, binding = 0) uniform CameraUbo{
    mat4 _View;
    mat4 _Proj;
} cameraUbo;

/**
 * The lights ubo
*/
layout(set = 1, binding = 0, std140) uniform LightsUbo{
    uint _NbPointLights;
    PointLight _PointLights[MAX_NB_POINT_LIGHTS];
    uint _NbDirectionalLights;
    DirectionalLight _DirectionalLights[MAX_NB_DIRECTIONAL_LIGHTS];
} lightsUbo;

/**
 * The object material ubo
*/
layout(set = 2, binding = 0, std140) uniform MaterialUbo{
    Material _Materials[MAX_NB_MATERIALS];
} materialUbo;

layout(push_constant, std430) uniform Push{
    mat4 _Model;
    uint _MaterialId;
}push;




// Diffuse
// f base diffuse
float getFD90(float roughness, vec3 h, vec3 wout){
    float cosTheta = abs(dot(h,wout));
    return 0.5f + 2.f*roughness*cosThetaD*cosThetaD;
}
float getFD(float fd90, vec3 n, vec3 w){
    float nw = abs(dot(n,w));
    return 1.f + (fd90 - 1.f)*(1.f - pow(nw, 5));
}
vec3 getBaseDiffuse(vec3 baseColor, float roughness, vec3 h, vec3 n, vec3 win, vec3 wout){
    float fd90 = getFD90(roughness, h, wout);
    float fdwin = getFD(fd90, n, win);
    float fdwout = getFD(fd90, n, wout);
    float factor = abs(dot(n,wout));
    vec3 color = baseColor / PI;
    return color * fdwin * fdwout * factor;
}
// f subsurface diffuse
float getFSS90(float roughness, vec3 h, vec3 wout){
    float hwout = abs(dot(h,wout));
    return roughness * hwout * hwout;
}
float getFSS(float fss90, vec3 n, vec3 w){
    float nw = abs(dot(n,w));
    return 1.f + (fss90 - 1.f)*(1.f-pow(nw, 5));
}
vec3 getSubsurfaceDiffuse(vec3 baseColor, float roughness, vec3 h, vec3 n, vec3 win, vec3 wout){
    float fss90 = getFSS90(roughness, h, wout);
    float fsswin = getFSS(fss90, n, win);
    float fsswout = getFSS(fss90, n, wout);
    float nwin = abs(dot(n, win));
    float nwout = abs(dot(n, wout));
    float factor = 1.f / (nwin + nwout) - 0.5f;
    vec3 color = 1.25f*baseColor / PI;
    return color * (fsswin * fsswout * factor + 0.5f) * nwout;
}
// f diffuse
vec3 getDiffuse(vec3 baseColor, float roughness, vec3 h, vec3 n, vec3 win, vec3 wout, float subsurface){
    vec3 baseDiffuse = getBaseDiffuse(baseColor. roughness, h, n, win, wout);
    vec3 subsurfaceDiffuse = getSubsurfaceDiffuse(baseColor. roughness, h, n, win, wout);
    return (1.f - subsurface) * baseDiffuse + subsurface * subsurfaceDiffuse;
}


// Metals
// Fm
vec3 getFmetal(vec3 baseColor, vec3 h, vec3 wout){
    float hwout = abs(dot(h, wout));
    return baseColor + (vec3(1.f, 1.f, 1.f) - baseColor)*(1.f-pow(hwout, 5));
}
// Dm
float getAspect(float anisotropic){
    return sqrt(1.f - 0.9f*anisotropic);
}
float getAlphaX(float roughness, float anisotropic){
    float alphaMin = 1e-4;
    float aspect = getAspect(anisotropic);
    return max(alphaMin, roughness*roughness*aspect);
}
float getAlphaY(float roughness, float anisotropic){
    float alphaMin = 1e-4;
    float aspect = getAspect(anisotropic);
    return max(alphaMin, roughness*roughness/aspect);
}
float getDmetal(vec3 baseColor, vec3 h, vec3 wout, float roughness, float anisotropic, vec3 localNormal, vec3 localTangent, vec3 localBiTangent){
    // get local frame
    vec3 hl = normalize(h.x*localTangent + h.y*localBiTangent + h.z*localNormal);
    float alphaX = getAlphaX(roughness, anisotropic);
    float alphaY = getAlphaY(roughness, anisotropic);

    float hlx = (hl.x*hl.x)/(alphaX*alphaX);
    float hly = (hl.y*hl.y)/(alphaY*alphaY);
    float hlz = hl.z * hl.z;
    float hlSum = (hlx + hly + hlz);
    
    return 1.f / (PI * alphaX * alphaY * hlSum * hlSum);
}
// Gm
float getLambda(vec3 roughness, vec3 anisotropic, vec3 w){
    float alphaX = getAlphaX(roughness, anisotropic);
    float alphaY = getAlphaY(roughness, anisotropic);
    float wx = (w.x * alphaX) * (w.x * alphaX);
    float wy = (w.y * alphaY) * (w.y * alphaY);
    float wz = w.z * w.z;
    float factor = sqrt(1.f + (wx + wy) / wz);
    return (factor - 1.f) / 2.f;
}
float getGmetalW(vec3 roughness, vec3 anisotropic, vec3 w){
    return 1.f / (1.f + getLambda(roughness, anisotropic, w));
}
float getGmetal(vec3 roughness, anisotropic, vec3 win, vec3 wout){
    return getGmetalW(roughness, anisotropic, win) * getGmetal(roughness, anisotropic, wout);
}
// f metal
vec3 getMetal(vec3 baseColor, vec3 h, vec3 wout, vec3 win, float roughness, float anisotropic, vec3 localNormal, vec3 localTangent, vec3 localBiTangent){
    vec3 Fm = getFmetal(baseColor, h, wout);
    float Dm = getDmetal(baseColor, h, wout, roughness, anisotropic, localNormal, localTangent, localBiTangent);
    float Gm = getGmetal(roughness, anisotropic, win, wout);
}

