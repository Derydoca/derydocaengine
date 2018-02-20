#version 120

attribute vec3 VertexPosition;
attribute vec2 texCoord;
attribute vec3 VertexNormal;

varying vec2 texCoord0;
varying vec3 normal0;

uniform mat4 MVP;

void main() {
    gl_Position = MVP * vec4(VertexPosition, 1.0);
    texCoord0 = texCoord;
    normal0 = (MVP * vec4(VertexNormal, 0.0)).xyz;
}