//
// Created by liao xiangsen on 2019/8/12.
//

#ifndef CONETRACER_OBJECT_HPP
#define CONETRACER_OBJECT_HPP

#include "common.hpp"
#include "material.hpp"
#include "mesh.hpp"

CONETRACER_NAMESPACE_BEGIN

class RenderObj {
public:
  RenderObj(const shared_ptr<Mesh> &mesh, const shared_ptr<Material> &material,
            const glm::vec3& pos= glm::vec3(0.0f), float scale = 1.0f);
  void draw();

  shared_ptr<Mesh> meshPtr;
  shared_ptr<Material> materialPtr;
  glm::mat4 objMat;
};

CONETRACER_NAMESPACE_END

#endif //CONETRACER_OBJECT_HPP
