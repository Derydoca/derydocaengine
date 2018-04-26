#version 430

struct LightInfo {
  vec4 Position;  // Light position in eye coords.
  vec4 Intensity; // A,D,S intensity
};
uniform LightInfo Lights[10];

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

vec3 diffuseModel( vec3 pos, vec3 norm, vec3 diff )
{
    vec3 s = normalize(vec3(Lights[0].Position) - pos);
    float sDotN = max( dot(s,norm), 0.0 );
    vec3 diffuse = Lights[0].Intensity.xyz * diff * sDotN;

    return diffuse;
}

void main() {
    // Retrieve position and normal information from textures
    vec3 pos = vec3( texture( PositionTex, TexCoord ) );
    vec3 norm = vec3( texture( NormalTex, TexCoord ) );
    vec3 diffColor = vec3( texture(ColorTex, TexCoord) );

    //FragColor = vec4( diffuseModel(pos,norm,diffColor), 1.0 );
    FragColor = vec4(pos.x, norm.y, diffColor.z, 1.0);
}