#version 400

in vec3 ReflectDir;

uniform samplerCube CubeMapTex;

uniform bool DrawSkyBox;
uniform float ReflectFactor;
uniform vec4 MaterialColor;

out vec4 FragColor;

void main()
{
    vec4 cubeMapColor = texture(CubeMapTex, ReflectDir);

    if(DrawSkyBox)
    {
        FragColor = cubeMapColor;
    }
    else
    {
        FragColor = mix(MaterialColor, cubeMapColor, ReflectFactor);
    }
}