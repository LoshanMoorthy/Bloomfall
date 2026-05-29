#include "cube.h"

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

void render_cube(
    SDL_Renderer *renderer,
    const Camera &camera,
    int world_x,
    int world_y,
    int z,
    SDL_Color base,
    int window_width
) {
    const int tile_width = 64;
    const int tile_height = 32;
    const int cube_h = 32;

    const int origin_x = window_width / 2;
    const int origin_y = 100;

    int cx =
        origin_x + (world_x - world_y) * (tile_width / 2) - static_cast<int>(camera.x);
    int cy =
        origin_y + (world_x + world_y) * (tile_height / 2) - static_cast<int>(camera.y) - z * cube_h;

    int hw = tile_width / 2;
    int hh = tile_height / 2;

    SDL_Point T{cx, cy};
    SDL_Point R{cx + hw, cy + hh};
    SDL_Point B{cx, cy + 2 * hh};
    SDL_Point L{cx - hw, cy + hh};
    SDL_Point Bp(B.x, B.y + cube_h);
    SDL_Point Rp(R.x, R.y + cube_h);
    SDL_Point Lp(L.x, L.y + cube_h);

    SDL_Color top_c = shade(base, 1.00f);
    SDL_Color left_c = shade(base, 0.75f);
    SDL_Color right_c = shade(base, 0.55f);

    // L, B, Bp, Lp -> diagonal L-Bp -> (L,B,Bp)+(L,Bp,Lp)
    SDL_Vertex left_face[6] = {
        {(float)L.x, (float)L.y, left_c, {0, 0}},
        {(float)B.x, (float)B.y, left_c, {0, 0}},
        {(float)Bp.x, (float)Bp.y, left_c, {0, 0}},
        {(float)L.x, (float)L.y, left_c, {0, 0}},
        {(float)Bp.x, (float)Bp.y, left_c, {0, 0}},
        {(float)Lp.x, (float)Lp.y, left_c, {0, 0}},
    };

    // R, B, Bp, Rp -> diagonal R-Bp -> (R,B,Bp)+(R,Bp,Rp)
    SDL_Vertex right_face[6] = {
        {(float)R.x, (float)R.y, right_c, {0, 0}},
        {(float)B.x, (float)B.y, right_c, {0, 0}},
        {(float)Bp.x, (float)Bp.y, right_c, {0, 0}},
        {(float)R.x, (float)R.y, right_c, {0, 0}},
        {(float)Bp.x, (float)Bp.y, right_c, {0, 0}},
        {(float)Rp.x, (float)Rp.y, right_c, {0, 0}},
    };

    // T, R, B, L -> (T,R,B)+(T,B,L)
    SDL_Vertex top_face[6] = {
        {(float)T.x, (float)T.y, top_c, {0, 0}},
        {(float)R.x, (float)R.y, top_c, {0, 0}},
        {(float)B.x, (float)B.y, top_c, {0, 0}},
        {(float)T.x, (float)T.y, top_c, {0, 0}},
        {(float)B.x, (float)B.y, top_c, {0, 0}},
        {(float)L.x, (float)L.y, top_c, {0, 0}},
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
