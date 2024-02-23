#!/bin/bash

# Set the input and output directories
inputDirectory="shaders"
outputDirectory="${inputDirectory}/compiled"
preprocessedDirectory="${inputDirectory}/preprocessed"
preprocessingScript="${inputDirectory}/shaderPreprocessor.py"

compiler="glslc"
types=("vert" "frag")

# Empty the output directory
rm -rf "${outputDirectory}" "${preprocessedDirectory}"

# Create the output directory if it doesn't exist
mkdir -p "${outputDirectory}" "${preprocessedDirectory}"

# Preprocessor function
preprocess_shader() {
    # Run the preprocessor script (assuming it's named preprocess_glsl.py)
    processedShader=$(python ${preprocessingScript} "$1")
    # Return the processed shader
    echo "$processedShader"
}

# Loop through all shader files in the input directory
for shaderType in "${types[@]}"; do
    for file in "${inputDirectory}"/*."${shaderType}"; do
        # Extract the base name of the file without extension
        baseName=$(basename "${file%.*}")
        # Set the output file name with the .spv extension
        outputFile="${outputDirectory}/${baseName}.${shaderType}.spv"

        # Preprocess the shader
        preprocessedShader=$(preprocess_shader "${file}")
        # Write the preprocessed shader to a temporary file
        tmpFile="${preprocessedDirectory}/${baseName}.pre.${shaderType}"
        echo "${preprocessedShader}" > "${tmpFile}"

        # Debug log
        echo "Compiling shader ${file} to ${outputFile}..."
        # Compile the shader using glslc
        glslc "${tmpFile}" -o "${outputFile}"

        # Check if the compilation was successful
        if [ $? -eq 0 ]; then
            echo "Successfully compiled shader ${file} to ${outputFile}"
            echo
        else
            echo "Failed to compile shader ${file}"
            echo
            exit 1
        fi
    done
done