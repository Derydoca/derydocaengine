#version 430

struct MaterialInfo {
  vec4 Kd;            // Diffuse reflectivity
};
uniform MaterialInfo Material;

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

layout (location = 1) out vec3 PositionData;
layout (location = 2) out vec3 NormalData;
layout (location = 3) out vec3 ColorData;

void main() {
  // Store position, normal, and diffuse color in textures
  PositionData = Position;
  NormalData = Normal;
  ColorData = Material.Kd.xyz;
}