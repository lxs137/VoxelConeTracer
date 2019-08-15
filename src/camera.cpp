//
// Created by liao xiangsen on 2019/8/13.
//

#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "utils.hpp"

CONETRACER_NAMESPACE_BEGIN

Camera::Camera(const glm::vec3 &_pos, float _yaw, float _pitch, float _roll, const glm::vec3 &_worldUp, float _fov,
               float _aspectRatio): pos(_pos), yaw(_yaw), pitch(_pitch), roll(_roll), worldUp(_worldUp), fov(_fov),
                                    aspectRatio(_aspectRatio) {
    front.x = cos(yaw) * cos(pitch);
    front.y = sin(pitch);
    front.z = sin(yaw) * cos(pitch);
    front = glm::normalize(front);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
    cout << "front: " << front << endl
         << "right: " << right << endl
         << "up: " << up << endl;
}

void Camera::move(const glm::vec3 &v) {
    pos += v;
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(pos, pos + front, up);
}

glm::mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(fov, aspectRatio, near, far);
}

CONETRACER_NAMESPACE_END

