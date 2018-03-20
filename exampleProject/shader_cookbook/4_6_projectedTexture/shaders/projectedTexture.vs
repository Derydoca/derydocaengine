#version 400

in vec3 VertexPosition;
in vec3 VertexNormal;

out vec3 EyeNormal; // Normal in eye coords
out vec4 EyePosition; // Position in eye coords
out vec4 ProjTexCoord;

uniform mat4 ProjectorMatrix;
uniform vec3 WorldCameraPosition;

uniform mat4 ModelViewMatrix;
uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

void main()
{
    vec4 pos4 = vec4(VertexPosition, 1.0);
    EyeNormal = normalize(NormalMatrix * VertexNormal);
    EyePosition = ModelViewMatrix * pos4;
    ProjTexCoord = ProjectorMatrix * (ModelMatrix * pos4);
    
    gl_Position = MVP * vec4(VertexPosition, 1.0);
}