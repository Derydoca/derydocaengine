#version 400

in vec2 TexCoord;

uniform sampler2D NoiseTexture;

out vec4 FragColor;

void main()
{
    vec4 texColor = texture(NoiseTexture, TexCoord);

    FragColor = vec4(texColor.aaa, 1.0);
}