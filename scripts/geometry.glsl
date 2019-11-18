#version 410 core
layout (points) in;
layout (points, max_vertices = 256) out;

const float PI =  3.141592f;
const float MAX_VERT = 16;

uniform mat4 transform;

void main() {
    for(int i = 0; i < MAX_VERT; i++) {
        for(int j = 0; j < MAX_VERT; j++) {
            float t0 = i * (2 * PI) / MAX_VERT;
            float t1 = j * (2 * PI) / MAX_VERT;
            gl_Position = gl_in[0].gl_Position + transform * 0.01f * vec4(cos(t0) * sin(t1), sin(t0) * sin(t1), cos(t1), 0.0);
            EmitVertex();
        }
    }

    EndPrimitive();
}