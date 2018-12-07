#version 400
in vec3 VertexPosition;
in vec3 VertexNormal;

out vec3 LightIntensity;

struct LightInfo {
    vec4 Position; // Eye coords
    vec4 Ld;
};
uniform LightInfo Lights[10];

uniform vec4 Kd; // Diffuse reflectivity
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

vec3 diffuse(int lightIndex)
{
    vec3 tnorm = normalize(NormalMatrix * VertexNormal);
    vec4 eyeCoords = ModelViewMatrix * vec4(VertexPosition, 1.0);
    vec3 s = normalize(vec3(Lights[lightIndex].Position - eyeCoords));
    return (Lights[lightIndex].Ld * Kd * max(dot(s, tnorm), 0.0)).xyz;
}

void main()
{
    // The diffuse shading equation
    LightIntensity = vec3(0.0);
    for(int i = 0; i < 10; i++)
    {
        LightIntensity += diffuse(i);
    }
    // Convert position to clip coordinates and pass along
    gl_Position = MVP * vec4(VertexPosition, 1.0);
}
