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
uniform mat4 ModelMatrix;

uniform sampler3D ShadowJitterTex;
uniform vec3 ShadowJitterTexSize; // (width, height, depth)

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
            vec4 offsets = texelFetch(ShadowJitterTex, offsetCoord, 0) * Lights[lightIndex].ShadowSoftness * ShadowCoord.w;
            
            sc.xy = ShadowCoord.xy + offsets.xy;
            sum += textureProj(ShadowMaps[lightIndex], sc);
            sc.xy = ShadowCoord.xy + offsets.zw;
            sum += textureProj(ShadowMaps[lightIndex], sc);
        }
        
        shadow = sum / 8.0;
        if(shadow != 1.0 && shadow != 0.0)
        {
            for(int i = 4; i < samplesDiv2; i++)
            {
                offsetCoord.z = i;
                vec4 offsets = texelFetch(ShadowJitterTex, offsetCoord, 0) * Lights[lightIndex].ShadowSoftness * ShadowCoord.w;
                sc.xy = ShadowCoord.xy + offsets.xy;
                sum += textureProj(ShadowMaps[lightIndex], sc);
                sc.xy = ShadowCoord.xy + offsets.zw;
                sum += textureProj(ShadowMaps[lightIndex], sc);
            }
            shadow = sum / float(samplesDiv2 * 2.0);
        }
    }

    return shadow;
}

void main()
{
    FragColor = vec4(0, 0, 0, 1);
    for(int i = 0; i < LightCount; i++)
    {
        vec4 ShadowCoord = Lights[i].ShadowMatrix * ModelMatrix * ModelCoord;
        
        float shadow = getShadowInfluence(i, ShadowCoord);
        vec3 diffAndSpec = ads(i);

        FragColor += vec4((diffAndSpec * shadow) + Material.Ka.xyz, 1.0);
    }

    FragColor.w = 1.0;

    // Gamma correct
    FragColor = pow(FragColor, vec4(1.0 / 1.5));
}