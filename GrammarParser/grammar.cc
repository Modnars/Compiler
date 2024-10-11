/*
 * @Author: modnarshen
 * @Date: 2024.10.08 15:36:22
 * @Note: Copyrights (c) 2024 modnarshen. All rights reserved.
 */
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

#include "grammar.h"
#include "lr0.h"
#include "parser.h"

const std::string Grammar::EndMark = "#";
const std::string Grammar::NilMark = "$";

std::set<std::shared_ptr<Production>> _InProcess;

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
}  // namespace

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
    std::int32_t productionNumber = 0;
    while (getline(is, line)) {
        auto tmpVec = split(line, "->");
        std::string left = trim(tmpVec[0]);
        std::vector<std::string> right;
        tmpVec = split(trim(tmpVec[1]), " ");
        for (int i = 0; i < tmpVec.size(); ++i) {
            auto symbol = trim(tmpVec[i]);
            right.emplace_back(symbol);
        }
        auto newProduction = std::make_shared<Production>(std::move(left), std::move(right), ++productionNumber);
        grammar.Productions.emplace_back(newProduction);
        grammar.ProductionIndexes[left].emplace_back(newProduction);
    }
    is.close();
    return 0;
}

int Grammar::ComputeFirstSet() {
    for (const auto &kv : this->ProductionIndexes) {
        computeFirstSet(kv.first);
    }
    return 0;
}

std::set<std::string> Grammar::computeFirstSet(const std::string &symbol) {
    if (IsTerminal(symbol)) {
        return {symbol};
    }
    if (!this->FirstSet[symbol].empty()) {
        return this->FirstSet[symbol];
    }
    std::set<std::string> result;
    for (auto production : this->ProductionIndexes[symbol]) {
        if (_InProcess.find(production) != _InProcess.end()) {
            continue;
        }
        _InProcess.insert(production);
        bool needAddNil = true;
        for (auto right : production->Right) {
            auto firstSetOfSymbol = computeFirstSet(right);
            result.insert(firstSetOfSymbol.begin(), firstSetOfSymbol.end());
            result.erase(Grammar::NilMark);
            if (firstSetOfSymbol.find(Grammar::NilMark) == firstSetOfSymbol.end()) {
                needAddNil = false;
                break;
            }
        }
        if (needAddNil) {
            result.insert(Grammar::NilMark);
        }
        _InProcess.erase(production);
    }
    this->FirstSet[symbol] = result;
    return result;
}
