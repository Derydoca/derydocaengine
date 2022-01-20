#version 410

in vec3 VertexPosition;
in vec3 VertexNormal;
varying vec2 texCoord0;

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

layout( location = 0 ) out vec4 FragColor;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform sampler2D diffuse;

vec3 ads(int lightIndex)
{
    if(Lights[lightIndex].Type == 0)
    {
        vec3 tnorm = normalize(NormalMatrix * vertNormal);
        float cosTheta = dot(vec3(Lights[lightIndex].Direction), tnorm);
        return Lights[lightIndex].Intensity * max(cosTheta, 0.0) * 3.0;
    }
    else if(Lights[lightIndex].Type == 1)
    {
        vec3 tnorm = normalize(NormalMatrix * vertNormal);
        vec3 eyeCoords = (ModelViewMatrix * vec4(vertPosition, 1.0)).xyz;
        vec3 s = normalize(Lights[lightIndex].Position.xyz - eyeCoords);

        float cosTheta = dot(s, tnorm);
        float dist = distance(eyeCoords, Lights[lightIndex].Position.xyz);
        return Lights[lightIndex].Intensity * max(cosTheta, 0.0) * (cosTheta / (dist*dist));
    }
}

void main() {

    vec3 color = vec3(0.0);
    for(int i = 0; i < NumLights; i++)
    {
        color += ads(i);
    }

    FragColor = texture2D(diffuse, texCoord0) * vec4(color, 1);
}