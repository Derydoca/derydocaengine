#version 400

struct MaterialInfo {
    vec4 Kd;
    vec4 Ka;
    vec4 Ks;
    float Shininess;
};
uniform MaterialInfo Material;

struct LightInfo {
    int Type;
    vec4 Position;
    vec4 Intensity;
};
uniform LightInfo Lights[10];

uniform vec4 LineColor;
in vec3 GPosition;
in vec3 GNormal;
flat in int GIsEdge;

out vec4 FragColor;

vec4 toonShade(int lightIndex)
{
    vec3 s = normalize(Lights[lightIndex].Position.xyz - GPosition.xyz);
    float cosine = max(0.0, dot(s, GNormal));
    vec3 diffuse = Material.Kd.xyz * floor(cosine * 3) * 1.0;
    return vec4(Lights[lightIndex].Intensity.xyz * (Material.Ka.xyz + diffuse), 1.0);
}

void main()
{
    if(GIsEdge == 1)
    {
        FragColor = LineColor;
    }
    else
    {
        FragColor = vec4(0.0);
        for(int i = 0; i < 10; i++)
        {
            FragColor += toonShade(i);
        }
    }
}