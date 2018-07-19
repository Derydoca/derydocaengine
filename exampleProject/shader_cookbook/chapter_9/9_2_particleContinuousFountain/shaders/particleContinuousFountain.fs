#version 410

in float Transp;

uniform sampler2D ParticleTex;

out vec4 FragColor;

void main()
{
    FragColor = texture(ParticleTex, gl_PointCoord);
    if(FragColor.a == 0.0)
    {
        discard;
    }
    FragColor.a *= Transp;
}
