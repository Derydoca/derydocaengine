#version 400

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

uniform mat4 ShadowMatrix[10];
uniform sampler2DShadow ShadowMap[10];
uniform float ShadowSoftness[10];

struct MaterialInfo {
    vec4 Kd;
    vec4 Ka;
    vec4 Ks;
    float Shininess;
};
uniform MaterialInfo Material;

in vec3 Position;
in vec3 Normal;
in vec4 ModelCoord;

out vec4 FragColor;

vec3 ads(int lightIndex)
{
    vec3 s = normalize(Lights[lightIndex].Position.xyz - Position);
    vec3 v = normalize(vec3(-Position));
    vec3 h = normalize(v + s);
    vec3 n = normalize(Normal);
    
    vec3 diffuse = Lights[lightIndex].Intensity.xyz * Material.Kd.xyz * max(0.0, dot(s, n));
    vec3 spec = Lights[lightIndex].Intensity.xyz * Material.Ks.xyz * pow(max(0.0, dot(h, n)), Material.Shininess);

    return diffuse + spec;
}

float getShadowInfluence(int lightIndex, vec4 ShadowCoord)
{
    float shadow = 0;
    shadow += textureProjOffset(ShadowMap[lightIndex], ShadowCoord, ivec2(-1, -1));
    shadow += textureProjOffset(ShadowMap[lightIndex], ShadowCoord, ivec2(-1, 1));
    shadow += textureProjOffset(ShadowMap[lightIndex], ShadowCoord, ivec2(1, 1));
    shadow += textureProjOffset(ShadowMap[lightIndex], ShadowCoord, ivec2(1, -1));
    shadow *= 0.25;
    return shadow;
}

void main()
{
    FragColor = vec4(0, 0, 0, 1);
    for(int i = 0; i < NumLights; i++)
    {
        vec4 ShadowCoord = ShadowMatrix[i] * ModelCoord;

        float shadow = getShadowInfluence(i, ShadowCoord);
        vec3 diffAndSpec = ads(i);

        FragColor += vec4((diffAndSpec * shadow) + Material.Ka.xyz, 1.0);
    }

    FragColor.w = 1.0;

    // Gamma correct
    FragColor = pow(FragColor, vec4(1.0 / 1.5));
}