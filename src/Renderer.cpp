#define GL_SILENCE_DEPRECATION

#include "Renderer.h"
#include "glad.h"
#include <iostream>

Renderer::Renderer(int frameWidth, int frameHeight) {
    vertexArray = 0;
    vertexBuffer = 0;
    projectionMatrix = glm::perspective(glm::radians(45.0f), (float) frameWidth / (float) frameHeight, 0.0001f, 10000.0f);

    glGenVertexArrays(1, &vertexArray);
    glGenBuffers(1, &vertexBuffer);

    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    //Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);

//    //Momentum
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//    glEnableVertexAttribArray(1);

    //Mass
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //Radius
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::renderParticles(const Shader& shader, const Camera& camera, const vector<Particle>& particles) const {
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, particles.size() * sizeof(Particle), particles.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    shader.use();

    shader.setMat4("projection", projectionMatrix);
    shader.setMat4("view", camera.getViewMatrix());
    glBindVertexArray(vertexArray);
    glDrawArrays(GL_POINTS, 0, particles.size());
    glBindVertexArray(0);
}

void Renderer::setProjection(int frameWidth, int frameHeight) {
    projectionMatrix = glm::perspective(glm::radians(45.0f), (float) frameWidth / (float) frameHeight, 0.0001f, 10000.0f);
}
