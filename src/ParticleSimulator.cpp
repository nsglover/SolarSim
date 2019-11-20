#include <fstream>
#include <chrono>
#include <cmath>

#include "ParticleSimulator.h"
#include <OpenCL/cl.h>

typedef std::chrono::high_resolution_clock Clock;

ParticleSimulator::ParticleSimulator(const string& kernelPath) {
    try {
        std::vector<cl::Platform> platforms;
        cl::Platform::get(&platforms);
        if(platforms.empty()) {
            cerr << "ERROR: No OpenCL platforms available" << endl;
        }

        platform = platforms[0];

        cl_context_properties properties[] = {CL_CONTEXT_PLATFORM, (cl_context_properties) (platforms[0])(), 0};
        context = cl::Context(CL_DEVICE_TYPE_ALL, properties);
        std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();

        device = devices[2];
        commandQueue = cl::CommandQueue(context, device, 0, nullptr);

        std::ifstream fileStream(kernelPath);
        std::string kernelCode((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());

        //Build program from source string
        cl::Program::Sources source(1, std::make_pair(kernelCode.data(), kernelCode.length()));
        program = cl::Program(context, source);
        program.build({device});

        // Create kernel objects
        generateKernel = cl::Kernel(program, "generateParticles", nullptr);
        forceKernel = cl::Kernel(program, "calculateForces", nullptr);
        updateKernel = cl::Kernel(program, "updateStates", nullptr);
    } catch(const cl::Error& err) {
        std::cerr << "ERROR: " << err.what() << "(" << err.err() << ")" << std::endl;

        cl::STRING_CLASS buildLog;
        program.getBuildInfo(device, CL_PROGRAM_BUILD_LOG, &buildLog);
        std::cerr << buildLog << std::endl;
        exit(err.err());
    }
}

vector<Particle> ParticleSimulator::generateParticles(unsigned int count) {
    vector<Particle> particles(count);
    size_t dataSize = count * sizeof(Particle);

    cl::Buffer output(context, CL_MEM_WRITE_ONLY, dataSize);

    // Bind kernel arguments to kernel
    generateKernel.setArg(0, output);

    runKernel1D(generateKernel, count, 64);

    commandQueue.enqueueReadBuffer(output, CL_TRUE, 0, dataSize, particles.data());

    return particles;
}

const cl_float* ParticleSimulator::calculateParticleForces(const vector<Particle>& particles) {
    int count = particles.size();
    auto* forces = new float[count * 3];
    size_t inputSize = count * sizeof(Particle);
    size_t outputSize = count * 3 * sizeof(float);

    cl::Buffer input(context, CL_MEM_READ_ONLY, inputSize);
    cl::Buffer output(context, CL_MEM_WRITE_ONLY, outputSize);

    // Bind memory buffers
    commandQueue.enqueueWriteBuffer(input, CL_TRUE, 0, inputSize, particles.data());

    // Bind kernel arguments to kernel
    forceKernel.setArg(0, input);
    forceKernel.setArg(1, output);

    runKernel2D(forceKernel, count, 128);

    commandQueue.enqueueReadBuffer(output, CL_TRUE, 0, outputSize, forces);

    return forces;
}

const float* ParticleSimulator::updateParticlePositions(vector<Particle>& particles, const cl_float* forces, float deltaTime) {
    int count = particles.size();
    auto* positions = new float[count * 3];
    size_t particleSize = count * sizeof(Particle);
    size_t forceInputSize = count * 3 * sizeof(float);
    size_t outputSize = count * 3 * sizeof(float);

    cl::Buffer particleInputOutput(context, CL_MEM_READ_WRITE, particleSize);
    cl::Buffer forceInput(context, CL_MEM_READ_ONLY, forceInputSize);
    cl::Buffer output(context, CL_MEM_WRITE_ONLY, outputSize);
    cl::Buffer particleOutput(context, CL_MEM_WRITE_ONLY, particleSize);

    // Bind memory buffers
    commandQueue.enqueueWriteBuffer(particleInputOutput, CL_TRUE, 0, particleSize, particles.data());
    commandQueue.enqueueWriteBuffer(forceInput, CL_TRUE, 0, forceInputSize, forces);

    // Bind kernel arguments to kernel
    updateKernel.setArg(0, particleInputOutput);
    updateKernel.setArg(1, forceInput);
    updateKernel.setArg(2, deltaTime);
    updateKernel.setArg(3, output);

    runKernel1D(updateKernel, count, 64);

    commandQueue.enqueueReadBuffer(output, CL_TRUE, 0, outputSize, positions);
    commandQueue.enqueueReadBuffer(particleInputOutput, CL_TRUE, 0, particleSize, particles.data());

    return positions;
}

void ParticleSimulator::runKernel1D(const cl::Kernel& kernel, unsigned int globalSize, unsigned int localSize) const {
    cl::NDRange localSizeRange(localSize);
    size_t realGlobalSize = (size_t) (ceil((float) globalSize / (float) localSize)) * localSize;
    cl::NDRange globalSizeRange(realGlobalSize);

//    auto start = Clock::now();
//    printf("Beginning 1D operation...\n");

    cl::Event event;
    commandQueue.enqueueNDRangeKernel(kernel, cl::NullRange, globalSizeRange, localSizeRange, nullptr, &event);

    event.wait();

//    std::chrono::duration<float> fs = Clock::now() - start;
//    printf("Done. Operation took %f seconds\n", fs.count());
}

void ParticleSimulator::runKernel2D(const cl::Kernel& kernel, unsigned int globalSize, unsigned int localSize) const {
    cl::NDRange localSizeRange(localSize, 1);
    size_t realGlobalSize = (size_t) (ceil((float) globalSize / (float) localSize)) * localSize;
    cl::NDRange globalSizeRange(realGlobalSize, realGlobalSize);

//    auto start = Clock::now();
//    printf("Beginning 2D operation...\n");

    cl::Event event;
    commandQueue.enqueueNDRangeKernel(kernel, cl::NullRange, globalSizeRange, localSizeRange, nullptr, &event);

    event.wait();

//    std::chrono::duration<float> fs = Clock::now() - start;
//    printf("Done. Operation took %f seconds\n", fs.count());
}

