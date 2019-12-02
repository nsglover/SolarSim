#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"

#define __CL_ENABLE_EXCEPTIONS

#ifndef SOLARSIM_PARTICLESIMULATOR_H
#define SOLARSIM_PARTICLESIMULATOR_H

#include <OpenCL/opencl.hpp>
#include <iostream>
#include <err.h>
#include "Particle.h"

using namespace std;

class ParticleSimulator {
private:
    cl::Context context;
    cl::Platform platform;
    cl::Device device;
    cl::CommandQueue commandQueue;

    cl::Program program;
    cl::Kernel generateKernel;
    cl::Kernel forceKernel;
    cl::Kernel updateKernel;

    void runKernel1D(const cl::Kernel& kernel, unsigned int globalSize, unsigned int localSize) const;
    void runKernel2D(const cl::Kernel& kernel, unsigned int globalSize, unsigned int localSize) const;

    //Returns forces (every three entries represents one vector)
    const cl_float* calculateParticleForces(vector<Particle>& particles);

public:
    explicit ParticleSimulator(const string& kernelPath);

    vector<Particle> generateParticles(unsigned int count);
    void updateParticlePositions(vector<Particle>& particles, float deltaTime);
};


#endif //SOLARSIM_PARTICLESIMULATOR_H

#pragma clang diagnostic pop