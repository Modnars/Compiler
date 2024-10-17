/*
 * @Author: modnarshen
 * @Date: 2024.10.13 15:46:30
 * @Note: Copyrights (c) 2024 modnarshen. All rights reserved.
 */
#include <algorithm>
#include <iostream>
#include <queue>
#include <sstream>

#include "lr1.h"

namespace mcc {

std::string LR1Item::ToString() const {
    std::stringstream ss;
    for (auto iter = lookahead_.begin(); iter != lookahead_.end(); ++iter) {
        if (iter != lookahead_.begin()) {
            ss << "/";
        }
        ss << *iter;
    }
    return "[" + lr0Item_->ToString() + ", " + ss.str() + "]";
}

int LR1Parser::Parse() {
    grammar_.ComputeAndCacheFirstSet();
    computeAndCacheLr0Items();

    parsedSucc_ = true;
    auto I0 = std::make_shared<ItemSet<LR1Item>>();
    I0->Add(newLr1Item(lr0Item(grammar_.AllProductions()[0], 0UL), {Grammar::EndMark}));
    CLOSURE(I0);
    I0->SetNumber(closureNum_++);
    closures_.insert({I0->Number(), I0});
    std::queue<std::shared_ptr<ItemSet<LR1Item>>> seq;
    seq.push(I0);
    while (!seq.empty()) {
        auto itemSet = seq.front();
        seq.pop();
        for (const auto &kv : computeGOTO(itemSet)) {
            CLOSURE(kv.second);
            auto iter = std::find_if(closures_.begin(), closures_.end(),
                                     [&kv](auto &&closure) { return closure.second->Equals(kv.second); });
            if (iter == closures_.end()) {
                kv.second->SetNumber(closureNum_++);
                closures_.insert({kv.second->Number(), kv.second});
                seq.push(kv.second);
                parsedSucc_ &= (0 == fillActionTable(itemSet->Number(), kv.first, kv.second->Number()));
            } else {
                parsedSucc_ &= (0 == fillActionTable(itemSet->Number(), kv.first, iter->second->Number()));
            }
        }
    }
    return parsedSucc_ ? 0 : 1;
}

void LR1Parser::ShowDetails(std::ostream &os) const {
    grammar_.ShowDetails(os);

    os << std::endl << "STATE CLOSURE:" << std::endl;
    for (const auto &kv : closures_) {
        os << "\nI" << kv.first << ":" << std::endl;
        for (auto item : kv.second->Items()) {
            os << item->ToString() << std::endl;
        }
    }
    os << std::endl << "ACTION TABLE:" << std::endl;
    for (std::size_t i = 0UL; i < actionTable_.size(); ++i) {
        os << i << ":\t";
        for (const auto &kv : actionTable_[i]) {
            os << kv.first << ": " << kv.second << "\t";
        }
        os << std::endl;
    }
}

std::set<std::string> LR1Parser::computeLookahead(std::shared_ptr<const LR1Item> item) const {
    if (item->CanReduce()) {
        return item->lookahead_;
    }
    if (item->HasNextSymbol() && !grammar_.IsNonTerminal(item->NextSymbol())) {
        return item->lookahead_;
    }
    std::set<std::string> result;
    std::size_t dotPos = item->lr0Item_->DotPos() + 1UL;
    for (; dotPos < item->lr0Item_->Right().size(); ++dotPos) {
        auto symbol = item->lr0Item_->Right()[dotPos];
        if (!grammar_.IsNonTerminal(symbol)) {
            result.insert(symbol);
            break;
        }
        const auto &firstSet = grammar_.FIRST(symbol);
        if (firstSet.find(Grammar::NilMark) == firstSet.end()) {
            result.insert(firstSet.begin(), firstSet.end());
            break;
        }
        result.insert(firstSet.begin(), firstSet.end());
    }
    if (dotPos >= item->lr0Item_->Right().size()) {
        result.erase(Grammar::NilMark);
        // Compute the FIRST(β lookahead). If FIRST(β) contains ε, then FIRST(β lookahead) contains FIRST(lookahead).
        // Since lookahead is a terminal symbol (`lookahead_` should also be a set of terminal symbols), we can insert
        // lookahead (`lookahead_`) directly.
        result.insert(item->lookahead_.begin(), item->lookahead_.end());
    }
    return result;
}

std::shared_ptr<const ItemSet<LR1Item>> LR1Parser::CLOSURE(std::shared_ptr<ItemSet<LR1Item>> itemSet) {
    std::queue<std::shared_ptr<const LR1Item>> seq;
    std::set<std::shared_ptr<const LR1Item>> added;
    for (auto item : itemSet->Items()) {
        added.insert(item);
        if (item->HasNextSymbol() && !grammar_.IsNonTerminal(item->NextSymbol())) {
            continue;
        }
        for (auto production : grammar_.GetProductions(item->NextSymbol())) {
            auto newItem = newLr1Item(lr0Item(production, 0UL), computeLookahead(item));
            if (added.find(newItem) == added.end()) {
                seq.push(newItem);
            }
        }
    }
    while (!seq.empty()) {
        auto item = seq.front();
        seq.pop();
        if (added.find(item) != added.end()) {
            continue;
        }
        added.insert(item);
        itemSet->Add(item);
        if (!item->HasNextSymbol() || !grammar_.IsNonTerminal(item->NextSymbol())) {
            continue;
        }
        for (auto p : grammar_.GetProductions(item->NextSymbol())) {
            auto newItem = newLr1Item(lr0Item(p, 0UL), computeLookahead(item));
            if (added.find(newItem) == added.end()) {
                seq.push(newItem);
            }
        }
    }
    return itemSet;
}

std::shared_ptr<const ItemSet<LR1Item>> LR1Parser::GOTO(std::shared_ptr<const ItemSet<LR1Item>> itemSet,
                                                        const std::string &shiftSymbol) {
    auto result = std::make_shared<ItemSet<LR1Item>>();
    for (auto item : itemSet->Items()) {
        if (item->HasNextSymbol() && item->NextSymbol() == shiftSymbol) {
            result->Add(newLr1Item(item->lr0Item_->Shift(), item->lookahead_));
        }
    }
    return CLOSURE(result);
}

std::map<std::string, std::shared_ptr<ItemSet<LR1Item>>> LR1Parser::computeGOTO(
    std::shared_ptr<const ItemSet<LR1Item>> itemSet) {
    std::map<std::string, std::shared_ptr<ItemSet<LR1Item>>> result;
    for (auto item : itemSet->Items()) {
        if (item->HasNextSymbol()) {
            if (result[item->NextSymbol()] == nullptr) {
                result[item->NextSymbol()] = std::make_shared<ItemSet<LR1Item>>();
            }
            result[item->NextSymbol()]->Add(newLr1Item(item->lr0Item_->Shift(), item->lookahead_));
        } else if (item->CanReduce()) {
            for (const auto &symbol : item->lookahead_) {
                parsedSucc_ &=
                    (0 == fillActionTable(itemSet->Number(), symbol, -item->lr0Item_->GetProduction()->Number()));
            }
            continue;
        }
    }
    return result;
}

}  // namespace mcc