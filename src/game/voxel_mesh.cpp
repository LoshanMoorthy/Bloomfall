#include <vector>

#include "voxel_mesh.h"
#include "world.h"

// each face: 6 verts * (3 pos + 3 normal + 2 uv),
// position relative to cell origin
static void add_face(std::vector<float> &v, int x, int y, int z, int dir) {
    float fx = (float)x, fy = (float)y, fz = (float)z;

    // 6 face defs: {nx,ny,nz} normal + 4 corner positions
    // corbers listed so winding faces outward
    struct Face { float n[3]; float c[4][3]; };
    static const Face faces[6] = {
        // +X (right)
        {{1,0,0},  {{1,0,0},{1,1,0},{1,1,1},{1,0,1}}},
        // -X (left)
        {{-1,0,0}, {{0,0,1},{0,1,1},{0,1,0},{0,0,0}}},
        // +Y (top)
        {{0,1,0},  {{0,1,0},{0,1,1},{1,1,1},{1,1,0}}},
        // -Y (bottom)
        {{0,-1,0}, {{0,0,1},{0,0,0},{1,0,0},{1,0,1}}},
        // +Z (front)
        {{0,0,1},  {{1,0,1},{1,1,1},{0,1,1},{0,0,1}}},
        // -Z (back)
        {{0,0,-1}, {{0,0,0},{0,1,0},{1,1,0},{1,0,0}}},
    };
    static const float uv[4][2] = {{0,0},{0,1},{1,1},{1,0}};
    // two triangles from the 4 corners
    static const int tri[6] = {0,1,2, 0,2,3};

    const Face &f = faces[dir];
    for (int i = 0; i < 6; i++) {
        int c = tri[i];
        v.push_back(fx + f.c[c][0]);
        v.push_back(fy + f.c[c][1]);
        v.push_back(fz + f.c[c][2]);
        v.push_back(f.n[0]);
        v.push_back(f.n[1]);
        v.push_back(f.n[2]);
        v.push_back(uv[c][0]);
        v.push_back(uv[c][1]);
    }
}

Mesh build_voxel_mesh(const World &world, int cut_height) {
    std::vector<float> verts;
    // neighbor offsets
    static const int off[6][3] = {
        {1,0,0}, {-1,0,0}, {0,1,0}, {0,-1,0}, {0,0,1}, {0,0,-1}
    };

    for (int z = 0; z < WORLD_Z; z++) {
        for (int y = 0; y < WORLD_Y; y++) {
            if (y > cut_height) continue;
            for (int x = 0; x < WORLD_X; x++) {
                if (!world.is_solid(x, y, z)) continue;
                for (int dir = 0; dir < 6; dir++) {
                    int nx = x + off[dir][0];
                    int ny = y + off[dir][1];
                    int nz = z + off[dir][2];
                    bool neighbor_solid =
                        (ny <= cut_height) && world.is_solid(nx, ny, nz);
                    if (!neighbor_solid)
                        add_face(verts, x, y, z, dir);
                }
            }
        }
    }

    return create_mesh(verts.data(), (int)verts.size());
}
