#version 400

struct LineInfo {
    float Width;
    vec4 Color;
};
uniform LineInfo Line;

uniform vec4 QuadColor;

noperspective in vec3 EdgeDistance;

out vec4 FragColor;

void main()
{
    float d = min(EdgeDistance.x, EdgeDistance.y);
    d = min(d, EdgeDistance.z);

    float mixVal;
    if( d < Line.Width - 1 ) {
        mixVal = 1.0;
    } else if( d > Line.Width + 1 ) {
        mixVal = 0.0;
    } else {
        float x = d - (Line.Width - 1);
        mixVal = exp2(-2.0 * (x*x));
    }
    
    FragColor = mix(QuadColor, Line.Color, mixVal);
}