#include <iostream>
#include "Window.h"

Window::Window(int width, int height, std::string title) {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    handle = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);

    if(handle == nullptr) {
        std::cerr << "Failed to initialize OpenGL and GLFW." << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(handle);
    glfwSwapInterval(1);

    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }

    glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursor(handle, nullptr);

    //Configure OpenGL state
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
}

bool Window::isOpen() const {
    return !glfwWindowShouldClose(handle);
}

void Window::processInput() {
    glfwPollEvents();
}

void Window::setVisible(bool visible) {
    if(visible) {
        glfwShowWindow(handle);
    } else {
        glfwHideWindow(handle);
    }
}

void Window::setPosition(int x, int y) {
    glfwSetWindowPos(handle, x, y);
}

void Window::setSize(int width, int height) {
    glfwSetWindowSize(handle, width, height);
}

void Window::swapBuffers() {
    glfwSwapBuffers(handle);
}

void Window::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::setClearColor(float red, float blue, float green) {
    glClearColor(red, blue, green, 1.0f);
}

void Window::close() {
    glfwSetWindowShouldClose(handle, true);
}

void Window::setCursorPosCallback(GLFWcursorposfun callback) {
    glfwSetCursorPosCallback(handle, callback);
}

void Window::setMouseButtonCallback(GLFWmousebuttonfun callback) {
    glfwSetMouseButtonCallback(handle, callback);
}

void Window::setScrollCallback(GLFWscrollfun callback) {
    glfwSetScrollCallback(handle, callback);
}

void Window::setFrameSizeCallback(GLFWframebuffersizefun callback) {
    glfwSetFramebufferSizeCallback(handle, callback);
}

int Window::getKeyState(int key) const {
    return glfwGetKey(handle, key);
}

void Window::setKeyCallback(GLFWkeyfun callback) {
    glfwSetKeyCallback(handle, callback);
}
