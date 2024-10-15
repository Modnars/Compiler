/*
 * @Author: modnarshen
 * @Date: 2024.09.25 11:00:50
 * @Note: Copyrights (c) 2024 modnarshen. All rights reserved.
 */
#include <algorithm>
#include <deque>
#include <iostream>
#include <ostream>
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
    if (!parsedSucc_) {
        util::LOG_WARN("[WARNING] grammar parsed failed, so the analyzing result is not valuable");
    }
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
                util::LOG_ERROR("[SHIFT] (%d, %s)", stateStack.top(), tokens[idx].c_str());
                acceptSucc = false;
                break;
            }
            if (val >= 0) {
                util::LOG_TRACE("[SHIFT] (%d, %s) -> %ld", stateStack.top(), tokens[idx].c_str(), val);
                stateStack.push(val);
                ++idx;
            } else {
                auto reduceProduction = grammar_.GetProduction(-static_cast<int32_t>(val));
                util::LOG_TRACE("[REDUCE] (%d, %s) -> %ld(%s)", stateStack.top(), tokens[idx].c_str(), val,
                                reduceProduction->ToString().c_str());
                if (reduceProduction->Number() == 1 && tokens[idx] == Grammar::EndMark) {
                    acceptSucc = true;
                    break;
                }
                if (reduceProduction->Right().front() != Grammar::NilMark) {
                    for (std::size_t i = 0UL; i < reduceProduction->Right().size(); ++i) {
                        stateStack.pop();
                    }
                }
                if (auto ret = searchActionTable(stateStack.top(), reduceProduction->Left(), val); ret != 0) {
                    util::LOG_ERROR("[GOTO] (%d, %s)", stateStack.top(), reduceProduction->Left().c_str());
                    acceptSucc = false;
                    break;
                } else {
                    util::LOG_TRACE("[GOTO] (%d, %s) -> %ld", stateStack.top(), reduceProduction->Left().c_str(), val);
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

void LRParser::OutputToGraphviz(std::ostream &os) const {
    os << "digraph G {" << std::endl;
    for (std::size_t stateNo = 0UL; stateNo < actionTable_.size(); ++stateNo) {
        for (const auto &kv : actionTable_[stateNo]) {
            if (kv.second < 0) {
                continue;
            }
            if (grammar_.IsNonTerminal(kv.first)) {
                continue;
            }
            os << "    "
               << "I" << stateNo << " -> I" << kv.second << " [label=\"" << kv.first << "\"]" << std::endl;
        }
    }
    os << "}" << std::endl;
}

void LRParser::OutputToCsv(std::ostream &os) const {
    std::deque<std::string> symbols;
    std::set<std::string> added;
    for (std::size_t stateNo = 0UL; stateNo < actionTable_.size(); ++stateNo) {
        for (const auto &kv : actionTable_[stateNo]) {
            if (added.find(kv.first) != added.end()) {
                continue;
            }
            added.insert(kv.first);
            if (grammar_.IsNonTerminal(kv.first)) {
                symbols.push_back(kv.first);
            } else {
                symbols.push_front(kv.first);
            }
        }
    }
    os << "STATE\\SYMBOL";
    for (const auto &symbol : symbols) {
        os << ", " << symbol;
    }
    os << std::endl;
    for (std::size_t stateNo = 0UL; stateNo < actionTable_.size(); ++stateNo) {
        os << stateNo;
        for (const auto &symbol : symbols) {
            auto iter = actionTable_[stateNo].find(symbol);
            if (iter == actionTable_[stateNo].end()) {
                os << ", error";
            } else {
                os << ", " << iter->second;
            }
        }
        os << std::endl;
    }
}

void LRParser::computeAndCacheLr0Items() {
    for (auto p : grammar_.AllProductions()) {
        auto head = NewLR0Item(p, 0UL);
        auto prev = head;
        lr0Items_.insert({{p, 0UL}, head});
        if (head->CanReduce()) {  // `A -> ε` only cache the first item `A -> ·ε`
            continue;
        }
        for (std::size_t i = 1UL; i <= p->Right().size(); ++i) {
            auto item = NewLR0Item(p, i, prev);
            lr0Items_.insert({{p, i}, item});
            prev = item;
        }
    }
}

void LRParser::fillActionTable(std::size_t stateNum, const std::string &symbol, std::int64_t val) {
    if (actionTable_.size() < stateNum + 1UL) {
        actionTable_.reserve(stateNum + 1UL);
        actionTable_.insert(actionTable_.end(), stateNum + 1UL - actionTable_.size(),
                            std::map<std::string, std::int64_t>());
    }
    if (actionTable_[stateNum].find(symbol) != actionTable_[stateNum].end()) {
        util::LOG_ERROR("[ERROR] Action conflict!");
        parsedSucc_ = false;
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
