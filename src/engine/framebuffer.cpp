#include "framebuffer.h"
#include "log.h"

bool Framebuffer::create(int w, int h) {
    width = w;
    height = h;

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // HDR
    glGenTextures(1, &color_texture);
    glBindTexture(GL_TEXTURE_2D, color_texture);
    glTexImage2D(
        GL_TEXTURE_2D, 0,
        GL_RGB16F, w, h, 0,
        GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D, color_texture, 0);

    // depth buffer
    glGenRenderbuffers(1, &depth_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, depth_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, w, h);
    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
        GL_RENDERBUFFER, depth_rbo);

    bool ok =
        glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    if (!ok) ERROR("framebuffer incomplete");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return ok;
}

void Framebuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, width, height);
}

void Framebuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::destroy() {
    if (color_texture) glDeleteTextures(1, &color_texture);
    if (depth_rbo)     glDeleteRenderbuffers(1, &depth_rbo);
    if (fbo)           glDeleteFramebuffers(1, &fbo);
    fbo = color_texture = depth_rbo = 0;
}
