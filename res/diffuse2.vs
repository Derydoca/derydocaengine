#version 400

in vec3 VertexPosition;
in vec3 VertexNormal;

struct LightInfo{
    vec3 Position; // Light position in eye coords
    vec3 Intensity; // Light intensity
};
uniform LightInfo lights[5];

out vec3 LightIntensity;

uniform mat4 MVP;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;

vec3 ads(int lightIndex)
{

    vec3 tnorm = normalize(NormalMatrix * VertexNormal);
    vec3 eyeCoords = (ModelViewMatrix * vec4(VertexPosition, 1.0)).xyz;
    vec3 s = normalize(lights[lightIndex].Position - eyeCoords);

    float cosTheta = dot(s, tnorm);
    float dist = distance(eyeCoords, lights[lightIndex].Position.xyz);
    return lights[lightIndex].Intensity * max(cosTheta, 0.0) * (cosTheta / (dist*dist));
}

void main()
{
    LightIntensity = vec3(0.0);
    for(int i = 0; i < 5; i++)
    {
        LightIntensity += ads(i);
    }

    gl_Position = MVP * vec4(VertexPosition,1.0);
}
