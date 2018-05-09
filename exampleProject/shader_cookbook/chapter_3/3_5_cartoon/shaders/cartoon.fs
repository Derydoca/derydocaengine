#version 400

in vec3 Position;
in vec3 Normal;

uniform vec4 Kd;
uniform vec4 Ka;

const int Levels = 3;
const float ScaleFactor = 1.0 / Levels;

struct LightInfo {
    int Type;
    vec4 Position;
    vec4 Intensity;
};
uniform LightInfo Lights[10];

out vec4 FragColor;

vec4 toonShade(int lightIndex)
{
    vec3 s = normalize(Lights[lightIndex].Position.xyz - Position.xyz);
    float cosine = max(0.0, dot(s, Normal));
    vec3 diffuse = Kd.xyz * floor(cosine * Levels) * ScaleFactor;
    return vec4(Lights[lightIndex].Intensity.xyz * (Ka.xyz + diffuse), 1.0);
}

void main()
{
    FragColor = vec4(0.0);
    for(int i = 0; i < 10; i++)
    {
        FragColor += toonShade(i);
    }
}