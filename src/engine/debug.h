#pragma once

#include <glad/glad.h>
#include <SDL2/SDL.h>

struct Debug {
    bool wireframe = false;

    // call once per SDL_KEYDOWN event with scancode
    void handle_key(SDL_Scancode key);

    // call each frame before drawing
    void apply() const;
};
