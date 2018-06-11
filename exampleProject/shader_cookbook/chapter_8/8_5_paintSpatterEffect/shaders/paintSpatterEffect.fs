#version 400

struct LightInfo
{
    vec4 Position;
    vec4 Intensity;
    mat4 ShadowMatrix;
    float ShadowSoftness;
};
uniform LightInfo Lights[10];
uniform int LightCount;

struct MaterialInfo {
    vec4 Kd;
    vec4 Ka;
    vec4 Ks;
    float Shininess;
    sampler2D AOTex;
    sampler2D DiffuseTex;
};
uniform MaterialInfo Material;
uniform MaterialInfo PaintMaterial;

uniform sampler2D NoiseTexture;

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform float Threshold;

out vec4 FragColor;

vec3 ads(int lightIndex, MaterialInfo mat)
{
    vec3 s = normalize(Lights[lightIndex].Position.xyz - Position);
    vec3 v = normalize(vec3(-Position));
    vec3 h = normalize(v + s);
    vec3 n = normalize(Normal);
    
    vec3 diffuse = Lights[lightIndex].Intensity.xyz * mat.Kd.xyz * max(0.0, dot(s, n));
    vec3 spec = Lights[lightIndex].Intensity.xyz * mat.Ks.xyz * pow(max(0.0, dot(h, n)), mat.Shininess);

    return diffuse + spec;
}

void main()
{
    // Get the noise value at TexCoord
    vec4 noise = texture(NoiseTexture, TexCoord);

    FragColor = vec4(0, 0, 0, 1);
    for(int i = 0; i < LightCount; i++)
    {
        if(noise.g < Threshold)
        {
            vec3 diffAndSpec = ads(i, Material);
            FragColor += vec4(diffAndSpec + Material.Ka.xyz, 1.0);
        }
        else
        {
            vec3 diffAndSpec = ads(i, PaintMaterial);
            FragColor += vec4(diffAndSpec + PaintMaterial.Ka.xyz, 1.0);
        }
    }
    FragColor.w = 1.0;
}