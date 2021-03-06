#version 410

subroutine void RenderPassType();
subroutine uniform RenderPassType RenderPass;

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexVelocity;
layout (location = 2) in float VertexStartTime;
layout (location = 3) in vec3 VertexInitialVelocity;
layout (location = 4) in vec3 VertexInitialPosition;

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
uniform float MinParticleSize;
uniform float MaxParticleSize;

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
            Position = VertexInitialPosition;
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
    gl_Position = MVP * vec4(VertexPosition, 1.0);
    gl_PointSize = 0.0;
    float age = Time - VertexStartTime;
    float agePct = age / ParticleLifetime;
    if(agePct > 0 && agePct <= 1.0) {
        gl_PointSize = mix(MinParticleSize, MaxParticleSize, agePct) / gl_Position.w;
    }
    Transp = 1.0 - age / ParticleLifetime;
}

void main()
{
    RenderPass();
}
