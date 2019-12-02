#include "Particle.h"


Particle::Particle() {
    data = {0, 0, 0, 0, 0, 0, 0, 0};
}

Particle::Particle(cl_float3 position, cl_float3 momentum, cl_float mass, cl_float radius) {
    data = (cl_float8){position.x, position.y, position.z, momentum.x, momentum.y, momentum.z, mass, radius};
}

Particle::Particle(cl_float px, cl_float py, cl_float pz, cl_float mx, cl_float my, cl_float mz, cl_float mass, cl_float radius) {
    data = (cl_float8){px, py, pz, mx, my, mz, mass, radius};
}

cl_float3 Particle::position() {
    return (cl_float3) {data.s[0], data.s[1], data.s[2]};
}

cl_float3 Particle::momentum() {
    return (cl_float3) {data.s[3], data.s[4], data.s[5]};
}

cl_float Particle::mass() {
    return data.s[6];
}

cl_float Particle::radius() {
    return data.s[7];
}
