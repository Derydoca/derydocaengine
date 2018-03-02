#version 410

in vec3 vertPosition;
in vec3 vertNormal;
varying vec2 texCoord0;

struct LightInfo {
    int Type;
    vec3 Direction;
    vec3 Position; // Light position in eye coords
    vec3 Ld; // Light intensity
};
uniform LightInfo Lights[10];

layout( location = 0 ) out vec4 FragColor;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform sampler2D diffuse;

vec3 ads(int lightIndex)
{
    if(Lights[lightIndex].Type == 0)
    {
        vec3 tnorm = normalize(NormalMatrix * vertNormal);
        float cosTheta = dot(Lights[lightIndex].Direction, tnorm);
        return Lights[lightIndex].Ld * max(cosTheta, 0.0) * 3.0;
    }
    else if(Lights[lightIndex].Type == 1)
    {
        vec3 tnorm = normalize(NormalMatrix * vertNormal);
        vec3 eyeCoords = (ModelViewMatrix * vec4(vertPosition, 1.0)).xyz;
        vec3 s = normalize(Lights[lightIndex].Position - eyeCoords);

        float cosTheta = dot(s, tnorm);
        float dist = distance(eyeCoords, Lights[lightIndex].Position.xyz);
        return Lights[lightIndex].Ld * max(cosTheta, 0.0) * (cosTheta / (dist*dist));
    }
}

void main() {

    vec3 color = vec3(0.0);
    for(int i = 0; i < 5; i++)
    {
        color += ads(i);
    }

    FragColor = texture2D(diffuse, texCoord0) * vec4(color, 1);
}