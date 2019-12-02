#include "Window.h"
#include "ParticleSimulator.h"
#include "Shader.h"
#include "Renderer.h"
#include "Camera.h"
#include <OpenGL/glu.h>


using namespace std;
typedef std::chrono::high_resolution_clock Clock;

float speedMultiplier = 0;

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
        camera.movementSpeed = 500.f;
    else
        camera.movementSpeed = 150.0f;
}

int main() {
    const string filePrefix = "/Users/nathanielglover/Documents/School/33-151/SolarSim/scripts/";

    Window window(1440, 800, "SolarSim");
    window.setClearColor(0.0f, 0.0f, 0.1f);

    Camera camera;
    cameraPtr = &camera;
    window.setCursorPosCallback(onMouseMove);
    window.setKeyCallback(onKeyPress);

    Shader shader(filePrefix + "vertex.glsl", filePrefix + "geometry.glsl", filePrefix + "fragment.glsl");
//    Shader shader(filePrefix + "vertex.glsl", filePrefix + "fragment.glsl");
    Renderer renderer(1440 * 2, 800 * 2);

    ParticleSimulator simulator(filePrefix + "kernel.cl");
    auto particles = simulator.generateParticles(2048);

    Particle sun1(-100, -100, -100, 5e14, 5e14, 5e14, 1e14, 5);
    Particle sun2(150, 50, 100, -10e14, -10e14, -10e14, 1e14, 5);
    particles.push_back(sun1);
    particles.push_back(sun2);

    float deltaTime = 0;

    window.setVisible(true);
    while(window.isOpen()) {
        auto start = Clock::now();
        window.clear();

        simulator.updateParticlePositions(particles, deltaTime * speedMultiplier);

        renderer.renderParticles(shader, camera, particles);

        window.swapBuffers();
        window.processInput();
        processInput(window, camera, deltaTime);

//        cout << gluErrorString(glGetError()) << endl;

        std::chrono::duration<float> fs = Clock::now() - start;
        deltaTime = fs.count();
    }

    return 0;
}
