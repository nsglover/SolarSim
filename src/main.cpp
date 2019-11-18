#include "Window.h"
#include "ParticleSimulator.h"
#include "Shader.h"
#include "Renderer.h"
#include "Camera.h"


using namespace std;
typedef std::chrono::high_resolution_clock Clock;

float speedMultiplier = 0;

void processInput(Window& window, Camera& camera, float deltaTime) {
    if(window.getKeyState(GLFW_KEY_ESCAPE) == GLFW_PRESS)
        window.close();

    if(window.getKeyState(GLFW_KEY_W) == GLFW_PRESS)
        camera.move(CameraMovement::FORWARD, deltaTime);
    if(window.getKeyState(GLFW_KEY_A) == GLFW_PRESS)
        camera.move(CameraMovement::LEFT, deltaTime);
    if(window.getKeyState(GLFW_KEY_S) == GLFW_PRESS)
        camera.move(CameraMovement::BACKWARD, deltaTime);
    if(window.getKeyState(GLFW_KEY_D) == GLFW_PRESS)
        camera.move(CameraMovement::RIGHT, deltaTime);

    if(window.getKeyState(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.movementSpeed = 10.f;
    else
        camera.movementSpeed = 1.5f;
}

Camera* cameraPtr;
bool firstMouse = true;
float lastX, lastY;

void onMouseMove(GLFWwindow*, double x, double y) {
    if(firstMouse) {
        lastX = (float) x;
        lastY = (float) y;
        firstMouse = false;
    }

    float xOffset = (float) x - lastX;
    float yOffset = lastY - (float) y; // reversed since y-coordinates go from bottom to top
    lastX = (float) x;
    lastY = (float) y;

    cameraPtr->processMouseMovement(xOffset, yOffset);
}

void onKeyPress(GLFWwindow*, int key, int scancode, int action, int mods) {
    if(key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        speedMultiplier = (speedMultiplier == 1) ? 0 : 1;
    }
}

const unsigned int NUM_PARTICLES = 2048;

int main() {
    const string filePrefix = "/Users/nathanielglover/Projects/SolarSim/scripts/";

    Window window(1440, 800, "SolarSim");
    window.setClearColor(0.0f, 0.0f, 0.2f);

    Camera camera;
    cameraPtr = &camera;
    window.setCursorPosCallback(onMouseMove);
    window.setKeyCallback(onKeyPress);

    Shader shader(filePrefix + "vertex.glsl", filePrefix + "geometry.glsl", filePrefix + "fragment.glsl");
    Renderer renderer(1440 * 2, 800 * 2, NUM_PARTICLES);

    ParticleSimulator simulator(filePrefix + "kernel.cl");
    auto* particles = simulator.generateParticles(NUM_PARTICLES);

    glPointSize(5);

    float deltaTime = 0;

    window.setVisible(true);
    while(window.isOpen()) {
        auto start = Clock::now();
        window.clear();

        auto* forces = simulator.calculateParticleForces(particles, NUM_PARTICLES);
        auto* positions = simulator.updateParticlePositions(particles, forces, NUM_PARTICLES, deltaTime / 10 * speedMultiplier);

//        for(int i = 0; i < NUM_PARTICLES; i++) {
//            int ix = 3 * i;
//            int iy = 3 * i + 1;
//            int iz = 3 * i + 2;
//
//            printf("Pos: (%f, %f, %f)\n", positions[ix], positions[iy], positions[iz]);
//            printf("Force: (%f, %f, %f)\n\n", forces[ix], forces[iy], forces[iz]);
//        }
//        printf("\n\n\n");

        renderer.renderParticles(shader, camera, positions);

        window.swapBuffers();
        window.processInput();
        processInput(window, camera, deltaTime);

        std::chrono::duration<float> fs = Clock::now() - start;
        deltaTime = fs.count();
    }

    return 0;
}
