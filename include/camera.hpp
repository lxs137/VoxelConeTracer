//
// Created by liao xiangsen on 2019/8/13.
//

#ifndef CONETRACER_CAMERA_HPP
#define CONETRACER_CAMERA_HPP

#include "common.hpp"

CONETRACER_NAMESPACE_BEGIN

const float CAMERA_NEAR_PLANE = 0.1f, CAMERA_FAR_PLANE = 1000.f;

class Controller;

class Camera {
public:
  Camera(const glm::vec3 &_pos, float _yaw, float _pitch, float _roll, const glm::vec3 &_worldUp, float _fov, float _aspectRatio);
  void move(const glm::vec3 &v);
  glm::mat4 getViewMatrix() const ;
  glm::mat4 getProjectionMatrix() const;
  friend class Controller;
private:
  // transform matrix
  glm::mat4 viewMat;
  glm::mat4 projMat;

  // world up
  glm::vec3 worldUp;

  glm::vec3 pos;
  // x: pitch, y: yaw, z: roll
  float yaw, pitch, roll;
  // camera local coordinate direction vector
  glm::vec3 front;
  glm::vec3 right;
  glm::vec3 up;
  // field of view
  float fov;
  // windows aspect ratio, width/height
  float aspectRatio;
  float near = CAMERA_NEAR_PLANE, far = CAMERA_FAR_PLANE;
};

CONETRACER_NAMESPACE_END

#endif //CONETRACER_CAMERA_HPP
