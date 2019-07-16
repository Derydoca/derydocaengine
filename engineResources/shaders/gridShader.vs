#version 400

in vec3 VertexPosition;
in vec3 VertexNormal;

out vec3 WorldPos;
out vec3 Normal;

uniform vec4 UnitGridLineColor;
uniform vec4 SubUnitGridLineColor;
uniform mat4 ModelMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 ViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

uniform float minimumScale = 10.0;

void main()
{
    Normal = NormalMatrix * VertexNormal;
    mat4 inv = inverse(ViewMatrix);
    vec3 camOffset = vec3(inv[3][0], 0.0, inv[3][2]);
    float currentScale = abs(inv[3][1]) + minimumScale;
    vec3 scaledVertexPosition = VertexPosition * vec3(currentScale);
    gl_Position = MVP * vec4(scaledVertexPosition + camOffset, 1.0);
    WorldPos = (ModelMatrix * vec4(scaledVertexPosition + camOffset, 1.0)).xyz;
}
