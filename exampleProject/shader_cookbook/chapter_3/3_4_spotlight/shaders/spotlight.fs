#version 400

in vec3 Position;
in vec3 Normal;

uniform vec4 Kd;
uniform vec4 Ka;
uniform vec4 Ks;
uniform float Shininess;

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

vec3 adsWithSpotlight(int lightIndex)
{

    vec3 s = normalize(vec3(Lights[lightIndex].Position) - Position);
    float angle = acos(dot(-s, vec3(Lights[lightIndex].Direction)));
    float cutoff = radians(clamp(Lights[lightIndex].Cutoff, 0.0, 90.0));
    vec3 ambient = vec3(Lights[lightIndex].Intensity) * vec3(Ka);
    if(angle < cutoff)
    {
        float spotFactor = pow(dot(-s, vec3(Lights[lightIndex].Direction)), Lights[lightIndex].Exponent);
        vec3 v = normalize(-Position);
        vec3 h = normalize(v + s);
        return ambient + spotFactor * vec3(Lights[lightIndex].Intensity) * (vec3(Kd) * max(dot(s, Normal), 0.0) + vec3(Ks) * pow(max(dot(h, Normal), 0.0), Shininess));
    }
    else
    {
        return ambient;
    }
}

void main()
{
    vec3 color = vec3(0.0);
    for(int i = 0; i < NumLights; i++)
    {
        color += adsWithSpotlight(i);
    }
    FragColor = vec4(color, 1.0);
}