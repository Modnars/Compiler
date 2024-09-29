/*
 * @Author: modnarshen
 * @Date: 2024.09.27 16:40:32
 * @Note: Copyrights (c) 2024 modnarshen. All rights reserved.
 */
#include <fstream>
#include <iostream>
#include <sstream>

#include "parser.h"

const std::string Color::RESET  = "\e[0m";
const std::string Color::BLUE   = "\e[0;34m";
const std::string Color::GREEN  = "\e[0;32m";
const std::string Color::RED    = "\e[0;31m";
const std::string Color::WHITE  = "\e[1;37m";
const std::string Color::YELLOW = "\e[1;33m";

namespace {
std::vector<std::string> split(const std::string &str, const std::string &delim) {
    std::vector<std::string> res;
    if ("" == str)
        return res;
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

std::string &trim(std::string &s) {
    if (s.empty())
        return s;
    s.erase(0, s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}
} // namespace

std::string Production::ToString() const {
    std::stringstream ss;
    ss << this->Left << " -> ";
    for (const auto &r : this->Right) {
        ss << r << " ";
    }
    return ss.str();
}

int ReadGrammar(std::string filepath, Grammar &grammar) {
    std::ifstream is(filepath);
    if (!is) {
        std::cerr << Color::RED << "error: failed to open file '" << filepath << "'." << Color::RESET << std::endl;
        return -1;
    }
    std::string line;
    while (getline(is, line)) {
        auto tmpVec      = split(line, "->");
        std::string left = trim(tmpVec[0]);
        std::vector<std::string> right;
        tmpVec = split(trim(tmpVec[1]), " ");
        for (int i = 0; i < tmpVec.size(); ++i)
            right.emplace_back(trim(tmpVec[i]));
        auto newProduction = std::make_shared<Production>(std::move(left), std::move(right));
        grammar.Productions.emplace_back(newProduction);
        grammar.ProductionIndexes[left].emplace_back(newProduction);
    }
    is.close();
    // Production::setStart(prodVec[0]->left); // Set the `Start` symbol.
    // Production::setNull(null);              // Set the `null` symbol.
    return 0;
}
