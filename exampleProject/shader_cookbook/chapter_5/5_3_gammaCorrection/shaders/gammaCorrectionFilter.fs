#version 400

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D RenderTex;
uniform int Width;
uniform int Height;
uniform float Gamma;

out vec4 FragColor;

void main()
{
    vec3 color = texture(RenderTex, TexCoord).xyz;
    FragColor = vec4(pow(color, vec3(1.0/Gamma)), 1.0);
}