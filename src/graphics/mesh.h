#pragma once

#include <glad/glad.h>

struct Mesh {
    GLuint vao = 0;
    GLuint vbo = 0;
    int vertex_count = 0;
};

// Build a mesh from interleaved [pos(3) + uv(2)] vertex data
Mesh create_mesh(const float *vertices, int float_count);
void draw_mesh(const Mesh &mesh);
