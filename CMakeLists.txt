cmake_minimum_required(VERSION 3.15)
project(SolarSim)

set(CMAKE_CXX_STANDARD 17)

add_executable(SolarSim glad/src/glad.c src/main.cpp src/Window.cpp src/Window.h src/Particle.h src/ParticleSimulator.cpp src/ParticleSimulator.h src/Renderer.cpp src/Renderer.h src/Shader.cpp src/Shader.h src/Camera.cpp src/Camera.h)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OpenCL REQUIRED)
find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    target_link_libraries(SolarSim ${GLEW_LIBRARIES})
endif()

target_include_directories (SolarSim PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} /usr/local/include glad)
target_link_libraries(SolarSim glfw ${OPENGL_gl_LIBRARY} ${OpenCL_LIBRARY})