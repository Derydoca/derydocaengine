#version 400

in vec3 VertexPosition;
in vec3 VertexNormal;
in ivec4 VertexBoneIndices;
in vec4 VertexBoneWeights;

out vec4 Color;

struct LightInfo {
    vec4 Position;
    vec4 Intensity;
};
uniform LightInfo Lights[10];

const uint MaxBoneCount = 150;
uniform mat4 BoneMatrices[MaxBoneCount];

uniform vec4 Kd;
uniform vec4 Ka;
uniform vec4 Ks;
uniform float Shininess;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

vec4 ads(int lightIndex, vec4 position, vec3 norm)
{
    vec3 s = normalize(vec3(Lights[lightIndex].Position - position));
    vec3 v = normalize(vec3(-position));
    vec3 r = reflect(-s, norm);
    vec4 I = Lights[lightIndex].Intensity;
    return I * (Ka + Kd * max(dot(s, norm), 0.0) + Ks * pow(max(dot(r,v), 0.0), Shininess));
}

void main()
{
    mat4 boneTransform = BoneMatrices[VertexBoneIndices[0]] * VertexBoneWeights[0];
    for(int i = 1; i < 4; i++)
    {
        int boneIndex = VertexBoneIndices[i];
        if(boneIndex > 0)
        {
            boneTransform += BoneMatrices[boneIndex] * VertexBoneWeights[i];
        }
    }

    vec3 eyeNorm = normalize(NormalMatrix * (boneTransform * vec4(VertexNormal, 0.0)).xyz);
    vec4 eyePosition = ModelViewMatrix * vec4(VertexPosition, 1.0);
    Color = vec4(0.0);
    for(int i = 0; i < 10; i++)
    {
       Color += ads(i, eyePosition, eyeNorm);
    }
    gl_Position = MVP * (boneTransform * vec4(VertexPosition, 1.0));
}
