// input view space position
layout(location = 0) in vec3 fPos;
// input color
layout(location = 1) in vec4 fCol;
// input normal
layout(location = 2) in vec3 fNorm;
// input texture coords
layout(location = 3) in vec2 fTex;

// output fragment color
layout(location = 0) out vec4 outColor;

// maximum number of point lights
const int MAX_NB_POINT_LIGHTS = 10;
// maximum number of directional lights
const int MAX_NB_DIRECTIONAL_LIGHTS = 10;

// pi constant
const float PI = 3.14159265358979323846;

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
    vec3 _Position;
    vec3 _Color;
    float _Intensity;
};

/**
 * A structure representing a directional light
*/
struct DirectionalLight{
    vec3 _Direction;
    vec3 _Color;
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
    vec3 lightViewPos = vec3(cameraUbo._View * vec4(light._Position, 1.f));
    vec3 lightDir = normalize(lightViewPos - objViewPos);
    float weight = light._Intensity * dot(lightDir, objNorm) / PI;
    return max(weight, 0.f) * vec4(light._Color, 1.f) * objColor;
}

/**
 * Get the diffuse model for a directional light
 * @param light The current directional light
 * @param objNorm The norm of the current fragment
 * @param objColor The fragment color
 * @return The color value of the response
*/
vec4 getDiffuseDirectionalLight(DirectionalLight light, vec3 objNorm, vec4 objColor){
    vec3 lightDir = vec3(cameraUbo._View * vec4(light._Direction, 1.f));
    float weight = light._Intensity * dot(-lightDir, objNorm) / PI;
    return max(weight, 0.f) * vec4(light._Color, 1.f) * objColor;
}

/**
 * Get the incident light direction for a point light
 * @param light The current point light
 * @param objViewPos The view position of the current fragment
 * @return The wi vector
*/
vec3 getWi(PointLight light, vec3 objViewPos){
    vec3 lightViewPos = vec3(cameraUbo._View * vec4(light._Position, 1.f));
    return normalize(lightViewPos - objViewPos);
}

/**
 * Get the incident light direction for a directional light
 * @param light The current directional light
 * @return The wi vector
*/
vec3 getWi(DirectionalLight light){
    return normalize(vec3(cameraUbo._View * vec4(light._Direction, 1.f)));
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
 * Get the ggx normal distribution
 * @param wh The half vector
 * @param alpha The roughness
 * @param objNorm The normal of the current fragment
 * @return The normal distribution
*/
float getGGXDistribution(vec3 wh, float alpha, vec3 objNorm){
    float alphaSqr = sqr(alpha);
    float den = PI * sqr(1+(alphaSqr-1)*sqr(dot(objNorm, wh)));
    return alphaSqr / den;
}

/**
 * Get the Beckmann normal distribution
 * @param wh The half vector
 * @param alpha The roughness
 * @param objNorm The normal of the current fragment
 * @return The normal distribution
*/
float getBeckmannDistribution(vec3 wh, float alpha, vec3 objNorm){
    float dotSqr = sqr(dot(objNorm, wh));
    float alphaSqr = sqr(alpha);
    float e = exp((dotSqr - 1) / (alphaSqr*dotSqr));
    return e / (PI * alphaSqr * sqr(dotSqr));
}

/**
 * Get the Schlick approximation of the fresnel term
 * @param f0 The Fresnel reflectance
 * @param wi The incident light
 * @param wh The half vector
 * @return The Fresnel term
*/
float getSchlickFresnel(float f0, vec3 wi, vec3 wh){
    return f0 + (1-f0)*pow((1-max(0, dot(wi, wh))), 5);
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
    float dotWhN = dot(objNorm, wh);
    float dotWoN = dot(objNorm, wo);
    float dotWiN = dot(objNorm, wi);
    float dotWoWh = dot(wh, wo);
    float minW = min((2.f*dotWhN*dotWiN), (2.f*dotWhN*dotWoN));
    return min(1.f, minW/dotWoWh);
}

/**
 * Get the G1 Smith value
 * @param w The current ray
 * @param objNorm The normal of the current fragment
 * @param alpha A constant
 * @return The G1 value
*/
float getG1Smith(vec3 w, vec3 objNorm, float alpha){
    float dotNW = dot(objNorm, w);
    float alphaSqr = sqr(alpha);
    return (2.f*dotNW) / (dotNW + (sqrt(alphaSqr + (1-alphaSqr) * sqr(dotNW))));
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
    float dotNW = dot(objNorm, w);
    return dotNW / (dotNW*(1-k)+k);
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



/// from https://github.com/wdas/brdf/blob/main/src/brdfs/disney.brdf
float getAnisoGTR2(float NdotH, float HdotX, float HdotY, float ax, float ay){
    return 1.f / (PI * ax*ay * sqr(sqr(HdotX/ax) + sqr(HdotY/ay) + NdotH*NdotH));
}

float getAnisoGGXSmithGeometric(float NdotV, float VdotX, float VdotY, float ax, float ay){
    return 1.f / (NdotV + sqrt( sqr(VdotX*ax) + sqr(VdotY*ay) + sqr(NdotV)));
}

vec3 mon2lin(vec3 x){
    return vec3(pow(x[0], 2.2), pow(x[1], 2.2), pow(x[2], 2.2));
}

float getGTR1(float NdotH, float a){
    if (a >= 1) return 1/PI;
    float a2 = a*a;
    float t = 1 + (a2-1)*NdotH*NdotH;
    return (a2-1) / (PI*log(a2)*t);
}