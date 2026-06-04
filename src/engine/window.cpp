#include <iostream>

#include "window.h"

bool Window::create(const char *title, int w, int h) {
    width = w;
    height = h;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SDL_init failed: " << SDL_GetError() << "\n";
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    handle = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        w, h, SDL_WINDOW_OPENGL);
    if (!handle) {
        std::cout << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
        SDL_Quit();
        return false;
    }

    gl_context = SDL_GL_CreateContext(handle);
    if (!gl_context) {
        std::cout << "GL context failed: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(handle);
        SDL_Quit();
        return false;
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cout << "glad load failed\n";
        SDL_GL_DeleteContext(gl_context);
        SDL_DestroyWindow(handle);
        SDL_Quit();
        return false;
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";
    return true;
}

void Window::swap() {
    SDL_GL_SwapWindow(handle);
}

void Window::destroy() {
    if (gl_context) SDL_GL_DeleteContext(gl_context);
    if (handle) SDL_DestroyWindow(handle);
    SDL_Quit();
}
