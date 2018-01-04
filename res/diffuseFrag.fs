#version 410

varying vec4 position;
varying vec3 normal;

uniform vec4 LightPosition; // Light position in eye coords. ??
uniform vec3 Kd;            // Diffuse reflectivity
uniform vec3 Ld;            // Diffuse light intensity

uniform mat3 NormalMatrix;
uniform mat4 MVP;

out vec4 FragColor;

void main()
{
    //vec3 tnorm = normalize( NormalMatrix * normal);
    vec3 tnorm = normalize(normal);
    vec4 eyeCoords = MVP * position;
    //eyeCoords = vec3(eyeCoords.x, eyeCoords.y, eyeCoords.z, eyeCoords.w);
    vec3 s = normalize(vec3(LightPosition - eyeCoords));
    //vec3 s = normalize(vec3(LightPosition - position)); // 'Specular' is square

    vec3 lightIntensity = Ld * Kd * max( dot( s, tnorm ),0.0);
    FragColor = vec4(lightIntensity, 1);
    //FragColor = vec4(1, 0, 0, 1);
}