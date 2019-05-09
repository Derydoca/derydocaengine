#version 400

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D RenderTex; // Render texture

struct MaterialInfo
{
    vec4 Kd;
    vec4 Ks;
    vec4 Ka;
    float Shininess;
};
uniform MaterialInfo Material;

out vec4 FragColor;

float luma(vec3 color)
{
    return 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
}

void main()
{
    FragColor = vec4(vec3(luma(texture(RenderTex, TexCoord).rgb)), 1);
}