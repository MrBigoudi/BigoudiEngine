#version 450

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec4 vCol;
layout(location = 2) in vec3 vNorm;
layout(location = 3) in vec2 vTex;

layout(location = 0) out vec4 fPos;
layout(location = 1) out vec4 fCol;
layout(location = 2) out vec3 fNorm;
layout(location = 3) out vec2 fTex;

layout(push_constant, std430) uniform Push{
    mat4 _Model;
}push;

layout(set = 0, binding = 0) uniform CameraUbo{
    mat4 _View;
    mat4 _Proj;
} cameraUbo;

void main() {
    gl_Position = cameraUbo._Proj * cameraUbo._View * push._Model * vec4(vPos, 1.f);
    fPos = gl_Position;
    fCol = vCol;
    fNorm = vNorm;
    fTex = vTex;
}