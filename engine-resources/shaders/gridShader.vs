#version 400

in vec3 VertexPosition;
in vec3 VertexNormal;
in vec2 VertexTexCoord;

out vec3 WorldPos;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

uniform float minimumScale = 50.0;

void main()
{
    TexCoord = VertexTexCoord;
    Normal = NormalMatrix * VertexNormal;
    mat4 inv = inverse(ViewMatrix);
    vec3 camOffset = vec3(inv[3][0], 0.0, inv[3][2]);
    float currentScale = abs(inv[3][1]) + minimumScale;
    vec3 scaledVertexPosition = VertexPosition * vec3(currentScale);
    gl_Position = MVP * vec4(scaledVertexPosition + camOffset, 1.0);
    WorldPos = (ModelMatrix * vec4(scaledVertexPosition + camOffset, 1.0)).xyz;
}
