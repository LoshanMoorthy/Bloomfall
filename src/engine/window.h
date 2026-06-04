#pragma once

#include <glad/glad.h>
#include <SDL2/SDL.h>

struct Window {
    SDL_Window *handle = nullptr;
    SDL_GLContext gl_context = nullptr;
    int width = 0;
    int height = 0;

    bool create(const char *title, int w, int h);
    void swap();
    void destroy();
};
