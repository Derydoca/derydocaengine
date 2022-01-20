#version 400
in vec4 LightIntensity;
out vec4 FragColor;

void main()
{
    FragColor = vec4(vec3(LightIntensity), 1.0);
}