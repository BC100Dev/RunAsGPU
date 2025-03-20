#ifndef RUNASGPU_SHARED_HPP
#define RUNASGPU_SHARED_HPP

#include <filesystem>

std::filesystem::path GetExecutablePath();

std::string TrimString(const std::string &s);

bool StartsWith(const std::string &s, const std::string &v);

bool EndsWith(const std::string &s, const std::string &v);

bool StringContains(const std::string& str, const std::string& val);

#endif //RUNASGPU_SHARED_HPP
