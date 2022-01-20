#version 400

uniform vec4 UnitGridLineColor;
uniform vec4 SubUnitGridLineColor;
uniform vec4 MajorUnitGridLineColor;
in vec3 WorldPos;
in vec2 TexCoord;
uniform mat4 ViewMatrix;

out vec4 FragColor;

float distanceToLine(float interval)
{
    float halfInterval = interval * 0.5;
    vec2 d = abs(mod(WorldPos.xz + halfInterval, vec2(interval)) - halfInterval) * 2;
    float dX = abs(mod(WorldPos.x + halfInterval, interval) - halfInterval) * 2;
    float dZ = abs(mod(WorldPos.z + halfInterval, interval) - halfInterval) * 2;
    return min(dX, dZ);
}

void main()
{
    float lineSize = 0.01f;

    float unitLineDistance = distanceToLine(1.0);
    float subUnitLineDistance = distanceToLine(1.0 / 10.0);
    float majorUnitLineDistance = distanceToLine(10.0);

    bool onUnitLine = unitLineDistance < lineSize;
    bool onSubUnitLine = subUnitLineDistance < lineSize;
    bool onMajorUnitLine = majorUnitLineDistance < lineSize;

    float unitLineInfluence = smoothstep(lineSize, lineSize * 0.5, unitLineDistance);
    float subUnitLineInfluence = smoothstep(lineSize, lineSize * 0.5, subUnitLineDistance);
    float majorUnitLineInfluence = smoothstep(lineSize, lineSize * 0.5, majorUnitLineDistance);

    if (!onUnitLine && !onSubUnitLine && !onMajorUnitLine)
    {
        discard;
    }

    FragColor = vec4(0);
    FragColor += UnitGridLineColor * unitLineInfluence;
    FragColor += SubUnitGridLineColor * subUnitLineInfluence;
    FragColor += MajorUnitGridLineColor * majorUnitLineInfluence;
    FragColor /= unitLineInfluence + subUnitLineInfluence + majorUnitLineInfluence;
    
    float closestDistance = min(unitLineDistance, subUnitLineDistance);
    closestDistance = min(closestDistance, majorUnitLineDistance);
    FragColor.a *= smoothstep(lineSize, lineSize * 0.5, closestDistance);

    // Fade the edges away
    vec2 fadeAmount = abs(TexCoord * 2 - 1);
    fadeAmount *= fadeAmount;
    fadeAmount *= fadeAmount;
    fadeAmount = vec2(1) - fadeAmount;
    FragColor.a *= fadeAmount.x * fadeAmount.y;
}