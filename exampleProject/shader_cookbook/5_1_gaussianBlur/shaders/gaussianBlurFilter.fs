#version 400

in vec3 Position;
in vec3 Normal;

uniform sampler2D RenderTex;

uniform int Width;
uniform int Height;

subroutine vec4 RenderPassType();
subroutine uniform RenderPassType RenderPass;

uniform float Weights[5];

out vec4 FragColor;

subroutine(RenderPassType)
vec4 pass1()
{
    ivec2 pix = ivec2(gl_FragCoord.xy);
    vec4 sum = texelFetch(RenderTex, pix, 0) * Weights[0];
    sum += texelFetchOffset(RenderTex, pix, 0, ivec2(0, 1)) * Weights[1];
    sum += texelFetchOffset(RenderTex, pix, 0, ivec2(0, -1)) * Weights[1];
    sum += texelFetchOffset(RenderTex, pix, 0, ivec2(0, 2)) * Weights[2];
    sum += texelFetchOffset(RenderTex, pix, 0, ivec2(0, -2)) * Weights[2];
    sum += texelFetchOffset(RenderTex, pix, 0, ivec2(0, 3)) * Weights[3];
    sum += texelFetchOffset(RenderTex, pix, 0, ivec2(0, -3)) * Weights[3];
    sum += texelFetchOffset(RenderTex, pix, 0, ivec2(0, 4)) * Weights[4];
    sum += texelFetchOffset(RenderTex, pix, 0, ivec2(0, -4)) * Weights[4];
    return sum;
}

subroutine(RenderPassType)
vec4 pass2()
{
    ivec2 pix = ivec2(gl_FragCoord.xy);
    vec4 sum = texelFetch(RenderTex, pix, 0) * Weights[0];
    sum += texelFetchOffset(RenderTex, pix, 0, ivec2(1, 0)) * Weights[1];
    sum += texelFetchOffset(RenderTex, pix, 0, ivec2(-1, 0)) * Weights[1];
    sum += texelFetchOffset(RenderTex, pix, 0, ivec2(2, 0)) * Weights[2];
    sum += texelFetchOffset(RenderTex, pix, 0, ivec2(-2, 0)) * Weights[2];
    sum += texelFetchOffset(RenderTex, pix, 0, ivec2(3, 0)) * Weights[3];
    sum += texelFetchOffset(RenderTex, pix, 0, ivec2(-3, 0)) * Weights[3];
    sum += texelFetchOffset(RenderTex, pix, 0, ivec2(4, 0)) * Weights[4];
    sum += texelFetchOffset(RenderTex, pix, 0, ivec2(-4, 0)) * Weights[4];
    return sum;
}

void main()
{
    FragColor = RenderPass();
    //FragColor = pass1();
    //FragColor = vec4(1);
}