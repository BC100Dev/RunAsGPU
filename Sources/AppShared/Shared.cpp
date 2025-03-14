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