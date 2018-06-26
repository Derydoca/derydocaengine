#version 410

subroutine void RenderPassType();
subroutine uniform RenderPassType RenderPass;

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexVelocity;
layout (location = 2) in float VertexStartTime;
layout (location = 3) in vec3 VertexInitialVelocity;

// To transform feedback
out vec3 Position;
out vec3 Velocity;
out float StartTime;

out float Transp; // To frag shader

uniform float Time;
uniform float H; // delta time
uniform vec3 Accel;
uniform float ParticleLifetime;
uniform vec3 EmitterPosition;

uniform mat4 MVP;

subroutine(RenderPassType)
void update()
{
    // Update pos & vel for next frame
    Position = VertexPosition;
    Velocity = VertexVelocity;
    StartTime = VertexStartTime;

    if (Time >= StartTime)
    {
        float age = Time - StartTime;

        if (age > ParticleLifetime)
        {
            // The particle is dead and is ready to be recycled
            Position = EmitterPosition;
            Velocity = VertexInitialVelocity;
            StartTime = Time;
        }
        else
        {
            // The particle is alive so update it
            Position += Velocity * H;
            Velocity += Accel * H;
        }
    }
}

subroutine (RenderPassType)
void render()
{
    float age = Time - VertexStartTime;
    Transp = 1.0 - age / ParticleLifetime;
    gl_Position = MVP * vec4(VertexPosition, 1.0);
}

void main()
{
    RenderPass();
}
