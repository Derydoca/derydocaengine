#version 400

in vec3 EyeNormal;
in vec4 EyePosition;
in vec4 ProjTexCoord;

uniform sampler2D ProjectorTex;

struct MaterialInfo
{
    vec4 Kd;
    vec4 Ks;
    vec4 Ka;
    float Shininess;
};
uniform MaterialInfo Material;

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

vec3 phongModel(int lightIndex, vec3 pos, vec3 norm)
{
    vec3 s = normalize(vec3(Lights[lightIndex].Position) - pos);
    vec3 v = normalize(-pos.xyz);
    vec3 r = reflect(-s, norm);
    vec3 ambient = Lights[lightIndex].Intensity.xyz * Material.Ka.xyz;
    float sDotN = max(dot(s, norm), 0.0);
    vec3 diffuse = Lights[lightIndex].Intensity.xyz * Material.Kd.xyz * sDotN;
    vec3 spec = vec3(0.0);
    if(sDotN > 0.0)
    {
        spec = Lights[lightIndex].Intensity.xyz * Material.Ks.xyz * pow(max(dot(r,v), 0.0), Material.Shininess);
    }
    return ambient + diffuse + spec;
}

void main()
{
    vec3 color = vec3(0);
    for(int i = 0; i < NumLights; i++)
    {
        color += phongModel(i, vec3(EyePosition), normalize(EyeNormal));
    }

    vec4 projTexColor = vec4(0.0);
    if(ProjTexCoord.z > 0.0)
    {
        projTexColor = textureProj(ProjectorTex, ProjTexCoord);
    }

    FragColor = vec4(color, 1.0) + projTexColor * 0.5;
}