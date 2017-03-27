#version 440

in vec4 position;
out vec4 fs_color;

layout (location=0) uniform mat4 transformation;

void main() {
    float blueRedRatio = position.z;
    fs_color.xyzw = vec4(1.0f * blueRedRatio, 0.0f, 1.0f - blueRedRatio*1.0f, 1.0);
    gl_Position = transformation*position;
}
