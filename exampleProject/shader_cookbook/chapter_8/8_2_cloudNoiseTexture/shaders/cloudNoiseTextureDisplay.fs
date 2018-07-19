#version 400
#define PI 3.14159265

in vec2 TexCoord;

uniform sampler2D NoiseTexture;
uniform float Scale;
uniform vec4 SkyColor;
uniform vec4 CloudColor;

out vec4 FragColor;

void main()
{
    vec4 noise = texture(NoiseTexture, TexCoord * Scale);
    float t = (cos(noise.g * PI) + 1.0) / 2.0;
    vec4 color = mix(SkyColor, CloudColor, t);
    
    FragColor = vec4(color.rgb, 1.0);
}