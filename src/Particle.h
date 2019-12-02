#ifndef SOLARSIM_PARTICLE_H
#define SOLARSIM_PARTICLE_H


#include <OpenCL/opencl.h>

typedef struct __attribute__ ((packed)) Particle {
private:
    cl_float8 data{};

public:
    Particle();
    explicit Particle(cl_float3 position, cl_float3 momentum, cl_float mass, cl_float radius);
    explicit Particle(cl_float px, cl_float py, cl_float pz, cl_float mx, cl_float my, cl_float mz, cl_float mass, cl_float radius);
    cl_float3 position();
    cl_float3 momentum();
    cl_float mass();
    cl_float radius();
} Particle;


#endif //SOLARSIM_PARTICLE_H
