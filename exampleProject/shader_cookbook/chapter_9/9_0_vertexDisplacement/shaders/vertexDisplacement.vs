#version 400

in vec3 VertexPosition;

out vec3 Position;
out vec3 Normal;

uniform float Time;

// Wave parameters
uniform float K;
uniform float Velocity;
uniform float Amp;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

void main()
{
    vec4 pos = vec4(VertexPosition, 1.0);

    // Translate the y coord
    float u = K * (pos.x - Velocity * Time);
    pos.y = Amp * sin(u);

    // Compute the normal vector
    vec3 n = vec3(0.0);
    n.xy = normalize(vec2(-K * Amp * cos(u), 1.0));

    // Send position and normal (in camera coords) to fragment shader
    Position = (ModelViewMatrix * pos).xyz;
    Normal = NormalMatrix * n;

    // The position in clip coords
    gl_Position = MVP * pos;
}