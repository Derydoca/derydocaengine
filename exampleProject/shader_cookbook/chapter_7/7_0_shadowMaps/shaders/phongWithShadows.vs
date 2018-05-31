#version 400

in vec3 VertexPosition;
in vec3 VertexNormal;

out vec3 Normal;
out vec3 Position;
out vec4 WorldCoord;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

void main()
{
    Position = (ModelViewMatrix * vec4(VertexPosition, 1.0)).xyz;
    Normal = normalize(NormalMatrix * VertexNormal);
    WorldCoord = vec4(VertexPosition, 1.0);
    gl_Position = MVP * vec4(VertexPosition, 1.0);
}