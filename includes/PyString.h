// PyString.h
#ifndef PYSTRING_H
#define PYSTRING_H

#include <string>
#include <vector>

constexpr char RED[] = "\033[0;31m";
constexpr char GREEN[] = "\033[0;32m";
constexpr char YELLOW[] = "\033[0;33m";
constexpr char BLUE[] = "\033[0;34m";
constexpr char MAGENTA[] = "\033[0;35m";
constexpr char CYAN[] = "\033[0;36m";
constexpr char RESET[] = "\033[0m"; // Reset to default color

class PyString {
public:
    static std::vector<std::string> split(const std::string& str, const std::string& delimiter);
    static std::string join(const std::vector<std::string>& parts, const std::string& delimiter);
    static std::string strip(const std::string& str, const std::string& chars = " \t\n");
    static std::string replace(const std::string& str, const std::string& from, const std::string& to);
    static int find(const std::string& str, const std::string& substr, size_t start = 0);
    static std::string upper(const std::string& str);
    static std::string lower(const std::string& str);
    static bool startswith(const std::string& str, const std::string& start);
    static bool endswith(const std::string& str, const std::string& end);
    static size_t len(const std::string& str);
};

#endif // PYSTRING_H

