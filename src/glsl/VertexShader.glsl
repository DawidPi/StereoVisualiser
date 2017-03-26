#version 440

in vec4 position;
out vec4 fs_color;

void main() {
    float grayscale = position.z;
    fs_color.xyzw = vec4(grayscale, grayscale, grayscale, 1.0);
    vec4 zoomedPosition = position;
    zoomedPosition.xyz *= 1;
    gl_Position = zoomedPosition;
}
