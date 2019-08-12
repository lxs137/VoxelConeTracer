//
// Created by liao xiangsen on 2019/8/11.
//

#include "application.hpp"

#include "utils.hpp"

CONETRACER_NAMESPACE_BEGIN

TracerApp::TracerApp(const int width, const int height, GLFWwindow *windowPtr) {
    window = windowPtr;
}

bool TracerApp::initContext() {
    cout << "Loading scene..." << endl;
    scenePtr = loadScene("../data/models/teapot/teapot.obj", "../data/models/teapot", glm::vec3(0.0f), 0.05f);
    scenePtr->voxelize(VOXELS_PER_SIDE, VOXELS_SIZE);

    return true;
}

void TracerApp::draw() {

}

CONETRACER_NAMESPACE_END