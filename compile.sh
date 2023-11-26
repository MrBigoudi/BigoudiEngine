#!/bin/bash

readonly shadersDir="src/shaders"

# Compile vertex shaders
for vert in $(find ${shadersDir} -type f -name "*.vert"); do
    shaderName=$(basename "${vert}" .vert)
    glslc "${vert}" -fshader-stage=vertex -o "${shadersDir}/${shaderName}.vert.spv"
done

# Compile fragment shaders
for frag in $(find ${shadersDir} -type f -name "*.frag"); do
    shaderName=$(basename "${frag}" .frag)
    glslc "${frag}" -fshader-stage=fragment -o "${shadersDir}/${shaderName}.frag.spv"
done