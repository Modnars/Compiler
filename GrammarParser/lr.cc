/*
 * @Author: modnarshen
 * @Date: 2024.09.25 11:00:50
 * @Note: Copyrights (c) 2024 modnarshen. All rights reserved.
 */
#include <algorithm>
#include <iostream>
#include <queue>
#include <sstream>

#include "grammar.h"
#include "lr.h"

namespace lr {

std::string Item::ToString() const {
    std::stringstream ss;
    for (auto iter = lookahead_.begin(); iter != lookahead_.end(); ++iter) {
        if (iter != lookahead_.begin()) {
            ss << "/";
        }
        ss << *iter;
    }
    return "[" + lr0Item_->ToString() + ", " + ss.str() + "]";
}

bool operator<(const Item &lhs, const Item &rhs) {
    return std::tie(lhs.lr0Item_, lhs.lookahead_) < std::tie(rhs.lr0Item_, rhs.lookahead_);
}

int Parser::Parse() {
    grammar_.ComputeAndCacheFirstSet();
    grammar_.ComputeAndCacheFollowSet();
    computeAndCacheItems();

    auto I0 = std::make_shared<lr::ItemSet>();
    I0->Add(newLrItem(grammar_.AllProductions()[0], 0UL, {Grammar::EndMark}));
    CLOSURE(I0);
    I0->SetNumber(closureNum_++);
    closures_.insert({I0->Number(), I0});
    std::queue<std::shared_ptr<ItemSet>> seq;
    seq.push(I0);
    while (!seq.empty()) {
        auto itemSet = seq.front();
        seq.pop();
        for (const auto &kv : computeGOTO(itemSet)) {
            CLOSURE(kv.second);
            auto iter = std::find_if(closures_.begin(), closures_.end(),
                                     [&kv](auto &&closure) { return closure.second->Items() == kv.second->Items(); });
            if (iter == closures_.end()) {
                kv.second->SetNumber(closureNum_++);
                closures_.insert({kv.second->Number(), kv.second});
                seq.push(kv.second);
                fillActionTable(itemSet->Number(), kv.first, kv.second->Number());
            } else {
                fillActionTable(itemSet->Number(), kv.first, iter->second->Number());
            }
        }
    }
    return 0;
}

void Parser::ShowDetails() const {
    grammar_.ShowDetails();

    std::cout << std::endl << "STATE CLOSURE:" << std::endl;
    for (const auto &kv : closures_) {
        std::cout << "\nI" << kv.first << ":" << std::endl;
        for (auto item : kv.second->Items()) {
            std::cout << item->ToString() << std::endl;
        }
    }
    std::cout << std::endl << "ACTION TABLE:" << std::endl;
    for (std::size_t i = 0UL; i < actionTable_.size(); ++i) {
        std::cout << i << ":\t";
        for (const auto &kv : actionTable_[i]) {
            std::cout << kv.first << ": " << kv.second << "\t";
        }
        std::cout << std::endl;
    }
}

void Parser::computeAndCacheItems() {
    for (auto p : grammar_.AllProductions()) {
        auto head = lr0::NewItem(p, 0UL);
        auto prev = head;
        lr0Items_.insert({{p, 0UL}, head});
        if (head->CanReduce()) {  // `A -> ε` only cache the first item `A -> ·ε`
            continue;
        }
        for (std::size_t i = 1UL; i <= p->Right().size(); ++i) {
            auto item = lr0::NewItem(p, i, prev);
            lr0Items_.insert({{p, i}, item});
            prev = item;
        }
    }
}

std::set<std::string> Parser::computeLookahead(std::shared_ptr<const Item> item) const {
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

std::shared_ptr<const ItemSet> Parser::CLOSURE(std::shared_ptr<ItemSet> itemSet) {
    std::queue<std::shared_ptr<const lr::Item>> seq;
    std::set<std::shared_ptr<const lr::Item>> added;
    for (auto item : itemSet->Items()) {
        added.insert(item);
        if (item->HasNextSymbol() && !grammar_.IsNonTerminal(item->NextSymbol())) {
            continue;
        }
        for (auto production : grammar_.GetProductions(item->NextSymbol())) {
            auto newItem = newLrItem(production, 0UL, computeLookahead(item));
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
            auto newItem = newLrItem(p, 0UL, computeLookahead(item));
            if (added.find(newItem) == added.end()) {
                seq.push(newItem);
            }
        }
    }
    return itemSet;
}

std::shared_ptr<const ItemSet> Parser::GOTO(std::shared_ptr<const ItemSet> itemSet, const std::string &shiftSymbol) {
    auto result = std::make_shared<ItemSet>();
    for (auto item : itemSet->Items()) {
        if (item->HasNextSymbol() && item->NextSymbol() == shiftSymbol) {
            result->Add(newLrItem(item->lr0Item_->Shift(), item->lookahead_));
        }
    }
    return CLOSURE(result);
}

std::map<std::string, std::shared_ptr<ItemSet>> Parser::computeGOTO(std::shared_ptr<const ItemSet> itemSet) {
    std::map<std::string, std::shared_ptr<ItemSet>> result;
    for (auto item : itemSet->Items()) {
        if (item->CanReduce()) {
            for (const auto &symbol : item->lookahead_) {
                fillActionTable(itemSet->Number(), symbol, -item->lr0Item_->GetProduction()->Number());
            }
            continue;
        }
        if (result[item->NextSymbol()] == nullptr) {
            result[item->NextSymbol()] = std::make_shared<ItemSet>();
        }
        result[item->NextSymbol()]->Add(newLrItem(item->lr0Item_->Shift(), item->lookahead_));
    }
    return result;
}

void Parser::fillActionTable(std::size_t stateNum, const std::string &symbol, std::int64_t val) {
    if (actionTable_.size() < stateNum + 1UL) {
        actionTable_.reserve(stateNum + 1UL);
        actionTable_.insert(actionTable_.end(), stateNum + 1UL - actionTable_.size(),
                            std::map<std::string, std::int64_t>());
    }
    if (actionTable_[stateNum].find(symbol) != actionTable_[stateNum].end()) {
        std::cerr << Color::RED << "conflict!" << Color::RESET << std::endl;
        return;
    }
    actionTable_[stateNum][symbol] = val;
}

}  // namespace lr
