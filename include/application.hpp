//
// Created by liao xiangsen on 2019/8/11.
//

#ifndef CONETRACER_APPLICATION_HPP
#define CONETRACER_APPLICATION_HPP

#include "common.hpp"
#include "scene/scene.hpp"
#include "controller.hpp"

CONETRACER_NAMESPACE_BEGIN

class TracerApp {
public:
  TracerApp(int width, int height, GLFWwindow *window);

  bool initContext();
  void drawVoxels();
  void draw(float loopCostTime);

protected:
  TRACERAPP_DRAW_MODE mode;
  GLFWwindow *windowPtr;
  int winWidth, winHeight;

  shared_ptr<Controller> ctrlPtr;
  shared_ptr<Scene> scenePtr;

  // compiled and linked program ID
  GLuint voxelShaderID;
  // VAO ID
  GLuint voxelVAOID, voxelVBOID;
  // texture
  Texture3DCubic voxelTex;
};

CONETRACER_NAMESPACE_END

#endif //CONETRACER_APPLICATION_HPP
