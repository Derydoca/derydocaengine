#version 400

in vec3 Position;
in vec2 TexCoord;
in vec4 Color;

uniform sampler2D CharacterSheet;

out vec4 FragColor;

void main()
{
    vec4 texColor = texture(CharacterSheet, TexCoord);

    if(texColor.r < 0.5)
    {
        discard;
    }

    FragColor = Color;
}