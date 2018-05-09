#version 400

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D RenderTex;
uniform sampler2D BlurTex;
uniform sampler2D BlurTex2;
uniform int Width;
uniform int Height;
uniform float LumThresh;

subroutine vec4 RenderPassType();
subroutine uniform RenderPassType RenderPass;

out vec4 FragColor;

uniform float PixOffset[10] = float[] (0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0);
uniform float Weights[10];

float luma(vec3 color)
{
    return 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
}

subroutine(RenderPassType)
vec4 lumThreshPass()
{
    vec4 val = texture(RenderTex, TexCoord);
    if(luma(val.rgb) > LumThresh)
    {
        return val;
    }
    else
    {
        return vec4(0);
    }
}

subroutine(RenderPassType)
vec4 blurY()
{
    float dy = 1.0 / float(Height);
    vec4 sum = texture(BlurTex, TexCoord) * Weights[0];
    for(int i = 1; i < 10; i++)
    {
        sum += texture(BlurTex, TexCoord + vec2(0.0, PixOffset[i]) * dy) * Weights[i];
        sum += texture(BlurTex, TexCoord - vec2(0.0, PixOffset[i]) * dy) * Weights[i];
    }
    return sum;
}

subroutine(RenderPassType)
vec4 blurX()
{
    float dx = 1.0 / float(Width);
    vec4 val = texture(RenderTex, TexCoord);
    vec4 sum = texture(BlurTex2, TexCoord) * Weights[0];
    for(int i = 1; i < 10; i++)
    {
        sum += texture(BlurTex2, TexCoord + vec2(PixOffset[i], 0.0) * dx) * Weights[i];
        sum += texture(BlurTex2, TexCoord - vec2(PixOffset[i], 0.0) * dx) * Weights[i];
    }
    return val + sum;
}

void main()
{
    FragColor = RenderPass();
}