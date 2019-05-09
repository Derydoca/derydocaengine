#version 400

in vec3 Position;
in vec3 Normal;

uniform vec4 Kd;
uniform vec4 Ka;

const int Levels = 3;
const float ScaleFactor = 1.0 / Levels;

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

out vec4 FragColor;

vec4 toonShade(int lightIndex)
{
    vec3 s = normalize(Lights[lightIndex].Position.xyz - Position.xyz);
    float cosine = max(0.0, dot(s, Normal));
    vec3 diffuse = Kd.xyz * floor(cosine * Levels) * ScaleFactor;
    return vec4(Lights[lightIndex].Intensity.xyz * (Ka.xyz + diffuse), 1.0);
}

void main()
{
    FragColor = vec4(0.0);
    for(int i = 0; i < NumLights; i++)
    {
        FragColor += toonShade(i);
    }
}