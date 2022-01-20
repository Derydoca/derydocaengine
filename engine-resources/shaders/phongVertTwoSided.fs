#version 400

in vec4 FrontColor;
in vec4 BackColor;

out vec4 FragColor;

void main()
{
    if(gl_FrontFacing)
    {
        FragColor = FrontColor;
    }
    else
    {
        FragColor = BackColor;
    }
}
