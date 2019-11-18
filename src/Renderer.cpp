#define GL_SILENCE_DEPRECATION

#include "Renderer.h"
#include "glad.h"

Renderer::Renderer(int frameWidth, int frameHeight, unsigned int numParticles) {
    vertexArray = 0;
    vertexBuffer = 0;
    projectionMatrix = glm::perspective(glm::radians(45.0f), (float) frameWidth / (float) frameHeight, 0.01f, 1000.0f);

    glGenVertexArrays(1, &vertexArray);
    glGenBuffers(1, &vertexBuffer);
    glBindVertexArray(vertexArray);

    this->numParticles = numParticles;

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 3 * numParticles * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::renderParticles(const Shader& shader, const Camera& camera, const cl_float* positions) const {
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * numParticles * sizeof(float), positions);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    shader.use();

    shader.setMat4("transform", projectionMatrix * camera.getViewMatrix());
    glBindVertexArray(vertexArray);
    glDrawArrays(GL_POINTS, 0, numParticles);
    glBindVertexArray(0);
}

void Renderer::setProjection(int frameWidth, int frameHeight) {
    projectionMatrix = glm::perspective(glm::radians(45.0f), (float) frameWidth / (float) frameHeight, 0.01f, 1000.0f);
}
