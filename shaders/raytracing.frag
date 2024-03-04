#version 450

layout(location = 1) in vec2 fTex;

layout(location = 0) out vec4 outColor;

layout(binding = 0) uniform sampler2D scene;

void main() {
    outColor = texture(scene, fTex);
}