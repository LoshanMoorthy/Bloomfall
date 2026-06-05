#include "screen_quad.h"

void ScreenQuad::create() {
    float verts[] = {
        // pos     // uv
        -1,-1,  0,0,   1,-1,  1,0,   1, 1,  1,1,
        -1,-1,  0,0,   1, 1,  1,1,  -1, 1,  0,1,
    };
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

void ScreenQuad::draw() const {
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void ScreenQuad::destroy() {
    if (vbo) glDeleteBuffers(1, &vbo);
    if (vao) glDeleteVertexArrays(1, &vao);
    vao = vbo = 0;
}
