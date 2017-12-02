#version 330

in vec4 position;

varying vec3 texCoord0;

uniform mat4 transform;

void main(void)
{
    texCoord0 = normalize(position.xyz);
    gl_Position = transform * position;
}