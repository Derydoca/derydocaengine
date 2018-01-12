#version 400

in vec2 texCoord;
in vec3 VertexPosition;
in vec3 VertexNormal;

out vec3 vertPosition;
out vec3 vertNormal;
out vec2 texCoord0;

uniform mat4 MVP;

void main()
{
    vertPosition = VertexPosition;
    vertNormal = VertexNormal;
    texCoord0 = texCoord;
    gl_Position = MVP * vec4(VertexPosition,1.0);
}
