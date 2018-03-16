#version 430

in vec3 LightDirs[10];
in vec2 TexCoord;
in vec3 ViewDir;

uniform sampler2D ColorTex;
uniform sampler2D NormalMapTex;

struct LightInfo {
  vec4 Position;  // Light position in eye coords.
  vec4 Intensity; // A,D,S intensity
};
uniform LightInfo Lights[10];

struct MaterialInfo {
  vec4 Ka;            // Ambient reflectivity
  vec4 Ks;            // Specular reflectivity
  float Shininess;    // Specular shininess factor
};
uniform MaterialInfo Material;

layout( location = 0 ) out vec4 FragColor;

vec3 phongModel(int lightIndex, vec3 norm, vec3 diffR ) {
    vec3 r = reflect( -LightDirs[lightIndex], norm );
    vec3 ambient = Lights[lightIndex].Intensity.xyz * Material.Ka.xyz;
    float sDotN = max( dot(LightDirs[lightIndex], norm), 0.0 );
    vec3 diffuse = Lights[lightIndex].Intensity.xyz * diffR * sDotN;

    vec3 spec = vec3(0.0);
    if( sDotN > 0.0 )
        spec = Lights[lightIndex].Intensity.xyz * Material.Ks.xyz *
               pow( max( dot(r,ViewDir), 0.0 ), Material.Shininess );

    return ambient + diffuse + spec;
}

void main() {
    // Lookup the normal from the normal map
    vec4 normal = 2.0 * texture( NormalMapTex, TexCoord ) - 1.0;

    vec4 texColor = texture( ColorTex, TexCoord );
    vec3 color = vec3(0);
    for(int i = 0; i < 10; i++)
    {
        color += phongModel(i, normal.xyz, texColor.rgb);
    }
    FragColor = vec4(color, 1.0);
}