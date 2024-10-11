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
    for (auto iter = this->lookahead_.begin(); iter != this->lookahead_.end(); ++iter) {
        if (iter != this->lookahead_.begin()) {
            ss << "/";
        }
        ss << *iter;
    }
    return "[" + this->item_->ToString() + ", " + ss.str() + "]";
}

bool operator<(const Item &lhs, const Item &rhs) {
    return std::tie(lhs.item_, lhs.lookahead_) < std::tie(rhs.item_, rhs.lookahead_);
}

int Parser::Parse() {
    int ret = this->grammar_.ComputeFirstSet();
    if (ret != 0) {
        return ret;
    }
    makeItems();

    auto I0 = std::make_shared<lr::ItemSet>();
    I0->Add(newLrItem(this->grammar_.Productions[0], 0UL, {"#"}));
    CLOSURE(I0);
    closures_.insert({closureNum_++, I0});
    std::queue<std::shared_ptr<ItemSet>> seq;
    seq.push(I0);
    while (!seq.empty()) {
        auto itemSet = seq.front();
        seq.pop();
        for (const auto &kv : computeGOTO(itemSet)) {
            CLOSURE(kv.second);
            if (auto iter =
                    std::find_if(closures_.begin(), closures_.end(),
                                 [&kv](auto &&closure) { return closure.second->Items() == kv.second->Items(); });
                iter == closures_.end()) {
                closures_.insert({closureNum_++, kv.second});
                seq.push(kv.second);
            }
        }
    }
    return 0;
}

void Parser::ShowDetails() const {
    for (const auto &kv : this->grammar_.FirstSet) {
        std::cout << kv.first << ": ";
        for (const auto &sym : kv.second) {
            std::cout << sym << " ";
        }
        std::cout << std::endl;
    }
    for (const auto &kv : this->closures_) {
        std::cout << "\nI" << kv.first << ":" << std::endl;
        for (auto item : kv.second->Items()) {
            std::cout << item->ToString() << std::endl;
        }
    }
}

void Parser::makeItems() {
    for (auto p : this->grammar_.Productions) {
        auto head = lr0::NewItem(p, 0UL);
        auto prev = head;
        this->lr0Items_.insert({std::make_pair(p, 0UL), head});
        for (std::size_t i = 1UL; i <= p->Right.size(); ++i) {
            auto item = NewItem(p, i, prev);
            this->lr0Items_.insert({std::make_pair(p, i), item});
            prev = item;
        }
    }
}

std::set<std::string> Parser::computeLookahead(std::shared_ptr<const Item> item) const {
    if (item->CanReduce()) {
        return item->lookahead_;
    }
    if (item->HasNextSymbol() && grammar_.IsTerminal(item->NextSymbol())) {
        return item->lookahead_;
    }
    std::set<std::string> result;
    std::size_t dotPos = item->item_->DotPos() + 1UL;
    for (; dotPos < item->item_->Right().size(); ++dotPos) {
        auto symbol = item->item_->Right()[dotPos];
        if (grammar_.IsTerminal(symbol)) {
            result.insert(symbol);
            break;
        }
        if (!grammar_.FirstSet[symbol].count(Grammar::NilMark)) {
            result.insert(grammar_.FirstSet[symbol].begin(), grammar_.FirstSet[symbol].end());
            break;
        }
        result.insert(grammar_.FirstSet[symbol].begin(), grammar_.FirstSet[symbol].end());
    }
    if (dotPos >= item->item_->Right().size()) {
        result.insert(item->lookahead_.begin(), item->lookahead_.end());
    }
    return result;
}

std::shared_ptr<const ItemSet> Parser::CLOSURE(std::shared_ptr<ItemSet> itemSet) {
    std::queue<std::shared_ptr<const lr::Item>> seq;
    std::set<std::shared_ptr<const lr::Item>> added;
    for (auto item : itemSet->Items()) {
        added.insert(item);
        if (grammar_.IsTerminal(item->NextSymbol())) {
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
        if (!item->HasNextSymbol() || grammar_.IsTerminal(item->NextSymbol())) {
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

std::map<std::string, std::shared_ptr<ItemSet>> Parser::computeGOTO(std::shared_ptr<const ItemSet> itemSet) {
    std::map<std::string, std::shared_ptr<ItemSet>> result;
    for (auto item : itemSet->Items()) {
        if (item->CanReduce()) {
            continue;
        }
        if (result[item->NextSymbol()] == nullptr) {
            result[item->NextSymbol()] = std::make_shared<ItemSet>();
        }
        // result[item->NextSymbol()]->Add(newItem(item->item_->Shift(), computeLookahead(item)));
        result[item->NextSymbol()]->Add(newLrItem(item->item_->Shift(), item->lookahead_));
    }
    return result;
}

std::shared_ptr<const ItemSet> Parser::GOTO(std::shared_ptr<const ItemSet> itemSet, const std::string &shiftSymbol) {
    auto result = std::make_shared<ItemSet>();
    for (auto item : itemSet->Items()) {
        if (item->HasNextSymbol() && item->NextSymbol() == shiftSymbol) {
            result->Add(newLrItem(item->item_->Shift(), item->lookahead_));
        }
    }
    return CLOSURE(result);
}

}  // namespace lr
