#version 400

in vec3 Position;
in vec3 Normal;

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

struct FogInfo {
    float MaxDist;
    float MinDist;
    vec4 Color;
};
uniform FogInfo Fog;

uniform vec4 Kd;
uniform vec4 Ka;
uniform vec4 Ks;
uniform float Shininess;

out vec4 FragColor;

vec3 ads(int lightIndex)
{
    vec3 s = normalize(Lights[lightIndex].Position.xyz - Position);
    vec3 v = normalize(vec3(-Position));
    vec3 h = normalize(v + s);
    vec3 ambient = Ka.xyz * Lights[lightIndex].Intensity.xyz;
    vec3 diffuse = Lights[lightIndex].Intensity.xyz * Kd.xyz * max(0.0, dot(s, Normal));
    vec3 spec = Lights[lightIndex].Intensity.xyz * Ks.xyz * pow(max(0.0, dot(h, Normal)), Shininess);

    return ambient + diffuse + spec;
}

void main()
{
    float dist = abs(Position.z);
    float fogFactor = (Fog.MaxDist - dist) / (Fog.MaxDist - Fog.MinDist);
    fogFactor = clamp(fogFactor, 0.0, 1.0);

    vec3 shadeColor = vec3(0.0);
    for(int i = 0; i < NumLights; i++)
    {
        shadeColor += ads(i);
    }

    vec3 color = mix(Fog.Color.xyz, shadeColor.xyz, fogFactor);
    FragColor = vec4(color, 1.0);
}