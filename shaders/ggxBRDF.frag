#version 450

#include "utils/includeBRDF.frag.inc.glsl"


/// adapted from https://github.com/wdas/brdf/blob/main/src/brdfs/disney.brdf
vec3 BRDF(vec3 wi, vec3 wo, vec3 wh, vec3 objNorm, vec3 color, vec3 tangent, vec3 bitangent, Material mat){
    float NdotL = dot(objNorm, wi);
    float NdotV = dot(objNorm, wo);
    if (NdotL < 0 || NdotV < 0) return vec3(0);

    float NdotH = dot(objNorm, wh);
    float LdotH = dot(wi, wh);

    vec3 Cdlin = mon2lin(color);
    float Cdlum = .3*Cdlin[0] + .6*Cdlin[1]  + .1*Cdlin[2]; // luminance approx.

    vec3 Ctint = Cdlum > 0 ? Cdlin/Cdlum : vec3(1); // normalize lum. to isolate hue+sat
    vec3 Cspec0 = mix(mat._Specular*.08*mix(vec3(1), Ctint, mat._SpecularTint), Cdlin, mat._Metallic);
    vec3 Csheen = mix(vec3(1), Ctint, mat._SheenTint);

    // Diffuse fresnel - go from 1 at normal incidence to .5 at grazing
    // and mix in diffuse retro-reflection based on roughness
    float FL = getSchlickFresnel(NdotL, wi, wh);
    float FV = getSchlickFresnel(NdotV, wi, wh);
    float Fd90 = 0.5 + 2 * LdotH*LdotH * mat._Roughness;
    float Fd = mix(1.0, Fd90, FL) * mix(1.0, Fd90, FV);

    // Based on Hanrahan-Krueger brdf approximation of isotropic bssrdf
    // 1.25 scale is used to (roughly) preserve albedo
    // Fss90 used to "flatten" retroreflection based on roughness
    float Fss90 = LdotH*LdotH*mat._Roughness;
    float Fss = mix(1.0, Fss90, FL) * mix(1.0, Fss90, FV);
    float ss = 1.25 * (Fss * (1 / (NdotL + NdotV) - .5) + .5);

    // specular
    float aspect = sqrt(1-mat._Anisotropic*.9);
    float ax = max(.001, sqr(mat._Roughness)/aspect);
    float ay = max(.001, sqr(mat._Roughness)*aspect);
    float Ds = getAnisoGTR2(NdotH, dot(wh, tangent), dot(wh, bitangent), ax, ay);
    float FH = getSchlickFresnel(LdotH, wi, wh);
    vec3 Fs = mix(Cspec0, vec3(1), FH);
    float Gs;
    Gs  = getAnisoGGXSmithGeometric(NdotL, dot(wi, tangent), dot(wi, bitangent), ax, ay);
    Gs *= getAnisoGGXSmithGeometric(NdotV, dot(wo, tangent), dot(wo, bitangent), ax, ay);

    // sheen
    vec3 Fsheen = FH * mat._Sheen * Csheen;

    // clearcoat (ior = 1.5 -> F0 = 0.04)
    float Dr = getGTR1(NdotH, mix(.1,.001,mat._ClearcoatGloss));
    float Fr = mix(.04, 1.0, FH);
    float Gr = getGGXSmithGeometric(wi, wo, objNorm, 0.25);

    return ((1/PI) * mix(Fd, ss, mat._Subsurface)*Cdlin + Fsheen)
        * (1-mat._Metallic)
        + Gs*Fs*Ds + .25*mat._Clearcoat*Gr*Fr*Dr;
}


void main() {
    vec3 finalColor = vec3(0.f);

    vec3 normal = normalize(fNorm);
    vec3 tangent = normalize( cross( vec3(0,1,0), normal ) );
    vec3 bitangent = normalize( cross( normal, tangent ) );

    vec3 wo = getWo(fPos);
    Material material = materialUbo._ObjMaterial;

    for(int i=0; i<min(MAX_NB_POINT_LIGHTS, lightsUbo._NbPointLights); i++){
        PointLight curPointLight = lightsUbo._PointLights[i];
        vec3 wi = getWi(curPointLight, fPos);
        vec3 wh = getWh(wi, wo);
        vec3 color = fCol.xyz * curPointLight._Color;
        finalColor += BRDF(wi, wo, wh, normal, color, tangent, bitangent, material);
    }

    for(int i=0; i<min(MAX_NB_DIRECTIONAL_LIGHTS, lightsUbo._NbDirectionalLights); i++){
        DirectionalLight curDirectionalLight = lightsUbo._DirectionalLights[i];
        vec3 wi = getWi(curDirectionalLight);
        vec3 wh = getWh(wi, wo);
        vec3 color = fCol.xyz * curDirectionalLight._Color;
        finalColor += BRDF(wi, wo, wh, normal, color, tangent, bitangent, material);
    }
    outColor = vec4(finalColor, 1.f);    
}