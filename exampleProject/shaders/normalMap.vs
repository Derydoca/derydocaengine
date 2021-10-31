#version 430

in vec3 VertexPosition;
in vec3 VertexNormal;
in vec2 VertexTexCoord;
in vec4 VertexTangent;
in vec3 VertexBitangent;

struct Light {
    vec4 Direction;
    vec4 Position;
    vec4 Intensity;
    int Type;
    float Cutoff;
    float Exponent;
    float _padding;
};
layout (std140) uniform LightCollection
{
    Light Lights[10];
    int NumLights;
};

out vec3 LightDirs[10];
out vec2 TexCoord;
out vec3 ViewDir;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

void main()
{
    // Transform normal and tangent to eye space
    vec3 norm = normalize( NormalMatrix * VertexNormal );
    vec3 tang = normalize( NormalMatrix * vec3(VertexTangent) );
    // Compute the binormal
    vec3 binormal = normalize( cross( norm, tang ) ) * VertexTangent.w;

    // Matrix for transformation to tangent space
    mat3 toObjectLocal = mat3(
        tang.x, binormal.x, norm.x,
        tang.y, binormal.y, norm.y,
        tang.z, binormal.z, norm.z ) ;

    // Transform light direction and view direction to tangent space
    vec3 pos = vec3( ModelViewMatrix * vec4(VertexPosition,1.0) );
    for(int i = 0; i < 10; i++)
    {
        LightDirs[i] = normalize( toObjectLocal * (Lights[i].Position.xyz - pos) );
    }

    ViewDir = toObjectLocal * normalize(-pos);

    TexCoord = VertexTexCoord;

    gl_Position = MVP * vec4(VertexPosition,1.0);
}