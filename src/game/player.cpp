#include "player.h"

void update_player_rect(Player &player) {
    player.rect.x = static_cast<int>(player.x);
    player.rect.y = static_cast<int>(player.y);
    player.rect.w = player.size;
    player.rect.h = player.size;
}

void move_player(
    Player &player,
    const uint8_t *keys,
    float delta_time
) {
    float dx = 0.0f;
    float dy = 0.0f;

    if (keys[SDL_SCANCODE_W]) {
        dx -= 1.0f;
        dy -= 1.0f;
    }
    if (keys[SDL_SCANCODE_S]) {
        dx += 1.0f;
        dy += 1.0f;
    }
    if (keys[SDL_SCANCODE_A]) {
        dx -= 1.0f;
        dy += 1.0f;
    }
    if (keys[SDL_SCANCODE_D]) {
        dx += 1.0f;
        dy -= 1.0f;
    }

    player.x += dx * player.speed * delta_time;
    player.y += dy * player.speed * delta_time;
}

void clamp_player_to_window(
    Player &player,
    int window_width,
    int window_height
) {
    if (player.x < 0.0f)
        player.x = 0.0f;

    if (player.x > window_width - player.size)
        player.x = static_cast<float>(window_width - player.size);

    if (player.y < 0.0f)
        player.y = 0.0f;

    if (player.y > window_height - player.size)
        player.y = static_cast<float>(window_height - player.size);
}

static SDL_Color shade(SDL_Color c, float f) {
    auto clamp = [](float v) -> Uint8 {
        if (v < 0)
            return 0;
        if (v > 255)
            return 255;
        return (Uint8)v;
    };
    return SDL_Color{clamp(c.r * f), clamp(c.g * f), clamp(c.b * f), c.a};
}

void render_player(
    SDL_Renderer *renderer,
    const Camera &camera,
    float world_x,
    float world_y,
    float z,
    SDL_Color base,
    int window_width
) {
    const float tile_width = 64.0f;
    const float tile_height = 32.0f;

    float pw = tile_width * 0.35f;
    float ph = tile_height * 0.35f;
    float player_h = 40.0f;

    const int origin_x = window_width / 2;
    const int origin_y = 100;

    float cx =
        origin_x + (world_x - world_y) * (tile_width / 2) - camera.x;
    float cy =
        origin_y + (world_x + world_y) * (tile_height / 2) - camera.y - z * player_h;

    // so player stands on tile?
    cy -= player_h + 2 * ph;

    // Just to copy (will fix later)
    float hw = pw;
    float hh = ph;

    SDL_FPoint T{cx, cy};
    SDL_FPoint R{cx + hw, cy + hh};
    SDL_FPoint B{cx, cy + 2 * hh};
    SDL_FPoint L{cx - hw, cy + hh};
    SDL_FPoint Bp(B.x, B.y + player_h);
    SDL_FPoint Rp(R.x, R.y + player_h);
    SDL_FPoint Lp(L.x, L.y + player_h);

    SDL_Color top_c = shade(base, 1.00f);
    SDL_Color left_c = shade(base, 0.75f);
    SDL_Color right_c = shade(base, 0.55f);

    // L, B, Bp, Lp -> diagonal L-Bp -> (L,B,Bp)+(L,Bp,Lp)
    SDL_Vertex left_face[6] = {
        {{L.x, L.y}, left_c, {0, 0}},
        {{B.x, B.y}, left_c, {0, 0}},
        {{Bp.x, Bp.y}, left_c, {0, 0}},
        {{L.x, L.y}, left_c, {0, 0}},
        {{Bp.x, Bp.y}, left_c, {0, 0}},
        {{Lp.x, Lp.y}, left_c, {0, 0}},
    };

    // R, B, Bp, Rp -> diagonal R-Bp -> (R,B,Bp)+(R,Bp,Rp)
    SDL_Vertex right_face[6] = {
        {{R.x, R.y}, right_c, {0, 0}},
        {{B.x, B.y}, right_c, {0, 0}},
        {{Bp.x, Bp.y}, right_c, {0, 0}},
        {{R.x, R.y}, right_c, {0, 0}},
        {{Bp.x, Bp.y}, right_c, {0, 0}},
        {{Rp.x, Rp.y}, right_c, {0, 0}},
    };

    // T, R, B, L -> (T,R,B)+(T,B,L)
    SDL_Vertex top_face[6] = {
        {{T.x, T.y}, top_c, {0, 0}},
        {{R.x, R.y}, top_c, {0, 0}},
        {{B.x, B.y}, top_c, {0, 0}},
        {{T.x, T.y}, top_c, {0, 0}},
        {{B.x, B.y}, top_c, {0, 0}},
        {{L.x, L.y}, top_c, {0, 0}},
    };

    SDL_RenderGeometry(
        renderer,
        nullptr,
        left_face,
        6,
        nullptr,
        0
    );
    SDL_RenderGeometry(
        renderer,
        nullptr,
        right_face,
        6,
        nullptr,
        0
    );
    SDL_RenderGeometry(
        renderer,
        nullptr,
        top_face,
        6,
        nullptr,
        0
    );
}
