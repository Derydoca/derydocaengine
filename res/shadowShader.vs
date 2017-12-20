#version 330

attribute vec3 VertexPosition;

uniform mat4 transform;
uniform mat4 u_shadowViewMat;
uniform mat4 u_shadowProjMat;
uniform mat4 u_modelMat;

void main(void)
{
    gl_Position = u_shadowProjMat * u_shadowViewMat * u_modelMat * vec4(VertexPosition, 1);
}