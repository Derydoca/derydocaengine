#version 400

struct LightInfo
{
    vec4 Position;
    vec4 Intensity;
    mat4 ShadowMatrix;
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

uniform sampler2DShadow ShadowMaps[10];

in vec3 Position;
in vec3 Normal;
in vec4 ModelCoord;

out vec4 FragColor;

vec4 ads(int lightIndex)
{
    vec3 s = normalize(Lights[lightIndex].Position.xyz - Position);
    vec3 v = normalize(vec3(-Position));
    vec3 h = normalize(v + s);
    vec3 n = normalize(Normal);
    
    vec3 diffuse = Lights[lightIndex].Intensity.xyz * Material.Kd.xyz * max(0.0, dot(s, n));
    vec3 spec = Lights[lightIndex].Intensity.xyz * Material.Ks.xyz * pow(max(0.0, dot(h, n)), Material.Shininess);

    return vec4((diffuse + spec), 1.0);
}

void main()
{
    vec4 ShadowCoord = Lights[0].ShadowMatrix * ModelCoord;

    FragColor = Material.Ka;
    for(int i = 0; i < LightCount; i++)
    {
        float shadow = 0;
        shadow += textureProjOffset(ShadowMaps[i], ShadowCoord, ivec2(-1, -1));
        shadow += textureProjOffset(ShadowMaps[i], ShadowCoord, ivec2(-1, 1));
        shadow += textureProjOffset(ShadowMaps[i], ShadowCoord, ivec2(1, 1));
        shadow += textureProjOffset(ShadowMaps[i], ShadowCoord, ivec2(1, -1));
        shadow = shadow * 0.25;

        vec4 shadedLight = vec4(0, 0, 0, 1);
        if(shadow > 0)
        {
            shadedLight = ads(i) * shadow;
        }

        FragColor += shadedLight;
    }

    FragColor.w = 1.0;

    // Gamma correct
    FragColor = pow(FragColor, vec4(1.0 / 2.2));
}