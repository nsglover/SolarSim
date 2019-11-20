#version 410 core

out vec4 fragColor;

in float depth;

float sigm(float x) {
    return 1 / (1 + exp(-x));
}

void main() {
    fragColor = vec4(1, 0, 0, 1);
}