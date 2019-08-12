//
// Created by liao xiangsen on 2019/8/12.
//

#include <glm/gtc/matrix_transform.hpp>

#include "object.hpp"
#include "common.hpp"

CONETRACER_NAMESPACE_BEGIN

RenderObj::RenderObj(const shared_ptr<Mesh> &mesh, const shared_ptr<Material> &material,
                     const glm::vec3 &position, float scale): meshPtr(mesh), materialPtr(material) {
    objMat = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(scale)), position);
}

void RenderObj::draw() {

}

CONETRACER_NAMESPACE_END