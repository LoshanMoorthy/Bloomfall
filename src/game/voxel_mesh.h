#pragma once

#include "engine/mesh.h"

struct World;

Mesh build_voxel_mesh(const World &world, int cut_heigt);
