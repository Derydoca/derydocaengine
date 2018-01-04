#version 410

in vec3 VertexPosition;
in vec3 VertexNormal;

varying vec4 position;
varying vec3 normal;

uniform vec4 LightPosition; // Light position in eye coords. ??
uniform vec3 Kd;            // Diffuse reflectivity
uniform vec3 Ld;            // Diffuse light intensity

uniform mat3 NormalMatrix;
uniform mat4 MVP;

void main()
{
    //normal = (MVP * vec4(VertexNormal, 0.0)).xyz;
    normal = NormalMatrix * VertexNormal;
    position = MVP * vec4(VertexPosition,1.0);
    gl_Position = MVP * vec4(VertexPosition,1.0);
}