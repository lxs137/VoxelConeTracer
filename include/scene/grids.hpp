//
// Created by liao xiangsen on 2019/8/14.
//

#ifndef CONETRACER_GRIDS_HPP
#define CONETRACER_GRIDS_HPP

#include "common.hpp"

#include "voxelizer.hpp"

CONETRACER_NAMESPACE_BEGIN

class Grids {
public:
  Grids() = default;
  Grids(int w, int h, int d, vx_point_cloud_t *points, float voxelSize_);
  void addVoxels(vx_point_cloud_t *points);
  inline int getOffset(int x, int y, int z) {
      return (x + y * height + z * planeSize);
  }
  inline float getDensity() {
      return (float)voxels.size() / (planeSize * depth);
  };
  // planeSize = width * height
  int width, height, depth, planeSize;
  float voxelSize;
  std::vector<glm::ivec3> voxels;
};

CONETRACER_NAMESPACE_END

#endif //CONETRACER_GRIDS_HPP
