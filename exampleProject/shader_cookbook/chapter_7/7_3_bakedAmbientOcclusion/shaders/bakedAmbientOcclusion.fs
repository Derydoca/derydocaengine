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

uniform sampler3D ShadowJitterTex;
uniform vec3 ShadowJitterTexSize; // (width, height, depth)

struct MaterialInfo {
    vec4 Kd;
    vec4 Ka;
    vec4 Ks;
    float Shininess;
    sampler2D AOTex;
    sampler2D DiffuseTex;
};
uniform MaterialInfo Material;

in vec3 Position;
in vec3 Normal;
in vec4 ModelCoord;
in vec2 TexCoord;

out vec4 FragColor;

vec3 ads(int lightIndex)
{
    vec3 s = normalize(Lights[lightIndex].Position.xyz - Position);
    vec3 v = normalize(vec3(-Position));
    vec3 h = normalize(v + s);
    vec3 n = normalize(Normal);
    
    vec4 aoFactor = texture(Material.AOTex, TexCoord);
    vec3 diffuse = Lights[lightIndex].Intensity.xyz * Material.Kd.xyz * max(0.0, dot(s, n)) * aoFactor.r;
    vec3 spec = Lights[lightIndex].Intensity.xyz * Material.Ks.xyz * pow(max(0.0, dot(h, n)), Material.Shininess);

    return diffuse + spec;
}

float getShadowInfluence(int lightIndex, vec4 ShadowCoord)
{
    float shadow = 0.0f;

    ivec3 offsetCoord;
    offsetCoord.xy = ivec2(mod(gl_FragCoord.xy, ShadowJitterTexSize.xy));

    float sum = 0.0;
    int samplesDiv2 = int (ShadowJitterTexSize.z);
    vec4 sc = ShadowCoord;

    if(sc.z >= 0)
    {
        for(int i = 0; i < 4; i++)
        {
            offsetCoord.z = i;
            vec4 offsets = texelFetch(ShadowJitterTex, offsetCoord, 0) * ShadowSoftness[lightIndex] * ShadowCoord.w;
            
            sc.xy = ShadowCoord.xy + offsets.xy;
            sum += textureProj(ShadowMap[lightIndex], sc);
            sc.xy = ShadowCoord.xy + offsets.zw;
            sum += textureProj(ShadowMap[lightIndex], sc);
        }
        
        shadow = sum / 8.0;
        if(shadow != 1.0 && shadow != 0.0)
        {
            for(int i = 4; i < samplesDiv2; i++)
            {
                offsetCoord.z = i;
                vec4 offsets = texelFetch(ShadowJitterTex, offsetCoord, 0) * ShadowSoftness[lightIndex] * ShadowCoord.w;
                sc.xy = ShadowCoord.xy + offsets.xy;
                sum += textureProj(ShadowMap[lightIndex], sc);
                sc.xy = ShadowCoord.xy + offsets.zw;
                sum += textureProj(ShadowMap[lightIndex], sc);
            }
            shadow = sum / float(samplesDiv2 * 2.0);
        }
    }

    return shadow;
}

void main()
{
    FragColor = vec4(0, 0, 0, 1);
    for(int i = 0; i < NumLights; i++)
    {
        vec4 ShadowCoord = ShadowMatrix[i] * ModelCoord;

        float shadow = getShadowInfluence(i, ShadowCoord);
        vec3 diffuseTexFactor = texture(Material.DiffuseTex, TexCoord).xyz;
        vec3 diffAndSpec = ads(i);
        
        FragColor += vec4((diffAndSpec * diffuseTexFactor * shadow) + (Material.Ka.xyz * diffuseTexFactor), 0);
    }

    FragColor.w = 1.0;

    // Gamma correct
    FragColor = pow(FragColor, vec4(1.0 / 1.5));
}