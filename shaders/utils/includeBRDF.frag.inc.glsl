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
const int MAX_NB_POINT_LIGHTS = 1024;
// maximum number of directional lights
const int MAX_NB_DIRECTIONAL_LIGHTS = 1;

// pi constant
const float PI = 3.14159265358979323846;

// epsilon to avoid zero division
const float EPSILON = 1e-5;

/**
 * A structure representing a material
 * @ see be::Material
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
layout(set = 1, binding = 0) uniform LightsUbo{
    uint _NbPointLights;
    PointLight _PointLights[MAX_NB_POINT_LIGHTS];
    uint _NbDirectionalLights;
    DirectionalLight _DirectionalLights[MAX_NB_DIRECTIONAL_LIGHTS];
} lightsUbo;

/**
 * The object material ubo
*/
layout(set = 2, binding = 0) uniform MaterialUbo{
    Material _ObjMaterial;
} materialUbo;


/**
 * Get the diffuse model for a point light
 * @param light The current point light
 * @param objViewPos The view position of the current fragment
 * @param objNorm The norm of the current fragment
 * @param objColor The fragment color
 * @return The color value of the response
*/
vec4 getDiffusePointLight(PointLight light, vec3 objViewPos, vec3 objNorm, vec4 objColor){
    vec3 lightViewPos = vec3(cameraUbo._View * light._Position);
    vec3 lightDir = normalize(lightViewPos - objViewPos);
    float weight = light._Intensity * dot(lightDir, objNorm) / PI;
    return max(weight, 0.f) * light._Color * objColor;
}

/**
 * Get the diffuse model for a directional light
 * @param light The current directional light
 * @param objNorm The norm of the current fragment
 * @param objColor The fragment color
 * @return The color value of the response
*/
vec4 getDiffuseDirectionalLight(DirectionalLight light, vec3 objNorm, vec4 objColor){
    float weight = light._Intensity * dot(normalize(-light._Direction.xyz), objNorm) / PI;
    return max(weight, 0.f) * light._Color * objColor;
}

/**
 * Get the incident light direction for a point light
 * @param light The current point light
 * @param objViewPos The view position of the current fragment
 * @return The wi vector
*/
vec3 getWi(PointLight light, vec3 objViewPos){
    vec3 lightViewPos = vec3(cameraUbo._View * light._Position);
    return normalize(lightViewPos - objViewPos);
}

/**
 * Get the incident light direction for a directional light
 * @param light The current directional light
 * @return The wi vector
*/
vec3 getWi(DirectionalLight light){
    return normalize(-light._Direction.xyz);
}

/**
 * Get the outgoing light direction
 * @param objViewPos The view position of the current fragment
 * @return The wo vector
*/
vec3 getWo(vec3 objViewPos){
    return normalize(-objViewPos);
}

/**
 * Get the half vector
 * @param wi The incident vector
 * @param wo The outgoing vector
 * @return The wh vector
*/
vec3 getWh(vec3 wi, vec3 wo){
    return normalize(wi+wo);
}

/**
 * Return the square of a given float
 * @param a The current float
 * @return a^2
*/
float sqr(float a){
    return a*a;
}

/**
 * Get the Beckmann normal distribution
 * @param wh The half vector
 * @param alpha The roughness
 * @param objNorm The normal of the current fragment
 * @return The normal distribution
*/
float getBeckmannDistribution(vec3 wh, float alpha, vec3 objNorm){
    float dotSqr = sqr(clamp(dot(objNorm, wh), 0.f, 1.f));
    float a2 = sqr(alpha);
    float e = exp((dotSqr - 1) / (a2*dotSqr));
    return e / (PI * a2 * sqr(dotSqr));
}

/**
 * Get the ggx normal distribution
 * @param wh The half vector
 * @param alpha The roughness
 * @param objNorm The normal of the current fragment
 * @return The normal distribution
*/
float getGGXDistribution(vec3 wh, float alpha, vec3 objNorm){
    float a2 = sqr(alpha);
    float den = PI * sqr(1+(a2-1)*sqr(clamp(dot(objNorm, wh), 0.f, 1.f))) + EPSILON;
    return a2 / den;
}

/**
 * Get the Schlick approximation of the fresnel term
 * @param f0 The Fresnel reflectance
 * @param wi The incident light
 * @param wh The half vector
 * @return The Fresnel term
*/
vec3 getSchlickFresnel(vec3 f0, vec3 wi, vec3 wh){
    float LoH = clamp(dot(wi, wh), 0.f, 1.f);
    return f0 + (vec3(1.f) - f0) * pow(1.f - LoH, 5.f);
}

/**
 * Get the Cook-Torrance geometric term
 * @param wi The incident light
 * @param wo The outgoing light
 * @param wh The half vector
 * @param objNorm The normal of the current fragment
 * @return The geometric term
*/
float getCookTorranceGeometric(vec3 wi, vec3 wo, vec3 wh, vec3 objNorm){
    float dotWhN = clamp(dot(objNorm, wh), 0.f, 1.f);
    float dotWoN = clamp(dot(objNorm, wo), 0.f, 1.f);
    float dotWiN = clamp(dot(objNorm, wi), 0.f, 1.f);
    float dotWoWh = clamp(dot(wh, wo), 0.f, 1.f);
    float minW = min((2.f*dotWhN*dotWiN), (2.f*dotWhN*dotWoN));
    float den = dotWoWh + EPSILON;
    return min(1.f, minW/den);
}

/**
 * Get the G1 Smith value
 * @param w The current ray
 * @param objNorm The normal of the current fragment
 * @param alpha A constant
 * @return The G1 value
*/
float getG1Smith(vec3 w, vec3 objNorm, float alpha){
    float dotNW = clamp(dot(objNorm, w), 0.f, 1.f);
    float a2 = sqr(alpha);
    float den = (dotNW + (sqrt(a2 + (1-a2) * sqr(dotNW)))) + EPSILON;
    return (2.f*dotNW) / den;
}

/**
 * Get the G1 Schlick value
 * @param w The current ray
 * @param objNorm The normal of the current fragment
 * @param alpha A constant
 * @return The G1 value
*/
float getG1Schlick(vec3 w, vec3 objNorm, float alpha){
    float k = alpha * sqrt(2/PI);
    float dotNW = clamp(dot(objNorm, w), 0.f, 1.f);
    float den = (dotNW*(1-k)+k) + EPSILON;
    return dotNW / den;
}

/**
 * Get the GGX Smith geometric term
 * @param wi The incident light
 * @param wo The outgoing light
 * @param objNorm The normal of the current fragment
 * @param alpha A constant
 * @return The geometric term
*/
float getGGXSmithGeometric(vec3 wi, vec3 wo, vec3 objNorm, float alpha){
    return getG1Smith(wi, objNorm, alpha) * getG1Smith(wo, objNorm, alpha);
}

/**
 * Get the GGX Schlick geometric term
 * @param wi The incident light
 * @param wo The outgoing light
 * @param objNorm The normal of the current fragment
 * @param alpha A constant
 * @return The geometric term
*/
float getGGXSchlickGeometric(vec3 wi, vec3 wo, vec3 objNorm, float alpha){
    return getG1Schlick(wi, objNorm, alpha) * getG1Schlick(wo, objNorm, alpha);
}

/**
 * Get the diffuse component of a BRDF
 * @param m The material
 * @param color The object color
 * @return The diffuse component
*/
vec3 diffuseBRDF(Material m, vec3 color){
    return (1.f - m._Metallic) * color / PI;
}

vec3 getAttenuation(PointLight l, vec3 lightViewPos, vec3 fragViewPos) {
	float d = distance(lightViewPos, fragViewPos);
    float den = sqr(d) + EPSILON;
	return l._Intensity * l._Color.xyz / den;
}

vec3 getAttenuation(DirectionalLight l) {
	return l._Intensity * l._Color.xyz;
}

vec3 getToneMap (vec3 radiance, float exposure, float gamma) {
	vec3 rgb = exposure * radiance;
	return vec3 (pow (rgb.r, gamma), 
				 pow (rgb.g, gamma),
				 pow (rgb.b, gamma));
}

float D_GGX(vec3 n, vec3 wh, float a) {
    float NoH = clamp(dot(n, wh), 0.f, 1.f);
    float a2 = sqr(a);
    float f = (NoH * a2 - NoH) * NoH + 1.0;
    return a2 / (PI * f * f);
}

float V_SmithGGXCorrelated(vec3 wi, vec3 wo, vec3 n, float a) {
    float NoV = abs(dot(n, wo)) + EPSILON;
    float NoL = clamp(dot(n, wi), 0.f, 1.f);
    float a2 = a * a;
    float GGXL = NoV * sqrt((-NoL * a2 + NoL) * NoL + a2);
    float GGXV = NoL * sqrt((-NoV * a2 + NoV) * NoV + a2);
    return 0.5 / (GGXV + GGXL);
}