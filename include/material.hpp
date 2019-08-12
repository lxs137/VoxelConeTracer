//
// Created by liao xiangsen on 2019/8/12.
//

#ifndef CONETRACER_MATERIAL_HPP
#define CONETRACER_MATERIAL_HPP

#include <assimp/scene.h>

#include "common.hpp"

CONETRACER_NAMESPACE_BEGIN

class Material {
public:
  Material() = default;
  static shared_ptr<Material> build(const aiMaterial *material, const string &resDir);
  void bindShader(GLuint shader) const ;
  string name;
protected:
  glm::vec3 ambientColor, diffuseColor, specularColor, emissiveColor;
  float shininess, opacity;

  Texture2D diffuseTex, specularTex, maskTex, heightTex;
};

CONETRACER_NAMESPACE_END

#endif //CONETRACER_MATERIAL_HPP
