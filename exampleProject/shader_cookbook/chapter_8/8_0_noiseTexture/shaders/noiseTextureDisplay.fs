#version 400

in vec2 TexCoord;

uniform sampler2D NoiseTexture;
uniform float Scale;

out vec4 FragColor;

void main()
{
    vec4 texColor = texture(NoiseTexture, TexCoord * Scale);

    FragColor = vec4(texColor.aaa, 1.0);
}