#version 410 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4 view;
uniform mat4 projection;

in vec3[] pos;
in float[] Radius;
in float[] Mass;

out vec2 coords;
out vec3 centerPos;
out float radius;
out float mass;

void main()
{
    for(int i = 0; i < 4; i++) {
        switch(i)
        {
            case 0:
            coords = vec2(-1.0f, -1.0f);
            break;
            case 1:
            coords = vec2(-1.0f, +1.0f);
            break;
            case 2:
            coords = vec2(+1.0f, -1.0f);
            break;
            case 3:
            coords = vec2(+1.0f, +1.0f);
            break;
        }

        // right & up axes camera space
        vec3 right = vec3(view[0][0], view[1][0], view[2][0]);
        vec3 up = vec3(view[0][1], view[1][1], view[2][1]);

        vec3 center = pos[0];
        centerPos = center;
        radius = Radius[0];
        mass = Mass[0];

        // vertex position
        vec3 position = Radius[0] * vec3(coords, 0.0f);
        position = right * position.x + up * position.y;

        mat4 MVP = projection * view;
        gl_Position = MVP * vec4(center + position, 1.0f);

        EmitVertex();
    }

    EndPrimitive();
}
