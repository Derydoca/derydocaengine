#version 400

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D NoiseTexture;
uniform sampler2D RenderTex;
uniform int Width;
uniform int Height;
uniform float Radius;

out vec4 FragColor;

float luminance(vec3 color)
{
    return dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));
}

void main()
{
    vec4 noise = texture(NoiseTexture, TexCoord);
    vec4 color = texture(RenderTex, TexCoord);
    float green = luminance(color.rgb);

    float dist1 = length(gl_FragCoord.xy - vec2(Width / 4.0, Height / 2.0));
    float dist2 = length(gl_FragCoord.xy - vec2(3.0 * Width / 4.0, Height / 2.0));

    if(dist1 > Radius && dist2 > Radius)
    {
        green = 0.0;
    }

    FragColor = vec4(0.0, green * clamp(noise.a, 0.0, 1.0), 0.0, 1.0);
}