#version 400

in vec3 VertexPosition;
in vec3 VertexNormal;
in vec2 VertexTexCoord;
in vec3 VertexTangent;
in vec3 VertexBitangent;

struct LightInfo
{
    vec4 Position;
    vec4 Intensity;
};
uniform LightInfo Lights[10];

out vec3 LightDirs[10];
out vec2 TexCoord;
out vec3 ViewDir;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

void main()
{
    vec3 norm = normalize(NormalMatrix * VertexNormal);
    vec3 tang = normalize(NormalMatrix * VertexTangent);
    vec3 bitang = normalize(NormalMatrix * VertexBitangent);
    mat3 toObjectLocal = mat3(
        tang.x, bitang.x, norm.x,
        tang.y, bitang.y, norm.y,
        tang.z, bitang.z, norm.z
    );
    // Get the position in eye coords
    vec3 pos = vec3(ModelViewMatrix * vec4(VertexPosition, 1.0));
    // Transform light directions and view direction to tangent space
    for(int i = 0; i < 10; i++)
    {
        LightDirs[i] = normalize(toObjectLocal * (Lights[i].Position.xyz - pos));
    }
    ViewDir = toObjectLocal * normalize(-pos);
    // Pass along the texture coordinate
    TexCoord = VertexTexCoord;
    gl_Position = MVP * vec4(VertexPosition, 1.0);
}