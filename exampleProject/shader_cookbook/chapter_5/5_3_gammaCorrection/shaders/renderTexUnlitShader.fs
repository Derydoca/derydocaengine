#version 400

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D RenderTex; // Render texture

out vec4 FragColor;

void main()
{
    FragColor = texture(RenderTex, TexCoord);
}