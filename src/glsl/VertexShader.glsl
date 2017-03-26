#version 440

in vec4 position;
out vec4 fs_color;

uniform float near = 1;
uniform float far = 2;
uniform float right = 1;
uniform float left = -1;
uniform float top = 1;
uniform float bottom = -1;
uniform float rotationX=0;
uniform float rotationY=0;
uniform float rotationZ=0;

mat4 frustum = mat4(2*near/(right-left), 0,0,0,
                    0, 2*near/(top-bottom), 0, 0,
                    (right+left)/(right-left), (top+bottom)/(top-bottom), (near+far)/(near-far), -1,
                     0, 0, 2*near*far/(near-far), 0);

float zTranslation=-2;
mat4 translate = mat4(1,0,0,0,
                      0,1,0,0,
                      0,0,1,0,
                      0,0,zTranslation,1);


void main() {
    vec4 zoomedPosition = position;
    zoomedPosition.z *= 1;
    vec4 finalPosition = frustum*translate*zoomedPosition;
    float grayscale = position.z;
    fs_color.xyzw = vec4(grayscale, grayscale, grayscale, 1.0);
    gl_Position = frustum*translate*zoomedPosition;
}
