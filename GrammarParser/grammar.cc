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
#include "util.h"

const std::string Grammar::EndMark = "$";
const std::string Grammar::NilMark = "ε";

namespace {

std::set<std::shared_ptr<Production>> _InProcess;

}  // namespace

std::string Production::ToString() const {
    std::stringstream ss;
    ss << left_ << " -> ";
    for (std::size_t i = 0UL; i < right_.size(); ++i) {
        ss << right_[i];
        if (i + 1UL < right_.size()) {
            ss << " ";
        }
    }
    return ss.str();
}

int Grammar::ReadFromFile(const std::string &filepath) {
    if (!productions_.empty()) {
        util::LOG_ERROR("[ERROR] The grammar object already contains productions");
        return -1;
    }
    std::ifstream ifs(filepath);
    if (!ifs) {
        util::LOG_ERROR("[ERROR] Failed to open file `%s`.", filepath.c_str());
        return -1;
    }
    std::string line;
    std::int32_t productionNumber = 0;
    while (getline(ifs, line)) {
        auto tmpVec = util::Split(line, "->");
        std::string left = util::Trim(tmpVec[0]);
        std::vector<std::string> right;
        tmpVec = util::Split(util::Trim(tmpVec[1]), " ");
        for (int i = 0; i < tmpVec.size(); ++i) {
            auto symbol = util::Trim(tmpVec[i]);
            right.emplace_back(symbol);
        }
        auto newProduction = std::make_shared<Production>(std::move(left), std::move(right), ++productionNumber);
        productions_.emplace_back(newProduction);
        productionIndexes_[left].emplace_back(newProduction);
    }
    ifs.close();
    return 0;
}

void Grammar::ComputeAndCacheFirstSet() {
    if (!firstSet_.empty()) {
        return;
    }
    for (const auto &kv : productionIndexes_) {
        computeAndCacheFirstSet(kv.first);
    }
}

void Grammar::ComputeAndCacheFollowSet() {
    if (!followSet_.empty()) {
        return;
    }
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
                const auto &target = right[i];
                if (followSet_.find(target) == followSet_.end()) {  // only focus on non-terminal
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
                        changed |= followSet_[target].insert(terminal).second;
                    }
                    if (!containsNil) {
                        break;
                    }
                }
                if (containsNil) {
                    for (const auto &terminal : followSet_[left]) {
                        changed |= followSet_[target].insert(terminal).second;
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
    for (auto production : productionIndexes_[symbol]) {
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

std::set<std::string> Grammar::ComputeFirstSet(std::vector<std::string>::const_iterator begin,
                                               std::vector<std::string>::const_iterator end) const {
    std::set<std::string> result;
    bool containsNil = true;
    for (auto iter = begin; iter != end; ++iter) {
        if (IsNonTerminal(*iter)) {
            const auto &firsetSet = FIRST(*iter);
            containsNil = firsetSet.count(Grammar::NilMark);
            result.insert(firsetSet.begin(), firsetSet.end());
        } else {
            containsNil = (*iter == Grammar::NilMark);
            result.insert(*iter);
        }
        result.erase(Grammar::NilMark);
        if (!containsNil) {
            break;
        }
    }
    if (containsNil) {
        result.insert(Grammar::NilMark);
    }
    return result;
}

void Grammar::ShowDetails(std::ostream &os) const {
    os << "FIRST SET:" << std::endl;
    for (const auto &kv : firstSet_) {
        os << kv.first << ": ";
        for (const auto &symbol : kv.second) {
            os << symbol << " ";
        }
        os << std::endl;
    }

    os << std::endl;
    os << "FOLLOW SET:" << std::endl;
    for (const auto &kv : followSet_) {
        os << kv.first << ": ";
        for (const auto &symbol : kv.second) {
            os << symbol << " ";
        }
        os << std::endl;
    }
}
