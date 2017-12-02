#version 330

varying vec3 texCoord0;

//out vec4 diffuse;

uniform samplerCube cubeMap;

void main(void)
{
    //gl_FragColor = texture(cubeMap, texCoord0);
    gl_FragColor = (vec4(texCoord0, 1.0) / 2) + (texture(cubeMap, texCoord0) / 2);
}