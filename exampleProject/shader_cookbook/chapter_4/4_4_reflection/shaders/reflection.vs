#version 400

in vec3 VertexPosition;
in vec3 VertexNormal;
in vec2 VertexTexCoord;

out vec3 ReflectDir; // Direction of the reflected ray

uniform bool DrawSkyBox;
uniform vec3 WorldCameraPosition;

uniform mat4 ModelViewMatrix;
uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

void main()
{
    if(DrawSkyBox)
    {
        ReflectDir = VertexPosition;
    }
    else
    {
        // Compute the reflected direction in world coords
        vec3 worldPos = vec3(ModelMatrix * vec4(VertexPosition, 1.0));
        vec3 worldNorm = vec3(ModelMatrix * vec4(VertexNormal, 0.0));
        vec3 worldView = normalize(WorldCameraPosition - worldPos);

        ReflectDir = reflect(-worldView, worldNorm);
    }

    gl_Position = MVP * vec4(VertexPosition, 1.0);
}