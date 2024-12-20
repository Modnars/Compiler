/*
 * @Author: modnarshen
 * @Date: 2024.10.12 17:19:16
 * @Note: Copyrights (c) 2024 modnarshen. All rights reserved.
 */
#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace util {

extern bool IsVerboseMode;
static bool IsDebugMode = false;

inline std::vector<std::string> Split(const std::string &str, const std::string &delimiter) {
    std::vector<std::string> tokens;

    if (delimiter.empty()) {
        for (auto ch : str) {
            tokens.emplace_back(std::string(1, ch));
        }
        return tokens;
    }

    std::size_t start = 0;
    std::size_t end = str.find(delimiter);

    while (end != std::string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + delimiter.size();
        end = str.find(delimiter, start);
    }

    tokens.push_back(str.substr(start));

    return tokens;
}

inline std::string &Trim(std::string &s) {
    if (s.empty()) {
        return s;
    }
    s.erase(0, s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1UL);
    return s;
}

template <typename _Cntr>
inline void PrintSTLContainter(const _Cntr &cntr) {
    for (auto iter = cntr.begin(); iter != cntr.end(); ++iter) {
        std::cout << *iter << " ";
    }
    std::cout << std::endl;
}

void LOG_TRACE(const char *format, ...);
void LOG_INFO(const char *format, ...);
void LOG_WARN(const char *format, ...);
void LOG_ERROR(const char *format, ...);

inline std::string Indent(std::uint32_t num, char indentCh = ' ') {
    if (indentCh == ' ') {
        num *= 4U;
    }
    return std::string(num, indentCh);
}

inline std::string Basename(const std::string &filepath) {
    return filepath.substr(filepath.find_last_of("/") + 1UL);
}

inline std::string Filename(const std::string &filepath) {
    auto filename = filepath.substr(filepath.find_last_of("/") + 1UL);
    return filename.substr(0, filename.find_last_of("."));
}

}  // namespace util
