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

struct MaterialInfo {
    vec4 Kd;
    vec4 Ka;
    vec4 Ks;
    float Shininess;
    sampler2D AOTex;
    sampler2D DiffuseTex;
};
uniform MaterialInfo Material;

uniform sampler2D NoiseTexture;

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform float LowThreshold;
uniform float HighThreshold;

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

void main()
{
    // Get the noise value at TexCoord
    vec4 noise = texture(NoiseTexture, TexCoord);

    // If the value is outside the threshold, discard
    if(noise.a < LowThreshold) discard;
    if(noise.a > HighThreshold) discard;

    FragColor = vec4(0, 0, 0, 1);
    for(int i = 0; i < NumLights; i++)
    {
        vec3 diffAndSpec = ads(i);
        
        FragColor += vec4(diffAndSpec + Material.Ka.xyz, 1.0);
    }
    FragColor.w = 1.0;
}