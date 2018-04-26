#version 430

struct LightInfo {
  vec4 Position;  // Light position in eye coords.
  vec4 Intensity; // A,D,S intensity
};
uniform LightInfo Lights[10];

struct MaterialInfo {
  vec3 Kd;            // Diffuse reflectivity
};
uniform MaterialInfo Material;

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec3 PositionData;
layout (location = 2) out vec3 NormalData;
layout (location = 3) out vec3 ColorData;

void main() {
  FragColor = vec4(1, 0, 1, 1);
  // Store position, normal, and diffuse color in textures
  PositionData = Position;
  NormalData = Normal;
  ColorData = Material.Kd;
}