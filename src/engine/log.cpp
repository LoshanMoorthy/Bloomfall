#include "log.h"

void log::out(Level level,
    const std::string &file,
    int line,
    const std::string &fn,
    const std::string &msg) {
    static const char *names[] = { "NORMAL", "WARN", "ERROR", "DEBUG" };

    std::string f = file;
    auto pos = f.find("src/");
    if (pos != std::string::npos) f = f.substr(pos + 4);

    std::ostream &os = (level == LEVEL_ERROR) ? std::cerr : std::cout;
    os << "[" << names[level] << "][" << f << ":" << line << "][" << fn << "] " << msg;
    if (msg.empty() || msg.back() != '\n') os << "\n";
}
