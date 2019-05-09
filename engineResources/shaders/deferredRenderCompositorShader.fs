#version 430

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

struct MaterialInfo {
  vec3 Kd;            // Diffuse reflectivity
};
uniform MaterialInfo Material;

uniform sampler2D PositionTex;
uniform sampler2D NormalTex;
uniform sampler2D ColorTex;

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

vec3 diffuseModel(int lightIndex, vec3 pos, vec3 norm, vec3 diff )
{
    vec3 s = normalize(vec3(Lights[lightIndex].Position) - pos);
    float sDotN = max( dot(s,norm), 0.0 );
    vec3 diffuse = Lights[lightIndex].Intensity.xyz * diff * sDotN;

    return diffuse;
}

void main() {
  // Retrieve position and normal information from textures
  vec3 pos = vec3( texture( PositionTex, TexCoord ) );
  vec3 norm = vec3( texture( NormalTex, TexCoord ) );
  vec3 diffColor = vec3( texture(ColorTex, TexCoord) );

  vec3 diff = vec3(0);
  for(int i = 0; i < NumLights; i++)
  {
    diff += diffuseModel(i, pos, norm, diffColor);
  }
  FragColor = vec4(diff, 1.0 );
}