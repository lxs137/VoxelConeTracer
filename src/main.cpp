//
// Created by liao xiangsen on 2019/8/11.
//
#include <iostream>

#include "common.hpp"
#include "application.hpp"

using namespace conetracer;

void dumpGLInfo() {
    cout << "Vendor: " << glGetString(GL_VENDOR)
         << "\nRenderer: " << glGetString(GL_RENDERER)
         << "\nVersion: " << glGetString(GL_VERSION)
         << "\nGLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}

void dumpGLErrors() {
    GLenum glError = glGetError();
    if(glError) {
        switch (glError) {
            case GL_INVALID_ENUM:
                std::cout << "Invalid enum.\n";
                break;

            case GL_INVALID_VALUE:
                std::cout << "Invalid value.\n";
                break;

            case GL_INVALID_OPERATION:
                std::cout << "Invalid operation.\n";

            default:
                std::cout << "Unrecognised GLenum.\n";
                break;
        }
    }
}

int main() {
    if(!glfwInit()) {
        cerr << "Failed to initialize GLFW\n";
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "ConeTracer", nullptr, nullptr);
    if (window == nullptr) {
        cerr << "Failed to Create OpenGL Context\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(window, WIN_WIDTH, WIN_HEIGHT);

    glewExperimental = GL_TRUE; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        cerr << "Failed to initialize GLEW\n";
        return EXIT_FAILURE;
    }

    dumpGLInfo();
    dumpGLErrors();

    TracerApp app(WIN_WIDTH, WIN_HEIGHT, window);
    if (!app.initContext()) {
        cerr << "Failed to initialize TestApplication\n";
        return EXIT_FAILURE;
    }

    // main loop
    double curTime = 0.0, prevTime = 0.0, showFPSInternal = 0.0;
    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        curTime = glfwGetTime();
        const double deltaTime = curTime - prevTime;
        prevTime = curTime;

        // show FPS every 1 second
        if (showFPSInternal > 1.0) {
            cout << "FPS: " << 1.0 / deltaTime << endl;
            showFPSInternal = 0.0;
        } else {
            showFPSInternal += deltaTime;
        }

        app.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}