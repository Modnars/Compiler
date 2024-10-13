/*
 * @Author: modnarshen
 * @Date: 2024.10.12 17:19:16
 * @Note: Copyrights (c) 2024 modnarshen. All rights reserved.
 */
#pragma once

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

inline std::vector<std::string> Split(const std::string &str, const std::string &delim) {
    std::vector<std::string> res;
    if ("" == str) {
        return res;
    }
    std::string strs = str + delim;
    int pos, size = strs.size();
    for (int i = 0; i < size; ++i) {
        pos = strs.find(delim, i);
        if (pos < size) {
            res.emplace_back(strs.substr(i, pos - i));
            i = pos + delim.size() - 1;
        }
    }
    return res;
}

inline std::string &Trim(std::string &s) {
    if (s.empty()) {
        return s;
    }
    s.erase(0, s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1UL);
    return s;
}

void LOG_INFO(const char *format, ...);
void LOG_ERROR(const char *format, ...);

}  // namespace util
