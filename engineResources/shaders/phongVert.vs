#version 400

in vec3 VertexPosition;
in vec3 VertexNormal;

out vec4 LightIntensity;

struct LightInfo {
    vec4 Position; // Eye coords
    vec4 La; // Ambient intensity
    vec4 Ld; // Diffuse intensity
    vec4 Ls; // Specular intensity
};
uniform LightInfo lights[10];

struct MaterialInfo {
    vec4 Ka; // Ambient
    vec4 Kd; // Diffuse
    vec4 Ks; // Specular
    float Shininess; // Specular power
};
uniform MaterialInfo Material;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

vec4 phong(int lightIndex)
{
    vec3 tnorm = normalize(NormalMatrix * VertexNormal);
    vec4 eyeCoords = ModelViewMatrix * vec4(VertexPosition, 1.0);
    vec3 s = normalize(vec3(lights[lightIndex].Position - eyeCoords));
    vec3 v = normalize(-eyeCoords.xyz);
    vec3 r = reflect(-s, tnorm);
    vec4 ambient = lights[lightIndex].La * Material.Ka;
    float sDotN = max(dot(s, tnorm), 0.0);
    vec4 diffuse = lights[lightIndex].Ld * Material.Kd * sDotN;
    vec4 spec = vec4(0.0);
    if(sDotN > 0.0)
    {
        spec = lights[lightIndex].Ls * Material.Ks * pow(max(dot(r,v), 0.0), Material.Shininess);
    }
    return ambient + diffuse + spec;
}

void main()
{
    LightIntensity = vec4(0.0);
    for(int i = 0; i < 10; i++)
    {
        LightIntensity += phong(i);
    }

    gl_Position = MVP * vec4(VertexPosition, 1.0);
}