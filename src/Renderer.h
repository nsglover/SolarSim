#ifndef SOLARSIM_RENDERER_H
#define SOLARSIM_RENDERER_H


#include <OpenCL/opencl.h>
#include "Shader.h"
#include "Camera.h"
#include "Particle.h"

class Renderer {
private:
    unsigned int vertexArray{}, vertexBuffer{};
    glm::mat4 projectionMatrix{};

public:
    explicit Renderer(int frameWidth, int frameHeight);

    void renderParticles(const Shader& shader, const Camera& camera, const vector<Particle>& particles) const;

    void setProjection(int frameWidth, int frameHeight);
};


#endif //SOLARSIM_RENDERER_H
