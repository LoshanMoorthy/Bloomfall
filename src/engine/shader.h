#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

struct Shader {
    GLuint id = 0;

    bool load(const char *vertex_path, const char *fragment_path);
    void use() const;
    void set_int(const char *name, int value) const;
    void set_mat4(const char *name, const glm::mat4 &value) const;
    void destroy();
};
