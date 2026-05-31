#pragma once

#include <glad/glad.h>
#include <string>

// Loads, compiles and links a vert + fragment shader pair.
GLuint create_shader_program(const char *vertex_path, const char *fragment_path);
