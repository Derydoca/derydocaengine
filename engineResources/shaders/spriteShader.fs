#version 400

in vec3 Position;
in vec2 TexCoord;
in vec4 Color;

uniform sampler2D SpriteSheet;

out vec4 FragColor;

void main()
{
    vec4 texColor = texture(SpriteSheet, TexCoord);

    if(texColor.a < 0.5)
    {
        discard;
    }

    FragColor = texColor * Color;
}