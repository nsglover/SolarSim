#version 410 core
layout (points) in;
layout (points, max_vertices = 1) out;

const float PI =  3.141592f;
const float MAX_VERT = 1;

uniform mat4 transform;
out float depth;

void main() {
    for(int i = 0; i < MAX_VERT; i++) {
        for(int j = 0; j < MAX_VERT; j++) {
            float t0 = i * (2 * PI) / MAX_VERT;
            float t1 = j * (2 * PI) / MAX_VERT;
            gl_Position = gl_in[0].gl_Position + transform * 0.01f * vec4(cos(t0) * sin(t1), sin(t0) * sin(t1), cos(t1), 0.0);
            gl_PointSize = 50 / gl_Position.z;
            depth = gl_Position.z;
            EmitVertex();
        }
    }

    EndPrimitive();
}