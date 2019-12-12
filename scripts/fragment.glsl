#version 410 core

out vec4 fragColor;

float sigm(float x) {
    return 1 / (1 + exp(-x));
}

uniform mat4 view;
uniform mat4 projection;

in vec2 coords;
in vec3 centerPos;
in float radius;
in float mass;

out vec4 pixel;

void main()
{
    float d = dot(coords, coords);
    if (d > 1.0f)
    {
        discard;
    }

    float z = sqrt(1.0f - d);

    vec3 normal = vec3(coords, z);
    normal = mat3(transpose(view)) * normal;

    vec3 center = centerPos;

    vec3 position = center + radius * normal;

    mat4 MVP = projection * view;

    // gl_DepthRange.diff value is the far value minus the near value

    vec4 clipPos = MVP * vec4(position, 1.0f);
    float ndcDepth = clipPos.z / clipPos.w;
    gl_FragDepth = ((gl_DepthRange.diff * ndcDepth) + gl_DepthRange.near + gl_DepthRange.far) * 0.5f;

    float factor = exp(-(mass - 1e10) / 5e11);

    pixel = vec4(1 - factor, (0.5f + factor) * ((normal + 1.0f) * 0.5f).yz, 1.0f);

    if(mass >= 1e15) {
        pixel = vec4(((normal)).xyz, 1.0f);
    }
}
