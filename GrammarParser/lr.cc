/*
 * @Author: modnarshen
 * @Date: 2024.09.25 11:00:50
 * @Note: Copyrights (c) 2024 modnarshen. All rights reserved.
 */
#include <algorithm>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>

#include "grammar.h"
#include "lr.h"
#include "util.h"

namespace {

static const std::string _DotMark = "·";

}

namespace mcc {

std::string LR0Item::ToString() const {
    std::stringstream ss;
    ss << production_->Left() << " -> ";
    const auto &right = production_->Right();
    for (std::size_t i = 0UL; i <= right.size(); ++i) {
        if (i == dotPos_) {
            ss << _DotMark << "";
        }
        if (i < right.size()) {
            ss << right[i];
        }
        if (i + 1UL < right.size()) {
            ss << " ";
        }
    }
    return ss.str();
}

int LRParser::Analyze(std::istream &is) const {
    std::string line;
    while (std::getline(is, line)) {
        std::stack<std::size_t> stateStack;

        stateStack.push(0UL);

        auto tokens = util::Split(util::Trim(line), " ");
        tokens.emplace_back(Grammar::EndMark);
        std::cout << "tokens: ";
        for (const auto &tok : tokens) {
            std::cout << tok << " ";
        }
        std::cout << std::endl;
        bool acceptSucc = true;
        std::size_t idx = 0UL;
        while (idx < tokens.size()) {
            std::int64_t val;
            if (auto ret = searchActionTable(stateStack.top(), tokens[idx], val); ret != 0) {
                acceptSucc = false;
                break;
            }
            if (val >= 0) {
                stateStack.push(val);
                ++idx;
            } else {
                if (val == -1L && tokens[idx] == Grammar::EndMark) {
                    acceptSucc = true;
                    break;
                }
                auto reduceProduction = grammar_.GetProduction(-static_cast<int32_t>(val));
                util::LOG_TRACE("[REDUCE] %s", reduceProduction->ToString().c_str());
                if (reduceProduction->Right()[0] != Grammar::NilMark) {
                    for (std::size_t i = 0UL; i < reduceProduction->Right().size(); ++i) {
                        stateStack.pop();
                    }
                }
                if (auto ret = searchActionTable(stateStack.top(), reduceProduction->Left(), val); ret != 0) {
                    acceptSucc = false;
                    break;
                } else {
                    stateStack.push(static_cast<std::size_t>(val));
                }
            }
        }
        if (acceptSucc) {
            util::LOG_INFO("ACCEPT");
        } else {
            util::LOG_ERROR("ANALYZE FAILED");
        }
    }
    return 0;
}

void LRParser::fillActionTable(std::size_t stateNum, const std::string &symbol, std::int64_t val) {
    if (actionTable_.size() < stateNum + 1UL) {
        actionTable_.reserve(stateNum + 1UL);
        actionTable_.insert(actionTable_.end(), stateNum + 1UL - actionTable_.size(),
                            std::map<std::string, std::int64_t>());
    }
    if (actionTable_[stateNum].find(symbol) != actionTable_[stateNum].end()) {
        util::LOG_ERROR("conflict!");
        return;
    }
    actionTable_[stateNum][symbol] = val;
}

int LRParser::searchActionTable(std::size_t stateNum, const std::string &symbol, std::int64_t &val) const {
    if (stateNum >= actionTable_.size()) {
        return -1;
    }
    auto iter = actionTable_[stateNum].find(symbol);
    if (iter == actionTable_[stateNum].end()) {
        return -1;
    }
    val = iter->second;
    return 0;
}

}  // namespace mcc
