#version 400

in vec3 VertexPosition;
in vec3 VertexNormal;

out vec4 LightIntensity;

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

struct MaterialInfo {
    vec4 Ka; // Ambient
    vec4 Kd; // Diffuse
    vec4 Ks; // Specular
    float Shininess; // Specular power
};
uniform MaterialInfo Material;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

void getEyeSpace(out vec3 norm, out vec4 position)
{
    norm = normalize(NormalMatrix * VertexNormal);
    position = ModelViewMatrix * vec4(VertexPosition, 1.0);
}

vec4 phongModel(vec4 position, vec3 norm, int lightIndex)
{
    vec3 s = normalize(vec3(Lights[lightIndex].Position - position));
    vec3 v = normalize(-position.xyz);
    vec3 r = reflect(-s, norm);
    vec4 ambient = Material.Ka;
    float sDotN = max(dot(s, norm), 0.0);
    vec4 diffuse = Lights[lightIndex].Intensity * Material.Kd * sDotN;
    vec4 spec = vec4(0.0);
    if(sDotN > 0.0)
    {
        spec = Material.Ks * pow(max(dot(r,v), 0.0), Material.Shininess);
    }
    return ambient + diffuse + spec;
}

void main()
{
    vec3 eyeNorm;
    vec4 eyePosition;
    // Get the position and normal in eye space
    getEyeSpace(eyeNorm, eyePosition);
    // Evaluate the lighting equation
    LightIntensity = vec4(0.0);
    for(int i = 0; i < NumLights; i++)
    {
        LightIntensity += phongModel(eyePosition, eyeNorm, i);
    }

    gl_Position = MVP * vec4(VertexPosition, 1.0);
}
