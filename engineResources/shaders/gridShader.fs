#version 400

uniform vec4 UnitGridLineColor;
uniform vec4 SubUnitGridLineColor;
uniform vec4 MajorUnitGridLineColor;
in vec3 WorldPos;
in vec3 Normal;
in vec2 TexCoord;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec4 FragColor;

bool isOnLine(float lineSize, float interval)
{
    return mod(WorldPos.x + lineSize * 0.5, interval) < lineSize || mod(WorldPos.z + lineSize * 0.5, interval) < lineSize;
}

void main()
{
    float alignment = abs(dot(Normal, vec3(ViewMatrix[1].xyz)));
    float lineSize = 0.01f;

    bool onUnitLine = isOnLine(lineSize, 1.0);
    bool onSubUnitLine = isOnLine(lineSize / 2, 1.0 / 10.0);
    bool onMajorUnitLine = isOnLine(lineSize, 10.0);

    if (!onUnitLine && !onSubUnitLine && !onMajorUnitLine)
    {
        discard;
    }
    
    if(onMajorUnitLine)
    {
        FragColor = MajorUnitGridLineColor;
    }
    else if(onUnitLine)
    {
        FragColor = UnitGridLineColor;
    }
    else if(onSubUnitLine)
    {
        FragColor = SubUnitGridLineColor;
    }
    else
    {
        FragColor = vec4(0, 0, 0, 0.3);
    }

    // Fade the edges away
    FragColor.a *= 1 - abs((TexCoord.x - 0.5) * 2);
    FragColor.a *= 1 - abs((TexCoord.y - 0.5) * 2);
}