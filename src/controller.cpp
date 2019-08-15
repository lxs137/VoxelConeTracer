//
// Created by liao xiangsen on 2019/8/13.
//

#include "controller.hpp"

#include "utils.hpp"

CONETRACER_NAMESPACE_BEGIN

Controller::Controller(GLFWwindow *window, int winWidth, int winHeight): windowPtr(window) {
    moveSpeed = CONTROLLER_CAMERA_MOVE_SPEED;
    moveVec = glm::vec3(0.0, 0.0, 0.0);

    glm::vec3 pos = glm::vec3(0.0, 0.0, 5.0);
    glm::vec3 worldUp = glm::vec3(0.0, 1.0, 0.0);
    float yaw = -3.1415f / 2.0f, pitch = 45.0f, roll = 0.0f, fov = 45.f;
    cameraPtr = std::make_shared<Camera>(pos, yaw, pitch, roll, worldUp, fov, (float)winWidth / winHeight);
    keyToMoveMap.insert({GLFW_KEY_W, cameraPtr->front});
    keyToMoveMap.insert({GLFW_KEY_S, cameraPtr->front * -1.f});
    keyToMoveMap.insert({GLFW_KEY_D, cameraPtr->right});
    keyToMoveMap.insert({GLFW_KEY_A, cameraPtr->right * -1.f});
    keyToMoveMap.insert({GLFW_KEY_E, cameraPtr->up});
    keyToMoveMap.insert({GLFW_KEY_Q, cameraPtr->up * -1.f});

    glfwSetWindowUserPointer(windowPtr, this);
    glfwSetKeyCallback(windowPtr, Controller::keyCallbackWrapper);
}

Controller::~Controller() {
    glfwSetWindowUserPointer(windowPtr, nullptr);
}

void Controller::keyCallbackWrapper(GLFWwindow *window, int key, int scancode, int action, int mods) {
    Controller* that = static_cast<Controller*>(glfwGetWindowUserPointer(window));
    that->keyCallback(key, scancode, action, mods);
}

void Controller::keyCallback(int key, int scancode, int action, int mods) {
    auto lookupRes = keyToMoveMap.find(key);
    if (lookupRes != keyToMoveMap.end()) {
        if (action == GLFW_PRESS) {
            moveVec += lookupRes->second;
        } else if (action == GLFW_RELEASE) {
            moveVec -= lookupRes->second;
        }
    }
}

void Controller::loopUpdate(float loopCostTime) {
    cout << "move vector: " << moveVec << endl;
    cameraPtr->move(moveVec * (moveSpeed * loopCostTime));
}

CONETRACER_NAMESPACE_END