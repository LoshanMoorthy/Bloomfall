#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"

glm::mat4 Camera::view() const {
    glm::mat4 v(1.0f);
    v = glm::rotate(v, glm::radians(35.0f), glm::vec3(1, 0, 0));  // pitch down
    v = glm::rotate(v, glm::radians(45.0f), glm::vec3(0, 1, 0));  // yaw
    v = glm::translate(v, -target);
    return v;
}

glm::mat4 Camera::projection() const {
    float hw = ortho_size * aspect;
    return glm::ortho(-hw, hw, -ortho_size, ortho_size, near_plane, far_plane);
}
