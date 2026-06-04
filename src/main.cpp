#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "engine/shader.h"
#include "engine/mesh.h"
#include "engine/texture.h"
#include "engine/camera.h"
#include "engine/window.h"
#include "engine/cube_mesh.h"

#include "game/world.h"

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const int TARGET_FPS = 144;
const int FRAME_DELAY_MS = 1000 / TARGET_FPS;

int main(int argc, char *argv[]) {
    Window window;
    if (!window.create("Bloomfall", WINDOW_WIDTH, WINDOW_HEIGHT)) {
        return 1;
    }

    IMG_Init(IMG_INIT_PNG);

    glEnable(GL_DEPTH_TEST);

    Shader shader;
    if (!shader.load("D:/c_projects/Bloomfall/res/shaders/basic.vert",
                 "D:/c_projects/Bloomfall/res/shaders/basic.frag")) {
        std::cout << "Failed to create shader\n";
        return 1;
    }

    Mesh cube = create_cube_mesh();
    World world;

    GLuint texture = load_texture("D:/c_projects/Bloomfall/assets/tile/deep_rock.png");
    if (texture == 0) return 1;

    Camera camera;
    camera.position = glm::vec3(5.0f, 0.0f, 5.0f);
    camera.aspect = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
    camera.ortho_size = 7.0f; // smaller = more zoomed

    Uint32 last_tick = SDL_GetTicks();
    bool running = true;
    SDL_Event event;

    while (running) {
        Uint32 frame_start = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        Uint32 current_tick = SDL_GetTicks();
        float delta_time = (current_tick - last_tick) / 1000.0f;

        last_tick = current_tick;

        const Uint8 *keys = SDL_GetKeyboardState(nullptr);

        // note: colors are 0.0-1,0, not 0-255
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        glClearColor(0.08f, 0.08f, 0.10f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.view();
        glm::mat4 projection = camera.projection();

        float t = SDL_GetTicks() / 1000.0f;

        shader.use();
        shader.set_mat4("uView", view);
        shader.set_mat4("uProjection", projection);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        shader.set_int("uTexture", 0);

        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_WIDTH; x++) {
                int h = world.height_at(x, y);
                for (int z = 0; z < h; z++) {
                    glm::mat4 model =
                        glm::translate(
                            glm::mat4(1.0f),
                            glm::vec3((float)x, (float)z, (float)y));
                    shader.set_mat4("uModel", model);
                    draw_mesh(cube);
                }
            }
        }

        window.swap();

        Uint32 frame_time = SDL_GetTicks() - frame_start;
        if (frame_time < FRAME_DELAY_MS) {
            SDL_Delay(FRAME_DELAY_MS - frame_time);
        }
    }

    shader.destroy();
    window.destroy();

    return 0;
}
