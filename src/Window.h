#define GL_SILENCE_DEPRECATION

#include "glad.h"

#include <GLFW/glfw3.h>
#include <string>

#ifndef SOLARSIM_WINDOW_H
#define SOLARSIM_WINDOW_H


class Window {
private:
    GLFWwindow* handle;

public:
    explicit Window(int width, int height, std::string title);
    bool isOpen() const;
    void processInput();
    void setVisible(bool visible);
    void setPosition(int x, int y);
    void setSize(int width, int height);
    void swapBuffers();
    void setClearColor(float red, float green, float blue);
    void clear();
    void close();

    void setCursorPosCallback(GLFWcursorposfun callback);
    void setMouseButtonCallback(GLFWmousebuttonfun callback);
    void setScrollCallback(GLFWscrollfun callback);
    void setFrameSizeCallback(GLFWframebuffersizefun callback);
    void setKeyCallback(GLFWkeyfun callback);

    int getKeyState(int key) const;
};


#endif //SOLARSIM_WINDOW_H
