#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "game/player.h"
#include "graphics/shader.h"
#include "graphics/mesh.h"
#include "graphics/texture.h"

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const int TARGET_FPS = 144;
const int FRAME_DELAY_MS = 1000 / TARGET_FPS;

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SDL_init failed: " << SDL_GetError() << "\n";
        return 1;
    }

    // requesting OpenGL
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window *window = SDL_CreateWindow(
        "Bloomfall",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL
    );

    if (window == nullptr) {
        std::cout << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    // Create GL context
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (!gl_context) {
        std::cout << "GL context failed: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    // Load all OpenGL functions via SDL
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cout << "glad load failed\n";
        SDL_Quit();
        return 1;
    }
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";

    glEnable(GL_DEPTH_TEST);

    GLuint shader_program = create_shader_program(
        "D:/c_projects/Bloomfall/res/shaders/basic.vert",
        "D:/c_projects/Bloomfall/res/shaders/basic.frag"
    );
    if (shader_program == 0) {
        std::cout << "Failed to create shader program\n";
        return 1;
    }

    float vertices[] = {
        // back face
        -0.5f,-0.5f,-0.5f, 0,0,  0.5f,-0.5f,-0.5f, 1,0,  0.5f, 0.5f,-0.5f, 1,1,
         0.5f, 0.5f,-0.5f, 1,1, -0.5f, 0.5f,-0.5f, 0,1, -0.5f,-0.5f,-0.5f, 0,0,
        // front face
        -0.5f,-0.5f, 0.5f, 0,0,  0.5f,-0.5f, 0.5f, 1,0,  0.5f, 0.5f, 0.5f, 1,1,
         0.5f, 0.5f, 0.5f, 1,1, -0.5f, 0.5f, 0.5f, 0,1, -0.5f,-0.5f, 0.5f, 0,0,
        // left face
        -0.5f, 0.5f, 0.5f, 1,0, -0.5f, 0.5f,-0.5f, 1,1, -0.5f,-0.5f,-0.5f, 0,1,
        -0.5f,-0.5f,-0.5f, 0,1, -0.5f,-0.5f, 0.5f, 0,0, -0.5f, 0.5f, 0.5f, 1,0,
        // right face
         0.5f, 0.5f, 0.5f, 1,0,  0.5f, 0.5f,-0.5f, 1,1,  0.5f,-0.5f,-0.5f, 0,1,
         0.5f,-0.5f,-0.5f, 0,1,  0.5f,-0.5f, 0.5f, 0,0,  0.5f, 0.5f, 0.5f, 1,0,
        // bottom face
        -0.5f,-0.5f,-0.5f, 0,1,  0.5f,-0.5f,-0.5f, 1,1,  0.5f,-0.5f, 0.5f, 1,0,
         0.5f,-0.5f, 0.5f, 1,0, -0.5f,-0.5f, 0.5f, 0,0, -0.5f,-0.5f,-0.5f, 0,1,
        // top face
        -0.5f, 0.5f,-0.5f, 0,1,  0.5f, 0.5f,-0.5f, 1,1,  0.5f, 0.5f, 0.5f, 1,0,
         0.5f, 0.5f, 0.5f, 1,0, -0.5f, 0.5f, 0.5f, 0,0, -0.5f, 0.5f,-0.5f, 0,1,
    };

    Mesh quad = create_mesh(vertices, sizeof(vertices) / sizeof(float));

    GLuint texture = load_texture("D:/c_projects/Bloomfall/assets/tile/deep_rock.png");
    if (texture == 0) return 1;

    glm::mat4 projection =
        glm::perspective(glm::radians(45.0f),
        (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT,
        0.1f, 100.0f);

    Player player;
    player.x = 5.0f;
    player.y = 5.0f;

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
        Uint32 delta_time = (current_tick - last_tick) / 1000.0f;

        last_tick = current_tick;

        const Uint8 *keys = SDL_GetKeyboardState(nullptr);

        // note: colors are 0.0-1,0, not 0-255
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        glClearColor(0.08f, 0.08f, 0.10f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader_program);

        glm::mat4 view =
            glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));

        float t = SDL_GetTicks() / 1000.0f;
        glm::mat4 model =
            glm::rotate(glm::mat4(1.0f), t, glm::vec3(0.5, 1.0f, 0.0f));

        glUniformMatrix4fv(glGetUniformLocation(shader_program, "uModel"),
                           1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shader_program, "uView"),
                           1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader_program, "uProjection"),
                           1, GL_FALSE, glm::value_ptr(projection));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(shader_program, "uTexture"), 0);

        draw_mesh(quad);

        move_player(player, keys, delta_time);

        SDL_GL_SwapWindow(window);

        Uint32 frame_time = SDL_GetTicks() - frame_start;
        if (frame_time < FRAME_DELAY_MS) {
            SDL_Delay(FRAME_DELAY_MS - frame_time);
        }
    }

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
