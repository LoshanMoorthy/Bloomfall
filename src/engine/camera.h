#pragma once

#include <glm/glm.hpp>

struct Camera {
    glm::vec3 target{24.0f, 24.0f, 24.0f};
    float ortho_size = 25.0f;
    float aspect = 16.0f / 9.0f;
    float near_plane = -200.0f;
    float far_plane = 200.0f;

    glm::mat4 view() const;
    glm::mat4 projection() const;
};
