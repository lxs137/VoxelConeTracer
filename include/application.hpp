//
// Created by liao xiangsen on 2019/8/11.
//

#ifndef CONETRACER_APPLICATION_HPP
#define CONETRACER_APPLICATION_HPP

#include <GLFW/glfw3.h>

#include "common.hpp"

class TracerApp {
public:
  TracerApp(const int width, const int height, GLFWwindow *window);

  bool initContext();
  void draw();
private:
  GLFWwindow *window;
};


#endif //CONETRACER_APPLICATION_HPP
