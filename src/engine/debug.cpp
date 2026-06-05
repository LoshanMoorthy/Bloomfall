#include "debug.h"
#include "log.h"

void Debug::handle_key(SDL_Scancode key) {
    if (key == SDL_SCANCODE_F3) {
        wireframe = !wireframe;
        LOG("debug wireframe: {}", wireframe);
    }
}

void Debug::apply() const {
    glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
}
