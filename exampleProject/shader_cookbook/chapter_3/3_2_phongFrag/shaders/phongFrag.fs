#version 400

in vec3 Position;
in vec3 Normal;

uniform vec4 LightPosition;
uniform vec4 LightIntensity;
uniform vec4 Kd;
uniform vec4 Ka;
uniform vec4 Ks;
uniform float Shininess;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ModelMatrix;
uniform mat4 MVP;

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

vec4 ads(int lightIndex)
{
    vec3 lightPos = vec3(Lights[lightIndex].Position);

    vec3 l;
    if(Lights[lightIndex].Type == 0)
    {
        // Ignore directional lights for now
        return vec4(0);
    }
    else if(Lights[lightIndex].Type == 1)
    {
        l = normalize(lightPos - Position);
    }

    vec3 n = normalize(Normal);
    
    float lDotN = dot(l, n);
    vec4 diff = Lights[lightIndex].Intensity * Kd * max(lDotN, 0.0);

    vec3 v = normalize(-Position);
    
    vec3 r = reflect(-l,n);
    vec4 spec;
    if(lDotN > 0.0)
    {
        spec = vec4(pow(max(dot(r, v), 0.0), Shininess));
    }

    return Ka + diff + spec;
}

void main()
{
    FragColor = vec4(0.0);
    for(int i = 0; i < NumLights; i++)
    {
        FragColor += ads(i);
    }
}