#version 400

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D Tex1;
uniform sampler2D Tex2;

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

struct MaterialInfo
{
    vec4 Ka;
    vec4 Kd;
    vec4 Ks;
    float Shininess;
};
uniform MaterialInfo Material;

out vec4 FragColor;

void ads(int lightIndex, vec3 n, out vec3 ambAndDiff, out vec3 spec)
{
    vec3 s = normalize(Lights[lightIndex].Position.xyz - Position);
    vec3 v = normalize(vec3(-Position));
    vec3 h = normalize(v + s);
    vec3 ambient = Material.Ka.xyz * Lights[lightIndex].Intensity.xyz;
    vec3 diffuse = Lights[lightIndex].Intensity.xyz * Material.Kd.xyz * max(0.0, dot(s, n));
    
    ambAndDiff = ambient + diffuse;
    spec = Lights[lightIndex].Intensity.xyz * Material.Ks.xyz * pow(max(0.0, dot(h, n)), Material.Shininess);
}

void main()
{
    vec3 ambAndDiff, spec;
    vec4 baseColor = texture(Tex1, TexCoord);
    vec4 alphaMap = texture(Tex2, TexCoord);
    if(alphaMap.a > 0.15)
    {
        discard;
    }
    else
    {
        for(int i = 0; i < NumLights; i++)
        {
            vec3 tempAmbAndDiff;
            vec3 tempSpec;
            if(gl_FrontFacing)
            {
                ads(i, normalize(Normal), tempAmbAndDiff, tempSpec);
            }
            else
            {
                ads(i, normalize(-Normal), tempAmbAndDiff, tempSpec);
            }
            ambAndDiff += tempAmbAndDiff;
            spec += tempSpec;
        }

        FragColor = vec4(ambAndDiff, 1.0) * baseColor + vec4(spec, 1.0);
    }
}