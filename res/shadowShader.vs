#version 330

attribute vec3 position;

uniform mat4 transform;
unifrom mat4 u_shadowViewMat;
uniform mat4 u_shadowProjMat;

void main(void)
{
    gl_Position = u_shadowProjMat * u_shadowViewMat * u_modelMat * vec4(i_position, 1);
}