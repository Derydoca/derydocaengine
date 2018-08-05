#version 400

in vec3 VertexPosition;
in vec2 VertexTexCoord;
in vec4 VertexColor;

out vec3 Position;
out vec2 TexCoord;
out vec4 Color;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

void main()
{
    TexCoord = VertexTexCoord;
    Color = VertexColor;
    Position = vec3(ModelViewMatrix * vec4(VertexPosition, 1.0));
    gl_Position = MVP * vec4(VertexPosition, 1.0);
}