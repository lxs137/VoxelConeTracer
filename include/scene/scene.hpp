//
// Created by liao xiangsen on 2019/8/12.
//

#ifndef CONETRACER_SCENE_HPP
#define CONETRACER_SCENE_HPP

#include <vector>

#include <assimp/scene.h>

#include "scene/grids.hpp"
#include "common.hpp"
#include "mesh.hpp"
#include "object.hpp"

CONETRACER_NAMESPACE_BEGIN

class Scene {
public:
  Scene() = default;
  // @return {r,g,b,a}[n * n * n]
  shared_ptr<Grids> voxelize(int voxelsPerSide, float voxelSize);

  int width, height, depth;
  std::vector<std::shared_ptr<RenderObj>> objs;
};

CONETRACER_NAMESPACE_END

#endif //CONETRACER_SCENE_HPP
