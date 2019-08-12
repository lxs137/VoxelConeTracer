//
// Created by liao xiangsen on 2019/8/11.
//

#ifndef CONETRACER_UTILS_HPP
#define CONETRACER_UTILS_HPP

#include <stb_image.h>

#include "common.hpp"
#include "scene.hpp"

CONETRACER_NAMESPACE_BEGIN

std::shared_ptr<Scene> loadScene(const string &path, const string &resDir, const glm::vec3 &center, const double scale);

Texture2D loadTexture(const string &dir, const string &filename);

CONETRACER_NAMESPACE_END

#endif //CONETRACER_UTILS_HPP
