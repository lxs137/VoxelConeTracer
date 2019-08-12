//
// Created by liao xiangsen on 2019/8/11.
//

#ifndef CONETRACER_APPLICATION_HPP
#define CONETRACER_APPLICATION_HPP

#include "common.hpp"
#include "scene.hpp"

CONETRACER_NAMESPACE_BEGIN

class TracerApp {
public:
  TracerApp(const int width, const int height, GLFWwindow *windowPtr);

  bool initContext();
  void draw();


protected:
  GLFWwindow *window;
  shared_ptr<Scene> scenePtr;
};

CONETRACER_NAMESPACE_END

#endif //CONETRACER_APPLICATION_HPP
