#include <filesystem>
#include <climits>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __linux__
#include <unistd.h>
#endif

namespace fs = std::filesystem;

fs::path GetExecutablePath() {
#ifdef _WIN32
    char buffer[MAX_PATH];
    GetModuleFileName(nullptr, buffer, MAX_PATH);
    fs::path exePath = std::string(buffer);
    return exePath;
#else
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    fs::path exePath = std::string(result, (count > 0) ? count : 0);
    return exePath;
#endif
}

std::string TrimString(const std::string &s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos)
        return "";

    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

bool StartsWith(const std::string &str, const std::string &prefix) {
    if (prefix.size() > str.size())
        return false;

    return std::equal(prefix.begin(), prefix.end(), str.begin());
}

bool EndsWith(const std::string &str, const std::string &suffix) {
    if (suffix.size() > str.size())
        return false;

    return std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
}

bool StringContains(const std::string& str, const std::string& val) {
    return str.find(val) != std::string::npos;
}