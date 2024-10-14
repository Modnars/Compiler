/*
 * @Author: modnarshen
 * @Date: 2024.10.12 17:19:16
 * @Note: Copyrights (c) 2024 modnarshen. All rights reserved.
 */
#pragma once

#include <iostream>
#include <string>
#include <vector>

class Color {
public:
    static const std::string RESET;
    static const std::string BLUE;
    static const std::string GREEN;
    static const std::string RED;
    static const std::string WHITE;
    static const std::string YELLOW;
};

namespace util {

inline std::vector<std::string> Split(const std::string &str, const std::string &delimiter) {
    std::vector<std::string> tokens;

    if (delimiter.empty()) {
        for (auto ch: str) {
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

void LOG_TRACE(const char *format, ...);
void LOG_INFO(const char *format, ...);
void LOG_ERROR(const char *format, ...);

}  // namespace util
