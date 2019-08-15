//
// Created by liao xiangsen on 2019/8/11.
//

#ifndef CONETRACER_UTILS_HPP
#define CONETRACER_UTILS_HPP

#include <stb_image.h>

#include "common.hpp"
#include "scene/scene.hpp"

CONETRACER_NAMESPACE_BEGIN

std::shared_ptr<Scene> loadScene(const string &path, const string &resDir, const glm::vec3 &center, const double scale);

Texture2D loadTexture(const string &dir, const string &filename);

GLuint loadAndCompileShader(const string &path, GLenum shaderType);

GLuint linkProgram(GLuint vertShaderID, GLuint fragShaderID, GLuint geomShaderID = 0);

inline int clamp(int value, int low, int high) {
    if (value <= low)
        return low;
    else if (value >= high)
        return high;
    else
        return value;
}

std::ostream& operator<<(std::ostream& os, const glm::mat4& mat);

std::ostream& operator<<(std::ostream& os, const glm::vec3& vec);

std::ostream& operator<<(std::ostream& os, const glm::vec4& vec);

CONETRACER_NAMESPACE_END

#endif //CONETRACER_UTILS_HPP
