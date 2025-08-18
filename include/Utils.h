#pragma once

#include <vector>
#include <string>


std::vector<std::string> split(const std::string& str, const std::string& delimiter);
std::string GetContentTypeFromRequest(const std::string& path);
long long TimestampNow();