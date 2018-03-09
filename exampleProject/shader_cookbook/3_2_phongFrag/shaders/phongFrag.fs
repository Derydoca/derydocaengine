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

struct LightInfo {
    int Type;
    vec4 Position;
    vec4 Intensity;
};
uniform LightInfo Lights[10];

out vec4 FragColor;

vec4 ads(int lightIndex)
{
    vec3 n = normalize(Normal);
    vec3 s;
    vec3 lightPos = vec3(Lights[lightIndex].Position);
    //vec3 lightPos = vec3(0);
    if(Lights[lightIndex].Type == 0)
    {
        // Ignore directional lights for now
        return vec4(0);
        s = normalize(lightPos);
    }
    else
    {
        s = normalize(lightPos - Position);
    }
    
    vec3 v = normalize(vec3(-Position));
    vec3 r = reflect(-s, n);
    vec4 diffuse = Kd * max(dot(s, n), 0.0);
    vec4 spec = Ks * pow(max(dot(r,v), 0.0), Shininess);
    return Ka + diffuse;// + spec;
}

vec4 ads2(int lightIndex)
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

    //return diff;
    //return spec;

    return Ka + diff + spec;
}

void main()
{
    FragColor = vec4(0.0);
    for(int i = 0; i < 10; i++)
    {
        FragColor += ads2(i);
    }
}