/*
 * @Author: modnarshen
 * @Date: 2024.10.15 15:57:49
 * @Note: Copyrights (c) 2024 modnarshen. All rights reserved.
 */
#include <stack>

#include "grammar.h"
#include "ll1.h"
#include "util.h"

namespace mcc {

int LL1Parser::Parse() {
    grammar_.ComputeAndCacheFirstSet();
    grammar_.ComputeAndCacheFollowSet();

    for (auto p : grammar_.AllProductions()) {
        auto firsetSet = grammar_.ComputeFirstSet(p->Right().begin(), p->Right().end());
        for (const auto &terminal : firsetSet) {
            if (terminal == Grammar::NilMark) {
                continue;
            }
            if (auto ret = fillPredictionTable(p->Left(), terminal, p); ret != 0) {
                util::LOG_ERROR("[ERROR] conflict!");
                parsedSucc_ = false;
            }
        }
        if (firsetSet.find(Grammar::NilMark) != firsetSet.end()) {
            for (const auto &terminal : grammar_.FOLLOW(p->Left())) {
                if (auto ret = fillPredictionTable(p->Left(), terminal, p); ret != 0) {
                    util::LOG_ERROR("[ERROR] conflict!");
                    parsedSucc_ = false;
                }
            }
        }
    }

    return 0;
}

int LL1Parser::Analyze(std::istream &is) const {
    if (!parsedSucc_) {
        util::LOG_WARN("[WARNING] grammar parsed failed, so the analyzing result is not valuable");
    }
    std::string line;
    while (std::getline(is, line)) {
        auto tokens = util::Split(util::Trim(line), " ");
        tokens.emplace_back(Grammar::EndMark);

        std::stack<std::string> symbolStack;
        symbolStack.push(Grammar::EndMark);
        symbolStack.push(grammar_.GetProduction(1)->Left());
        std::size_t idx = 0UL;
        bool analyzedSucc = true;
        auto X = symbolStack.top();
        while (X != Grammar::EndMark) {
            if (X == tokens[idx]) {
                symbolStack.pop();
                ++idx;
            } else if (!grammar_.IsNonTerminal(X)) {
                util::LOG_ERROR("[ERROR] terminal symbol");
                analyzedSucc = false;
                break;
            } else {
                auto production = searchPredictionTable(X, tokens[idx]);
                if (production == nullptr) {
                    util::LOG_ERROR("[ERROR] search production failed");
                    analyzedSucc = false;
                    break;
                } else {
                    util::LOG_TRACE("[PREDICT] %s", production->ToString().c_str());
                    symbolStack.pop();
                    const auto &right = production->Right();
                    if (right.front() != Grammar::NilMark) {
                        for (auto riter = right.rbegin(); riter != right.rend(); ++riter) {
                            symbolStack.push(*riter);
                        }
                    }
                }
            }
            X = symbolStack.top();
        }
        if (analyzedSucc) {
            util::LOG_INFO("ACCEPT");
        } else {
            util::LOG_ERROR("ANALYZE FAILED");
        }
    }
    return 0;
}

void LL1Parser::ShowDetails(std::ostream &os) const {
    grammar_.ShowDetails();

    os << "\nPREDICTION TABLE:" << std::endl;
    for (const auto &kv : predictionTable_) {
        os << kv.first << ":\t";
        for (const auto &kkv : kv.second) {
            os << kkv.first << ": " << kkv.second->ToString() << "\t";
        }
        os << std::endl;
    }
}

}  // namespace mcc
