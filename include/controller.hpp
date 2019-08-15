//
// Created by liao xiangsen on 2019/8/13.
//

#ifndef CONETRACER_CONTROLLER_HPP
#define CONETRACER_CONTROLLER_HPP

#include <map>

#include "common.hpp"

#include "camera.hpp"

CONETRACER_NAMESPACE_BEGIN

const float CONTROLLER_CAMERA_MOVE_SPEED = 10.f;

class Controller {
public:
  Controller(GLFWwindow *window, int winWidth, int winHeight);
  ~Controller();
  static void keyCallbackWrapper(GLFWwindow* window, int key, int scancode, int action, int mods);
  // be called each main loop
  void loopUpdate(float loopCostTime);

  GLFWwindow *windowPtr;
  shared_ptr<Camera> cameraPtr;
private:
  void keyCallback(int key, int scancode, int action, int mods);
  float moveSpeed;
  glm::vec3 moveVec;
  std::map<int, glm::vec3> keyToMoveMap;
};

CONETRACER_NAMESPACE_END

#endif //CONETRACER_CONTROLLER_HPP
