import re
import sys
import os

# Replace #include directives with the content of the included file
def include_replacer(match):
    included_file = match.group(1)
    included_file_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), included_file)

    # Read the content of the included file
    with open(included_file_path, 'r') as file:
        included_content = file.read()

    return included_content

def process_shader(shader_source):
    # Regular expression pattern to match #include directives
    include_pattern = r'^\s*#include\s+"(.*)"\s*$'

    # Split the shader source into lines
    lines = shader_source.splitlines()

    # Process each line of the shader source
    processed_lines = []
    for line in lines:
        match = re.match(include_pattern, line)
        if match:
            # Handle #include directive
            processed_lines.append(include_replacer(match))
        else:
            # Keep the line as-is
            processed_lines.append(line)

    # Join the processed lines back into a single string
    processed_shader = '\n'.join(processed_lines)
    return processed_shader

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python preprocess_glsl.py <input_file>")
        sys.exit(1)

    input_file = sys.argv[1]
    # Read the shader source from the input file
    with open(input_file, 'r') as file:
        shader_source = file.read()
    # Process the shader source
    processed_shader = process_shader(shader_source)
    # Print the processed shader
    print(processed_shader)