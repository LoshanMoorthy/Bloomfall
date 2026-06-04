#pragma once

#include <glm/glm.hpp>

struct Camera {
    glm::vec3 position{0.0f, 0.0f, 3.0f};
    float aspect = 16.0f / 9.0f;
    float near_plane = -100.0f;
    float far_plane = 100.0f;
    float ortho_size = 10.0f;

    glm::mat4 view() const;
    glm::mat4 projection() const;
};
