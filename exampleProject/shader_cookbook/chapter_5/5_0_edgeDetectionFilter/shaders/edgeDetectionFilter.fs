#version 400

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D RenderTex; // Render texture
uniform float EdgeThreshold; // Squared threshold value
uniform int Width; // Pixel width
uniform int Height; // Pixel height

subroutine vec4 RenderPassType();
subroutine uniform RenderPassType RenderPass;

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

subroutine(RenderPassType)
vec4 edgeDetection()
{
    float dx = 1.0 / float(Width);
    float dy = 1.0 / float(Height);

    float s00 = luma(texture(RenderTex, TexCoord + vec2(-dx, dy)).rgb);
    float s10 = luma(texture(RenderTex, TexCoord + vec2(-dx, 0.0)).rgb);
    float s20 = luma(texture(RenderTex, TexCoord + vec2(-dx, -dy)).rgb);
    float s01 = luma(texture(RenderTex, TexCoord + vec2(0, dy)).rgb);
    float s21 = luma(texture(RenderTex, TexCoord + vec2(0, -dy)).rgb);
    float s02 = luma(texture(RenderTex, TexCoord + vec2(dx, dy)).rgb);
    float s12 = luma(texture(RenderTex, TexCoord + vec2(dx, 0.0)).rgb);
    float s22 = luma(texture(RenderTex, TexCoord + vec2(dx, -dy)).rgb);

    float sx = s00 + 2 * s10 + s20 - (s02 + 2 * s12 + s22);
    float sy = s00 + 2 * s01 + s02 - (s20 + 2 * s21 + s22);

    float dist = sx * sx + sy * sy;

    if(dist > EdgeThreshold)
    {
        return vec4(1.0);
    }
    else
    {
        return vec4(0.0, 0.0, 0.0, 1.0);
    }
}

void main()
{
    FragColor = edgeDetection();
}