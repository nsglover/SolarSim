#ifndef SOLARSIM_PARTICLE_H
#define SOLARSIM_PARTICLE_H

#include <OpenCL/opencl.hpp>

typedef struct __attribute__ ((packed)) particle_struct
{
    cl_float3 momentum;
    cl_float3 position;
    cl_float mass;
    cl_float radius;
} Particle;

#endif //SOLARSIM_PARTICLE_H
