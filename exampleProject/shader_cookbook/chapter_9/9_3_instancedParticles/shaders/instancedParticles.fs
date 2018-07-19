#version 400

struct LightInfo
{
    vec4 Position;
    vec4 Intensity;
};
uniform LightInfo Lights[10];
uniform int LightCount;

struct MaterialInfo {
    vec4 Kd;
    vec4 Ka;
    vec4 Ks;
    float Shininess;
};
uniform MaterialInfo Material;

in vec3 Position;
in vec3 Normal;

layout (location = 0) out vec4 FragColor;

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
    FragColor = vec4(0, 0, 0, 1);
    for(int i = 0; i < LightCount; i++)
    {
        vec3 diffAndSpec = ads(i);

        FragColor += vec4(diffAndSpec + Material.Ka.xyz, 1.0);
    }

    FragColor.w = 1.0;
}