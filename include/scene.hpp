//
// Created by liao xiangsen on 2019/8/12.
//

#ifndef CONETRACER_SCENE_HPP
#define CONETRACER_SCENE_HPP

#include <assimp/scene.h>

#include "common.hpp"
#include "mesh.hpp"
#include "object.hpp"

CONETRACER_NAMESPACE_BEGIN

class Scene {
public:
  Scene() = default;
  void voxelize(int voxelsPerSide, float voxelSize);
  Texture3DCubic voxelTex;
  std::vector<std::shared_ptr<RenderObj>> objs;
};

CONETRACER_NAMESPACE_END

#endif //CONETRACER_SCENE_HPP
