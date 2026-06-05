#pragma once

#include <glad/glad.h>

struct Framebuffer {
    GLuint fbo = 0;
    GLuint color_texture = 0;
    GLuint depth_rbo;
    int width = 0;
    int height = 0;

    bool create(int w, int h);
    void bind() const;
    static void unbind();
    void destroy();
};
