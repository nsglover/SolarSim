#ifndef SOLARSIM_RENDERER_H
#define SOLARSIM_RENDERER_H


#include <OpenCL/opencl.h>
#include "Shader.h"
#include "Camera.h"

class Renderer {
private:
    unsigned int numParticles;
    unsigned int vertexArray{}, vertexBuffer{};
    glm::mat4 projectionMatrix{};

public:
    explicit Renderer(int frameWidth, int frameHeight, unsigned int numParticles);

    void renderParticles(const Shader& shader, const Camera& camera, const cl_float* positions) const;

    void setProjection(int frameWidth, int frameHeight);
};


#endif //SOLARSIM_RENDERER_H
