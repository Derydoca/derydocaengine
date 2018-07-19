#version 400

in vec4 LightIntensity;

out vec4 FragColor;

void main()
{
    FragColor = LightIntensity;
}