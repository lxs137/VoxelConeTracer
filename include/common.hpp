//
// Created by liao xiangsen on 2019/8/11.
//

#ifndef CONETRACER_COMMON_HPP
#define CONETRACER_COMMON_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#define CONETRACER_NAMESPACE_BEGIN namespace conetracer {
#define CONETRACER_NAMESPACE_END }

CONETRACER_NAMESPACE_BEGIN

using std::cout;
using std::cerr;
using std::endl;

using std::shared_ptr;
using std::string;

const int WIN_WIDTH = 1280, WIN_HEIGHT = 720;
const int VOXELS_PER_SIDE = 128;
const float VOXELS_SIZE = 0.02f;
// VOXELS_SIZE * VOXELS_PER_SIDE > 2

enum TEXTURE_TYPE {
  DIFFUSE_TEXTURE,
  SPECULAR_TEXTURE,
  MASK_TEXTURE,
  HEIGHT_TEXTURE,
  NUM_TEXTURES
};

enum TRACERAPP_DRAW_MODE {
  RAW_VOXELS_MODE,
};

struct Texture2D {
  GLuint textureID;
  int width, height, componentsPerPixel;
};

struct Texture3DCubic {
  GLuint textureID;
  int size, componentsPerPixel;
};

CONETRACER_NAMESPACE_END

#endif //CONETRACER_COMMON_HPP
