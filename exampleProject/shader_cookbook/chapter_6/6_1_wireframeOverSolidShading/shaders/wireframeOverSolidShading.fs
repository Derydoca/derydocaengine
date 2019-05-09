#version 400

struct MaterialInfo {
    vec4 Kd;
    vec4 Ka;
    vec4 Ks;
    float Shininess;
};
uniform MaterialInfo Material;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ModelMatrix;
uniform mat4 MVP;

struct Light {
    vec4 Direction;
    vec4 Position;
    vec4 Intensity;
    int Type;
    float Cutoff;
    float Exponent;
    float _padding;
};
layout (std140) uniform LightCollection
{
    Light Lights[10];
    int NumLights;
};

struct LineInfo {
    float Width;
    vec4 Color;
};
uniform LineInfo Line;

in vec3 GPosition;
in vec3 GNormal;

noperspective in vec3 GEdgeDistance;

out vec4 FragColor;

vec4 ads(int lightIndex)
{
    vec3 lightPos = vec3(Lights[lightIndex].Position);

    vec3 l;
    if(Lights[lightIndex].Type == 0)
    {
        // Ignore directional lights for now
        return vec4(0);
    }
    else if(Lights[lightIndex].Type == 1)
    {
        l = normalize(lightPos - GPosition);
    }

    vec3 n = normalize(GNormal);
    
    float lDotN = dot(l, n);
    vec4 diff = Lights[lightIndex].Intensity * Material.Kd * max(lDotN, 0.0);

    vec3 v = normalize(-GPosition);
    
    vec3 r = reflect(-l,n);
    vec4 spec;
    if(lDotN > 0.0)
    {
        spec = vec4(pow(max(dot(r, v), 0.0), Material.Shininess));
    }

    return Material.Ka + diff + spec;
}

void main()
{
    vec4 color = vec4(0.0);
    for(int i = 0; i < NumLights; i++)
    {
        color += ads(i);
    }

    float d = min(GEdgeDistance.x, GEdgeDistance.y);
    d = min(d, GEdgeDistance.z);

    float mixVal;
    if( d < Line.Width - 1 ) {
        mixVal = 1.0;
    } else if( d > Line.Width + 1 ) {
        mixVal = 0.0;
    } else {
        float x = d - (Line.Width - 1);
        mixVal = exp2(-2.0 * (x*x));
    }
    
    FragColor = mix(color, Line.Color, mixVal);
}