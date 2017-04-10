#version 400

#define M_PI 3.1415926535897932384626433832795

in vec4 position;
out vec4 fs_color;

layout (location=0) uniform mat4 transformation;
layout (location=1) uniform bool colorful;
layout (location=2) uniform float minimumValue;
layout (location=3) uniform float maximumValue;

void main() {
    const float floatEps=0.000001;

    float differenceBetweenMinimum = position.z - minimumValue;
    if(differenceBetweenMinimum < floatEps) // corner case
        differenceBetweenMinimum=floatEps;

    float flatColor = (((differenceBetweenMinimum))/(maximumValue-minimumValue)) ;

    if(flatColor > (1.0f - floatEps)) // corner case
        flatColor=(1.0f - floatEps);

    float blueColor = sin(2*M_PI*flatColor) * 0.5 + 0.50;
    float greenColor = sin(2*(M_PI*flatColor - M_PI/4)) * 0.5 + 0.50;
    float redColor = sin(2*(M_PI*flatColor - M_PI/2)) * 0.5 + 0.50;

    if(blueColor < 0.5)
        blueColor=0;
    if(redColor < 0.5)
        redColor=0;
    if(greenColor < 0.5)
        greenColor=0;

    if(colorful)
        fs_color.xyzw = vec4(redColor, greenColor, blueColor, 1.0);
    else
        fs_color.xyzw = vec4(position.z, position.z, position.z, 1.0);
    gl_Position = transformation*position;
}
