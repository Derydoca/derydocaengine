#version 400

struct LightInfo
{
    vec4 Position;
    vec4 Intensity;
    mat4 ShadowMatrix;
};
uniform LightInfo Lights[10];

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
in vec4 WorldCoord;

out vec4 FragColor;

vec4 ads(int lightIndex)
{
    vec3 s = normalize(Lights[lightIndex].Position.xyz - Position);
    vec3 v = normalize(vec3(-Position));
    vec3 h = normalize(v + s);
    vec3 n = normalize(Normal);
    vec3 ambient = Material.Ka.xyz;
    vec3 diffuse = Lights[lightIndex].Intensity.xyz * Material.Kd.xyz * max(0.0, dot(s, n));
    
    vec3 ambAndDiff = ambient + diffuse;
    vec3 spec = Lights[lightIndex].Intensity.xyz * Material.Ks.xyz * pow(max(0.0, dot(h, n)), Material.Shininess);

    return vec4((ambAndDiff + spec), 1.0);
}

void main()
{
    vec4 ShadowCoord = Lights[0].ShadowMatrix * WorldCoord;

    for(int i = 0; i < 10; i++)
    {
        float shadow = textureProj(ShadowMaps[i], ShadowCoord);

        vec4 shadedLight = Material.Ka;
        if(shadow > 0)
        {
            shadedLight = ads(i);
        }

        FragColor += shadedLight;
    }

    FragColor.w = 1.0;
}