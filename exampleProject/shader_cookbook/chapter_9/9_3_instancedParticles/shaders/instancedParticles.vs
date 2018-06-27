#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec3 VertexTexCoord;
layout (location = 3) in vec3 VertexInitialVelocity; // World coords
layout (location = 4) in float StartTime;

out vec3 Position;
out vec3 Normal;

uniform float Time;
uniform vec3 Gravity; // World coords
uniform float ParticleLifetime;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

void main()
{
    // Work in camera coords
    Position = (ModelViewMatrix * vec4(VertexPosition, 1.0)).xyz;
    Normal = NormalMatrix * VertexNormal;
    vec3 g = NormalMatrix * Gravity;
    vec3 v0 = NormalMatrix * VertexInitialVelocity;

    if(Time > StartTime)
    {
        float t = Time - StartTime;

        if(t < ParticleLifetime)
        {
            Position += v0 * t + g * t * t;
        }
    }

    // Draw the current position
    gl_Position = ProjectionMatrix * vec4(Position, 1.0);
}