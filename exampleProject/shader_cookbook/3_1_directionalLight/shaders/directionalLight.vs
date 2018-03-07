#version 400

in vec3 VertexPosition;
in vec3 VertexNormal;

out vec4 Color;

struct LightInfo {
    int Type;
    vec4 Position;
    vec4 Intensity;
};
uniform LightInfo Lights[10];

//uniform vec4 LightPosition;
uniform vec4 LightIntensity;
uniform vec4 Kd;
uniform vec4 Ka;
uniform vec4 Ks;
uniform float Shininess;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

vec4 adsDirectional(int lightIndex, vec4 position, vec3 norm)
{
    //Lights[lightIndex].Intensity
    vec3 s;
    if(Lights[lightIndex].Type == 0)
    {
        s = normalize(vec3(Lights[lightIndex].Position));
    }
    else
    {
        s = normalize(vec3(Lights[lightIndex].Position - position));
    }

    vec3 v = normalize(vec3(-position));
    vec3 r = reflect(-s, norm);
    return Lights[lightIndex].Intensity * (Ka + Kd * max(dot(s, norm), 0.0) + Ks * pow(max(dot(r, v), 0.0), Shininess));
}

void main()
{
    vec3 eyeNorm = normalize(NormalMatrix * VertexNormal);
    vec4 eyePosition = ModelViewMatrix * vec4(VertexPosition, 1.0);
    
    Color = vec4(0.0);
    for(int i = 0; i < 10; i++)
    {
        Color += adsDirectional(i, eyePosition, eyeNorm);
    }
    gl_Position = MVP * vec4(VertexPosition, 1.0);
}
