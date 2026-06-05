#pragma once

#include "mesh.h"

// a fullscreen quad in NDC iwth UVs (pos2+uv2)
struct ScreenQuad {
    GLuint vao = 0;
    GLuint vbo = 0;
    void create();
    void draw() const;
    void destroy();
};
