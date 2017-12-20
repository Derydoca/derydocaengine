#version 330

in vec4 VertexPosition;

varying vec3 texCoord0;

uniform mat4 MVP;

void main(void)
{
    texCoord0 = normalize(VertexPosition.xyz);
    gl_Position = MVP * VertexPosition;
}