#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "engine/shader.h"
#include "engine/mesh.h"
#include "engine/texture.h"
#include "engine/camera.h"
#include "engine/window.h"
#include "engine/cube_mesh.h"
#include "engine/log.h"
#include "engine/debug.h"
#include "engine/framebuffer.h"
#include "engine/screen_quad.h"

#include "game/world.h"
#include "game/voxel_mesh.h"
#include "game/player.h"

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const int TARGET_FPS = 144;
const int FRAME_DELAY_MS = 1000 / TARGET_FPS;

struct Light {
    glm::vec3 position;
    glm::vec3 color;
};

int main(int argc, char *argv[]) {
    Window window;
    if (!window.create("Bloomfall", WINDOW_WIDTH, WINDOW_HEIGHT)) {
        return 1;
    }

    IMG_Init(IMG_INIT_PNG);

    glEnable(GL_DEPTH_TEST);

    Framebuffer scene_fb;
    scene_fb.create(WINDOW_WIDTH, WINDOW_HEIGHT);

    Framebuffer bright_fb;
    bright_fb.create(WINDOW_WIDTH, WINDOW_HEIGHT);

    Framebuffer blur_fb[2];
    blur_fb[0].create(WINDOW_WIDTH, WINDOW_HEIGHT);
    blur_fb[1].create(WINDOW_WIDTH, WINDOW_HEIGHT);

    ScreenQuad screen_quad;
    screen_quad.create();

    Shader screen_shader;
    screen_shader.load("D:/c_projects/Bloomfall/res/shaders/screen.vert",
                   "D:/c_projects/Bloomfall/res/shaders/screen.frag");

    Shader threshold_shader;
    threshold_shader.load("D:/c_projects/Bloomfall/res/shaders/screen.vert",
                   "D:/c_projects/Bloomfall/res/shaders/threshold.frag");

    Shader blur_shader;
    blur_shader.load("D:/c_projects/Bloomfall/res/shaders/screen.vert",
                   "D:/c_projects/Bloomfall/res/shaders/blur.frag");

    Shader composite_shader;
    composite_shader.load("D:/c_projects/Bloomfall/res/shaders/screen.vert",
                   "D:/c_projects/Bloomfall/res/shaders/composite.frag");

    Shader shader;
    if (!shader.load("D:/c_projects/Bloomfall/res/shaders/basic.vert",
                 "D:/c_projects/Bloomfall/res/shaders/basic.frag")) {
        ERROR("Failed to create shader");
        return 1;
    }

    World world;
    world.generate();

    int cut_height = 100;
    Mesh world_mesh = build_voxel_mesh(world, cut_height);
    Mesh cube = create_cube_mesh();

    GLuint texture = load_texture("D:/c_projects/Bloomfall/assets/tile/deep_rock.png");
    if (texture == 0) return 1;

    Camera camera;
    camera.ortho_size = 10.0f;

    Player player;
    player.position = glm::vec3(WORLD_X/2, 1, WORLD_Z/2);

    Uint32 last_tick = SDL_GetTicks();
    bool running = true;
    SDL_Event event;

    Debug debug;

    std::vector<Light> lights = {
        {{14, 1, 14}, {6.0f, 0.5f, 0.5f}},   // red
        {{18, 1, 16}, {0.5f, 6.0f, 0.5f}},   // green
        {{16, 1, 19}, {0.5f, 0.5f, 6.0f}},   // blue
        {{20, 1, 13}, {6.0f, 5.0f, 0.5f}},   // yellow
    };

    while (running) {
        Uint32 frame_start = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN)
                debug.handle_key(event.key.keysym.scancode);
            if (event.type == SDL_MOUSEWHEEL) {
                camera.ortho_size -= event.wheel.y;
                if (camera.ortho_size < 5) camera.ortho_size = 5;
                if (camera.ortho_size > 60) camera.ortho_size = 60;
            }
        }

        Uint32 current_tick = SDL_GetTicks();
        float delta_time = (current_tick - last_tick) / 1000.0f;

        last_tick = current_tick;

        const Uint8 *keys = SDL_GetKeyboardState(nullptr);

        float move_speed = 8.0f * delta_time;
        glm::vec3 move(0.0f);
        if (keys[SDL_SCANCODE_W]) move.z -= 1.0f;
        if (keys[SDL_SCANCODE_S]) move.z += 1.0f;
        if (keys[SDL_SCANCODE_A]) move.x -= 1.0f;
        if (keys[SDL_SCANCODE_D]) move.x += 1.0f;
        if (glm::length(move) > 0.0f)
            player.position += glm::normalize(move) * move_speed;

        // draw scene into fb
        scene_fb.bind();
        glClearColor(0.08f, 0.08f, 0.10f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        debug.apply();

        camera.target = player.position;
        glm::mat4 view = camera.view();
        glm::mat4 projection = camera.projection();

        shader.use();

        int total_lights = (int)lights.size() + 1;
        shader.set_int("uLightCount", total_lights);
        for (size_t i = 0; i < lights.size(); i++) {
            std::string base = "uLightPos[" + std::to_string(i) + "]";
            std::string cbase = "uLightColor[" + std::to_string(i) + "]";
            shader.set_vec3(base.c_str(), lights[i].position);
            shader.set_vec3(cbase.c_str(), lights[i].color);
        }

        int pi = (int)lights.size();
        std::string pp = "uLightPos["   + std::to_string(pi) + "]";
        std::string pc = "uLightColor[" + std::to_string(pi) + "]";
        shader.set_vec3(pp.c_str(), player.position + glm::vec3(0, 1.0f, 0));
        shader.set_vec3(pc.c_str(), glm::vec3(3.0f, 1.8f, 0.8f));

        shader.set_mat4("uView", view);
        shader.set_mat4("uProjection", projection);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        shader.set_int("uTexture", 0);

        shader.set_vec3("uTint", glm::vec3(1.0f));
        glm::mat4 world_model = glm::mat4(1.0f);
        shader.set_mat4("uModel", world_model);
        draw_mesh(world_mesh);

        shader.set_vec3("uTint", glm::vec3(1.0f, 0.4f, 0.2f));
        glm::mat4 player_model = glm::translate(glm::mat4(1.0f), player.position);
        shader.set_mat4("uModel", player_model);
        draw_mesh(cube);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // keep only bright pixels in to bright_fb
        bright_fb.bind();
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        threshold_shader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, scene_fb.color_texture);
        threshold_shader.set_int("uScene", 0);
        screen_quad.draw();

        // ping-pong blur
        glm::vec2 texel(1.0f / WINDOW_WIDTH, 1.0f / WINDOW_HEIGHT);
        bool horizontal = true;
        blur_shader.use();
        blur_shader.set_vec2("uTexelSize", texel);
        for (int i = 0; i < 10; i++) {
            blur_fb[horizontal].bind();
            glClear(GL_COLOR_BUFFER_BIT);
            blur_shader.set_int("uHorizontal", horizontal ? 1 : 0);
            GLuint src = (i == 0)
                ? bright_fb.color_texture : blur_fb[!horizontal].color_texture;
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, src);
            blur_shader.set_int("uImage", 0);
            screen_quad.draw();
            horizontal = !horizontal;
        }
        GLuint final_bloom = blur_fb[!horizontal].color_texture;

        // composite scene + bloom
        Framebuffer::unbind();
        glClear(GL_COLOR_BUFFER_BIT);
        composite_shader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, scene_fb.color_texture);
        composite_shader.set_int("uScene", 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, final_bloom);
        composite_shader.set_int("uBloom", 1);
        screen_quad.draw();

        window.swap();

        Uint32 frame_time = SDL_GetTicks() - frame_start;
        if (frame_time < FRAME_DELAY_MS) {
            SDL_Delay(FRAME_DELAY_MS - frame_time);
        }
    }

    scene_fb.destroy();
    screen_quad.destroy();
    screen_shader.destroy();
    shader.destroy();
    window.destroy();

    return 0;
}
