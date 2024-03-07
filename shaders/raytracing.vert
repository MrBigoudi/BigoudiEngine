#version 450

layout(location = 0) in vec3 vPos;
layout(location = 3) in vec2 vTex;

layout(location = 1) out vec2 fTex;

void main() {
    gl_Position = vec4(vPos, 1.f); // mandatory to fire rasterization properly
    fTex = vTex;
}