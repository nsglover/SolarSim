#version 410 core

layout (location = 0) in vec3 position;
//layout (location = 1) in vec3 momentum;
layout (location = 2) in float mass;
layout (location = 3) in float radius;

uniform mat4 view;
uniform mat4 projection;

out float Radius;
out float Mass;
out vec3 pos;

void main() {
    pos = position;
    Radius = radius;
    Mass = mass;
}
