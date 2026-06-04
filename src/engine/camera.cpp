#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"

glm::mat4 Camera::view() const {
    glm::mat4 v(1.0f);

    // tilt down -30deg, rotate 45deg
    v = glm::rotate(
        v, glm::radians(30.0f),
        glm::vec3(1.0f, 0.0f, 0.0f)); // pitch
    v = glm::rotate(
        v, glm::radians(45.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)); // yaw
    v = glm::translate(v, -position);
    return v;
}

glm::mat4 Camera::projection() const {
    float half_w = ortho_size * aspect;
    float half_h = ortho_size;
    return glm::ortho(-half_w, half_w, -half_h, half_h, near_plane, far_plane);
}
