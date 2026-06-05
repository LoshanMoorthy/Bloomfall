#pragma once

#include <string>
#include <format>
#include <iostream>

#define LOG(s, ...)                                          \
    log::out(log::LEVEL_NORMAL, __FILE__, __LINE__, __FUNCTION__,  \
             std::format(s, ##__VA_ARGS__))

#define WARN(s, ...)                                         \
    log::out(log::LEVEL_WARN, __FILE__, __LINE__, __FUNCTION__,    \
             std::format(s, ##__VA_ARGS__))

#define ERROR(s, ...)                                        \
    log::out(log::LEVEL_ERROR, __FILE__, __LINE__, __FUNCTION__,   \
             std::format(s, ##__VA_ARGS__))

namespace log {
    enum Level { LEVEL_NORMAL, LEVEL_WARN, LEVEL_ERROR, LEVEL_DEBUG };

    void out(Level level,
        const std::string &file,
        int line,
        const std::string &fn,
        const std::string &msg);
}
