#version 440

#define M_PI 3.1415926535897932384626433832795

in vec4 position;
out vec4 fs_color;

layout (location=0) uniform mat4 transformation;
layout (location=1) uniform bool colorful;
layout (location=2) uniform float minimumValue;
layout (location=3) uniform float maximumValue;

void main() {
    float flatColor = (((position.z - minimumValue)*1)/(maximumValue-minimumValue)) ;
    float blueColor = sin(2*M_PI*flatColor) * 0.5 + 0.5;
    float greenColor = sin(2*(M_PI*flatColor - M_PI/4)) * 0.5 + 0.5;
    float redColor = sin(2*(M_PI*flatColor - M_PI/2)) * 0.5 + 0.5;

    if(blueColor < 0.51)
        blueColor=0;
    if(redColor < 0.51)
        redColor=0;
    if(greenColor < 0.51)
        greenColor=0;

    if(colorful)
        fs_color.xyzw = vec4(redColor, greenColor, blueColor, 1.0);
    else
        fs_color.xyzw = vec4(position.z, position.z, position.z, 1.0);
    gl_Position = transformation*position;
}
