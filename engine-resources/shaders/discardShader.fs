#version 400

in vec4 FrontColor;
in vec4 BackColor;
in vec2 TexCoord;

out vec4 FragColor;

void main()
{
    const float scale = 30.0;
    bvec2 toDiscard = greaterThan(fract(TexCoord * scale), vec2(0.2, 0.2));
    if(all(toDiscard))
    {
        discard;
    }
    if (gl_FrontFacing)
    {
        FragColor = FrontColor;
    }
    else
    {
        FragColor = BackColor;
    }
}