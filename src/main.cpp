#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glad/glad.h>
#include <iostream>
#include <vector>

#include "game/player.h"
#include "graphics/shader.h"

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

const int TARGET_FPS = 144;
const int FRAME_DELAY_MS = 1000 / TARGET_FPS;

constexpr int map_width = 10;
constexpr int map_height = 10;

int world[map_height][map_width] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

int height_map[map_height][map_width] = {
    {1, 1, 1, 4, 1, 1, 1, 1, 1, 1},
    {1, 2, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 2, 1, 1, 1, 1},
    {1, 1, 1, 1, 2, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 3, 1, 1, 1, 1, 1},
    {1, 1, 1, 2, 3, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 3, 3, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 3, 1}
};

enum class DrawKind { Cube,
                      Player };
struct DrawCube {
    DrawKind kind;
    float x, y, z;
    SDL_Color color;
};

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

    GLuint shader_program = create_shader_program(
        "D:/c_projects/Bloomfall/res/shaders/basic.vert",
        "D:/c_projects/Bloomfall/res/shaders/basic.frag"
    );
    if (shader_program == 0) {
        std::cout << "Failed to create shader program\n";
    }
    std::cout << "Shader program created: " << shader_program << "\n";

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";

    float vertices[] = {
        // pos                // UV
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bl
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // br
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // tr
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bl
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // tr
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f, // tl
    };

    GLuint vao, vbo;

    // VAO
    glGenVertexArrays(1, &vao);
    // VBO (the gpu mem)
    glGenBuffers(1, &vbo);

    // record into VAO
    glBindVertexArray(vao);

    // activate the vbo
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // upload data
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer
        (1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    SDL_Surface *surface =
        IMG_Load("D:/c_projects/Bloomfall/assets/tile/deep_rock.png");
    if (!surface) {
        std::cout << "IMG_Load failed: " << IMG_GetError() << "\n";
        return 1;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // wrapping + filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // upload the pixels
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    SDL_FreeSurface(surface);

    /* ---- old SDL rendering disabled ----
    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    SDL_Texture *deep_rock_texture = nullptr;

    SDL_Surface *surface =
        IMG_Load("D:/c_projects/Bloomfall/assets/tile/deep_rock.png");

    if (surface == nullptr) {
        std::cout << "IMG_Load failed: " << IMG_GetError() << "\n";
        return 1;
    }

    deep_rock_texture =
        SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);

    if (deep_rock_texture == nullptr) {
        std::cout << "SDL_CreateTextureFromSurface failed: "
                  << SDL_GetError() << "\n";
        return 1;
    }
    */

    Player player;
    player.x = 5.0f;
    player.y = 5.0f;

    Uint32 last_tick = SDL_GetTicks();

    std::vector<SDL_Rect> walls{};
    Camera camera;

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
        Uint32 delta_ms = current_tick - last_tick;
        float delta_time = delta_ms / 1000.0f;

        last_tick = current_tick;

        const Uint8 *keys = SDL_GetKeyboardState(nullptr);

        // note: colors are 0.0-1,0, not 0-255
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        glClearColor(0.08f, 0.08f, 0.10f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(shader_program, "uTexture"), 0);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        if (keys[SDL_SCANCODE_UP])
            camera.y -= 300.0f * delta_time;

        if (keys[SDL_SCANCODE_DOWN])
            camera.y += 300.0f * delta_time;

        if (keys[SDL_SCANCODE_LEFT])
            camera.x -= 300.0f * delta_time;

        if (keys[SDL_SCANCODE_RIGHT])
            camera.x += 300.0f * delta_time;

        /* ---- old SDL draw loop disabled ----
        std::vector<DrawCube> cubes;
        for (int y{}; y < map_height; y++) {
            for (int x{}; x < map_width; x++) {
                int h = height_map[y][x];
                for (int z{}; z < h; z++) {
                    cubes.push_back(
                        {DrawKind::Cube,
                         (float)x,
                         (float)y,
                         (float)z,
                         SDL_Color{120, 120, 120, 155}}
                    );
                }
            }
        }

        cubes.push_back(
            {DrawKind::Player,
             player.x,
             player.y,
             0.0f,
             SDL_Color{220, 120, 60, 22}}
        );

        std::sort(cubes.begin(), cubes.end(), [](const DrawCube &a, const DrawCube &b) {
            if (a.x + a.y != b.x + b.y)
                return (a.x + a.y) < (b.x + b.y);
            return a.z < b.z;
        });

        for (const DrawCube &c : cubes) {
            if (c.kind == DrawKind::Cube)
                render_cube(renderer, camera, c.x, c.y, c.z, c.color, WINDOW_WIDTH);
            else
                render_player(renderer, camera, c.x, c.y, 0.0f, c.color, WINDOW_WIDTH);
        };
        */

        move_player(player, keys, delta_time);

        if (player.x < 0.0f)
            player.x = 0.0f;
        if (player.x > map_width - 1)
            player.x = map_width - 1;
        if (player.y < 0.0f)
            player.y = 0.0f;
        if (player.y > map_height - 1)
            player.y = map_height - 1;

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
