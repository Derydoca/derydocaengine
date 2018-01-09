#version 400

in vec3 VertexPosition;
in vec3 VertexNormal;

out vec3 LightIntensity;

uniform vec3 LightPositionWorld; // Light position in world coords.
uniform vec3 LightPosition;      // Light position in eye coords.
uniform vec3 Kd;                 // Diffuse reflectivity

uniform mat4 MVP;

void main()
{
    vec3 vertEyePos = (MVP * vec4(VertexPosition, 1.0)).xyz;
    vec3 lightDirection = normalize(LightPosition - vertEyePos);
    vec3 normalDirection = normalize((MVP * vec4(VertexNormal, 0)).xyz);

    float cosTheta = dot(normalDirection, lightDirection);
    float dist = distance(vertEyePos, LightPosition);
    LightIntensity = vec3(1,1,1) * max(cosTheta, 0.0) * (cosTheta / (dist*dist));
    gl_Position = MVP * vec4(VertexPosition,1.0);
}