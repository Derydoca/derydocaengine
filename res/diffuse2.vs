#version 150

uniform mat4 camera;
uniform mat4 model;

in vec3 vert;
in vec2 texCoord0;
in vec3 normal;

out vec3 fragVert;
out vec2 fragTexCoord;
out vec3 fragNormal;

void main() {
    // Pass some variables to the fragment shader
    fragTexCoord = texCoord0;
    fragNormal = normal;
    fragVert = position;
    
    // Apply all matrix transformations to vert
    gl_Position = camera * model * vec4(position, 1);
}