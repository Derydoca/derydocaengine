#version 400

in vec3 VertexPosition;
in vec3 VertexNormal;

out vec4 FrontColor;
out vec4 BackColor;

struct LightInfo
{
    vec4 Position;
    vec4 La;
    vec4 Ld;
    vec4 Ls;
};
uniform LightInfo Lights[10];

struct MaterialInfo
{
    vec4 Ka;
    vec4 Kd;
    vec4 Ks;
    float Shininess;
};
uniform MaterialInfo Material;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

vec4 phongModel(vec4 position, vec3 norm, int lightIndex)
{
    vec3 s = normalize(vec3(Lights[lightIndex].Position - position));
    vec3 v = normalize(-position.xyz);
    vec3 r = reflect(-s, norm);
    vec4 ambient = Lights[lightIndex].La * Material.Ka;
    float sDotN = max(dot(s, norm), 0.0);
    vec4 diffuse = Lights[lightIndex].Ld * Material.Kd * sDotN;
    vec4 spec = vec4(0.0);
    if(sDotN > 0.0)
    {
        spec = Lights[lightIndex].Ls * Material.Ks * pow(max(dot(r,v), 0.0), Material.Shininess);
    }
    return ambient + diffuse + spec;
}

void main()
{
    vec3 tnorm = normalize(NormalMatrix * VertexNormal);
    vec4 eyeCoords = ModelViewMatrix * vec4(VertexPosition, 1.0);
    FrontColor = vec4(0.0);
    BackColor = vec4(0.0);
    for(int i = 0; i < 10; i++)
    {
        FrontColor += phongModel(eyeCoords, tnorm, i);
        BackColor += phongModel(eyeCoords, -tnorm, i);
    }
    gl_Position = MVP * vec4(VertexPosition, 1.0);
}