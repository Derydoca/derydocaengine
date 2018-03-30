#version 400

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D RenderTex;
uniform int Width;
uniform int Height;
uniform float PixOffset[5] = float[] (0.0, 1.0, 2.0, 3.0, 4.0);
uniform float Weights[5];

subroutine vec4 RenderPassType();
subroutine uniform RenderPassType RenderPass;

out vec4 FragColor;

subroutine(RenderPassType)
vec4 pass1()
{
    float dy = 1.0 / float(Height);
    vec4 sum = texture(RenderTex, TexCoord) * Weights[0];
    for(int i = 1; i < 5; i++)
    {
        sum += texture(RenderTex, TexCoord + vec2(0.0, PixOffset[i]) * dy) * Weights[i];
        sum += texture(RenderTex, TexCoord - vec2(0.0, PixOffset[i]) * dy) * Weights[i];
    }
    return sum;
}

subroutine(RenderPassType)
vec4 pass2()
{
    return vec4(1);
    float dx = 1.0 / float(Width);
    vec4 sum = texture(RenderTex, TexCoord) * Weights[0];
    for(int i = 1; i < 5; i++)
    {
        sum += texture(RenderTex, TexCoord + vec2(PixOffset[i], 0.0) * dx) * Weights[i];
        sum += texture(RenderTex, TexCoord - vec2(PixOffset[i], 0.0) * dx) * Weights[i];
    }
    return sum;
}

void main()
{
    FragColor = RenderPass();
    //FragColor = pass1();
}