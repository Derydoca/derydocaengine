#version 400

in vec3 VertexPosition;
in vec3 VertexNormal;

out vec3 ReflectDir; // Reflected direction
out vec3 RefractDir; // Refracted direction

struct MaterialInfo
{
    float Eta; // Ratio of indices of refraction
    float ReflectionFactor; // Percentage of reflected light
};
uniform MaterialInfo Material;

uniform bool DrawSkyBox;
uniform vec3 WorldCameraPosition;

uniform mat4 ModelViewMatrix;
uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

void main()
{
    if(DrawSkyBox)
    {
        ReflectDir = VertexPosition;
    }
    else
    {
        vec3 worldPos = vec3(ModelMatrix * vec4(VertexPosition, 1.0));
        vec3 worldNorm = vec3(ModelMatrix * vec4(VertexNormal, 0.0));
        vec3 worldView = normalize(WorldCameraPosition - worldPos);

        ReflectDir = reflect(-worldView, worldNorm);
        RefractDir = refract(-worldView, worldNorm, Material.Eta);
    }

    gl_Position = MVP * vec4(VertexPosition, 1.0);
}