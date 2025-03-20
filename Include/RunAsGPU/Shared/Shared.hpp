#ifndef RUNASGPU_SHARED_HPP
#define RUNASGPU_SHARED_HPP

#include <filesystem>

std::filesystem::path GetExecutablePath();

std::string TrimString(const std::string& s);

#endif //RUNASGPU_SHARED_HPP
