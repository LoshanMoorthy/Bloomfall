#include <fstream>
#include <iostream>
#include <sstream>

#include "shader.h"

// Read an entire text file into a string.
static std::string read_file(const char *path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "Failed to open shader file: " << path << "\n";
        return "";
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

// compile one shader stage. Returns shader ID, or 0 on failure.
static GLuint compile_shader(GLenum type, const std::string &source, const char *label) {
    GLuint shader = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint ok = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        std::cout << "Shader compile failed (" << label << "):\n"
                  << log << "\n";
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint create_shader_program(const char *vertex_path, const char *fragment_path) {
    std::string vertex_src = read_file(vertex_path);
    std::string fragment_src = read_file(fragment_path);
    if (vertex_src.empty() || fragment_src.empty())
        return 0;

    GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_src, "vertex");
    GLuint fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_src, "fragment");
    if (vertex_shader == 0 || fragment_shader == 0)
        return 0;

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    GLint ok = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[512];
        glGetProgramInfoLog(program, 512, nullptr, log);
        std::cout << "Shader link failed:\n"
                  << log << "\n";
        glDeleteProgram(program);
        return 0;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}
