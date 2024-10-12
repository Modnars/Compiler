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

namespace {

std::set<std::shared_ptr<Production>> _InProcess;

std::vector<std::string> split(const std::string &str, const std::string &delim) {
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

std::string &trim(std::string &s) {
    if (s.empty()) {
        return s;
    }
    s.erase(0, s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1UL);
    return s;
}

}  // namespace

std::string Production::ToString() const {
    std::stringstream ss;
    ss << this->left_ << " -> ";
    for (const auto &r : this->right_) {
        ss << r << " ";
    }
    return ss.str();
}

int Grammar::ReadFromFile(const std::string &filepath) {
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
        productions_.emplace_back(newProduction);
        productionIndexes_[left].emplace_back(newProduction);
    }
    is.close();
    return 0;
}

void Grammar::ComputeAndCacheFirstSet() {
    for (const auto &kv : this->productionIndexes_) {
        computeAndCacheFirstSet(kv.first);
    }
}

void Grammar::ComputeAndCacheFollowSet() {
    const auto &startSymbol = productions_[0]->Left();
    for (const auto &kv : productionIndexes_) {
        followSet_[kv.first] = std::set<std::string>();
    }
    bool changed = followSet_[startSymbol].insert(Grammar::EndMark).second;
    while (changed) {  // until not changed
        changed = false;
        for (auto p : productions_) {
            const auto &left = p->Left();
            const auto &right = p->Right();
            for (std::size_t i = 0UL; i < right.size(); ++i) {
                const auto &B = right[i];
                if (followSet_.find(B) == followSet_.end()) {  // only focus on non-terminal
                    continue;
                }
                bool containsNil = true;
                for (std::size_t j = i + 1UL; j < right.size(); ++j) {
                    const std::string &symbol = right[j];
                    containsNil = false;
                    auto firstSetOfSymbol = computeAndCacheFirstSet(symbol);
                    for (const auto &terminal : firstSetOfSymbol) {
                        if (terminal == Grammar::NilMark) {
                            containsNil = true;
                            continue;
                        }
                        changed = followSet_[B].insert(terminal).second;
                    }
                    if (!containsNil) {
                        break;
                    }
                }
                if (containsNil) {
                    for (const auto &terminal : followSet_[left]) {
                        changed = followSet_[B].insert(terminal).second;
                    }
                }
            }
        }
    }
}

std::set<std::string> Grammar::computeAndCacheFirstSet(const std::string &symbol) {
    if (!IsNonTerminal(symbol)) {
        return {symbol};
    }
    if (!firstSet_[symbol].empty()) {
        return firstSet_[symbol];
    }
    std::set<std::string> result;
    for (auto production : this->productionIndexes_[symbol]) {
        if (_InProcess.find(production) != _InProcess.end()) {
            continue;
        }
        _InProcess.insert(production);
        bool needAddNil = true;
        for (auto right : production->Right()) {
            auto firstSetOfSymbol = computeAndCacheFirstSet(right);
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
    firstSet_[symbol] = result;
    return result;
}

void Grammar::ShowDetails() const {
    std::cout << "FIRST SET:" << std::endl;
    for (const auto &kv : firstSet_) {
        std::cout << kv.first << ": ";
        for (const auto &symbol : kv.second) {
            std::cout << symbol << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
    std::cout << "FOLLOW SET:" << std::endl;
    for (const auto &kv : followSet_) {
        std::cout << kv.first << ": ";
        for (const auto &symbol : kv.second) {
            std::cout << symbol << " ";
        }
        std::cout << std::endl;
    }
}