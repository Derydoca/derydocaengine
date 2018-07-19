#version 400
#define PI 3.14159265

in vec2 TexCoord;

uniform sampler2D NoiseTexture;
uniform float Scale;
uniform vec4 DarkWoodColor;
uniform vec4 LightWoodColor;
uniform mat4 Slice;

out vec4 FragColor;

void main()
{
    // Transform the texture coords to define the "slice" of the log
    vec4 cyl = Slice * vec4(TexCoord.st, 0.0, 1.0);

    // The distance from the log's y axis
    float dist = length(cyl.xz);

    // Perturb the distance using the noise texture
    vec4 noise = texture(NoiseTexture, TexCoord * Scale);
    dist += noise.b;

    // Determine the color as a mix of the light and dark wood colors
    float t = 1.0 - abs(fract(dist) * 2.0 - 1.0);
    t = smoothstep(0.2, 0.5, t);
    vec4 color = mix(DarkWoodColor, LightWoodColor, t);
    
    FragColor = vec4(color.rgb, 1.0);
}