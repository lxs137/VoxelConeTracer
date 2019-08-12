//
// Created by liao xiangsen on 2019/8/12.
//

#ifndef CONETRACER_MESH_HPP
#define CONETRACER_MESH_HPP

#include <assimp/scene.h>

#include "common.hpp"

CONETRACER_NAMESPACE_BEGIN

class Mesh {
public:
  Mesh();
  static shared_ptr<Mesh> build(const aiMesh *mesh);
  friend std::ostream& operator<<(std::ostream& os, const Mesh& mesh);

  // flag
  bool hasTexCoord, hasNormal, hasTangentsAndBitangents;

  unsigned int numVertices, numFaces;

  std::vector<glm::vec3> vertices;
  std::vector<glm::vec2> uvs;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec3> tangents;
  std::vector<glm::vec3> bitangents;
  std::vector<glm::uvec3> indices;
};

CONETRACER_NAMESPACE_END

#endif //CONETRACER_MESH_HPP
