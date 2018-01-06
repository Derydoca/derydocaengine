#version 400

in vec3 VertexPosition;
in vec3 VertexNormal;

out vec3 LightIntensity;

uniform vec3 LightPosition; // Light position in world coords.
uniform vec3 Kd;            // Diffuse reflectivity

uniform mat4 MVP;
uniform mat4 ModelMatrix;

void main()
{
    vec3 vertWorldPos = (ModelMatrix * vec4(VertexPosition, 1)).xyz;
    vec3 lightDirectionUnnormalized = LightPosition - vertWorldPos;
    vec3 lightDirection = normalize(lightDirectionUnnormalized);
    vec4 transformedNormal = (MVP * vec4(VertexNormal, 0));
    vec3 normalDirection = normalize(transformedNormal.xyz);

    //LightIntensity = vec3(1,1,1) * dot(normalDirection, lightDirection);
    float cosTheta = dot(VertexNormal, lightDirection);
    float dist = distance(VertexPosition, LightPosition);
    LightIntensity = vec3(1,1,1) * max(cosTheta, 0.0) * cosTheta / (dist*dist);
    gl_Position = MVP * vec4(VertexPosition,1.0);
}