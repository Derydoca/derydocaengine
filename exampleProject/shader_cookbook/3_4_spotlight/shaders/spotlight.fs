#version 400

in vec3 Position;
in vec3 Normal;

uniform vec4 Kd;
uniform vec4 Ka;
uniform vec4 Ks;
uniform float Shininess;

struct LightInfo {
    int Type;
    vec4 Position;
    vec4 Intensity;
    vec3 Direction;
    float Exponent;
    float Cutoff;
};
uniform LightInfo Lights[10];

out vec4 FragColor;

vec3 adsWithSpotlight(int lightIndex)
{

    vec3 s = normalize(vec3(Lights[lightIndex].Position) - Position);
    float angle = acos(dot(-s, Lights[lightIndex].Direction));
    float cutoff = radians(clamp(Lights[lightIndex].Cutoff, 0.0, 90.0));
    vec3 ambient = Lights[lightIndex].Intensity.xyz * Ka.xyz;
    if(angle < cutoff)
    {
        float spotFactor = pow(dot(-s, Lights[lightIndex].Direction), Lights[lightIndex].Exponent);
        vec3 v = normalize(vec3(-Position));
        vec3 h = normalize(v + s);
        return ambient + spotFactor * Lights[lightIndex].Intensity.xyz * (Kd.xyz * max(dot(s, Normal), 0.0) + Ks.xyz * pow(max(dot(h, Normal), 0.0), Shininess));
    }
    else
    {
        return ambient;
    }
}

void main()
{
    FragColor = vec4(0.0);
    for(int i = 0; i < 10; i++)
    {
        FragColor += adsWithSpotlight(i);
    }
    //FragColor = vec4(adsWithSpotlight(), 1.0);
}