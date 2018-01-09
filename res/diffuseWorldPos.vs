#version 400

in vec3 VertexPosition;
in vec3 VertexNormal;

out vec3 LightIntensity;

uniform vec3 LightPositionWorld; // Light position in world coords.
uniform vec3 LightPosition;      // Light position in eye coords.
uniform vec3 Kd;                 // Diffuse reflectivity

uniform mat4 MVP;
uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;

void main()
{
    vec4 v4verpos = vec4(VertexPosition.x, VertexPosition.y, VertexPosition.z, 1.0);
    vec4 v4mult = ModelMatrix * v4verpos;
    vec3 vertWorldPos = v4mult.xyz;
    vec3 lightDirectionUnnormalized = LightPositionWorld - vertWorldPos;
    vec3 lightDirection = normalize(lightDirectionUnnormalized);
    vec4 transformedNormal = (MVP * vec4(VertexNormal, 0));
    vec3 normalDirection = normalize(transformedNormal.xyz);

    float cosTheta = dot(VertexNormal, lightDirection);
    float dist = distance(VertexPosition, LightPositionWorld);
    LightIntensity = vec3(1,1,1) * max(cosTheta, 0.0) * (cosTheta / (dist*dist));
    gl_Position = MVP * v4verpos;//MVP * vec4(VertexPosition,1.0);
}