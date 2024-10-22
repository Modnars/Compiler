/*
 * @Author: modnarshen
 * @Date: 2024.09.27 16:17:23
 * @Note: Copyrights (c) 2024 modnarshen. All rights reserved.
 */
#include <algorithm>
#include <iostream>
#include <memory>
#include <queue>

#include "slr.h"

namespace mcc {

std::shared_ptr<ItemSet<LR0Item>> SLRParser::CLOSURE(std::shared_ptr<ItemSet<LR0Item>> itemSet) {
    std::queue<std::shared_ptr<const LR0Item>> seq;
    std::set<std::shared_ptr<const LR0Item>> added;
    for (auto item : itemSet->Items()) {
        added.insert(item);
        for (auto p : grammar_.GetProductions(item->NextSymbol())) {
            auto newItem = lr0Item(p, 0UL);
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
        for (auto p : grammar_.GetProductions(item->NextSymbol())) {
            auto newItem = lr0Item(p, 0UL);
            if (added.find(newItem) == added.end()) {
                seq.push(newItem);
            }
        }
    }
    return itemSet;
}

std::shared_ptr<ItemSet<LR0Item>> SLRParser::GOTO(std::shared_ptr<const ItemSet<LR0Item>> itemSet,
                                                  const std::string &shiftSymbol) {
    auto result = std::make_shared<ItemSet<LR0Item>>();
    for (auto item : itemSet->Items()) {
        if (item->HasNextSymbol() && item->NextSymbol() == shiftSymbol) {
            result->Add(item->Shift());
        }
    }
    return CLOSURE(result);
}

void SLRParser::ShowDetails(std::ostream &os) const {
    grammar_.ShowDetails(os);

    std::cout << std::endl << "STATE CLOSURE:" << std::endl;
    for (std::size_t i = 0UL; i < closures_.size(); ++i) {
        std::cout << "\nI" << i << ":" << std::endl;
        for (auto item : closures_[i]->Items()) {
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

int SLRParser::Parse() {
    grammar_.ComputeAndCacheFirstSet();
    grammar_.ComputeAndCacheFollowSet();
    computeAndCacheLr0Items();

    parsedSucc_ = true;
    auto I0 = std::make_shared<ItemSet<LR0Item>>();
    I0->Add(lr0Item(grammar_.AllProductions()[0], 0UL));
    I0 = CLOSURE(I0);
    I0->SetNumber(closureNum_++);
    closures_.push_back(I0);
    std::queue<std::shared_ptr<ItemSet<LR0Item>>> seq;
    seq.push(I0);
    while (!seq.empty()) {
        auto itemSet = seq.front();
        seq.pop();
        for (const auto &kv : computeGOTO(itemSet)) {
            auto newClosure = CLOSURE(kv.second);
            auto iter = std::find_if(closures_.begin(), closures_.end(),
                                     [newClosure](auto &&closure) { return closure->Equals(newClosure); });
            if (iter == closures_.end()) {
                newClosure->SetNumber(closureNum_++);
                closures_.emplace_back(newClosure);
                seq.push(newClosure);
                parsedSucc_ &= (0 == fillActionTable(itemSet->Number(), kv.first, newClosure->Number()));
            } else {
                parsedSucc_ &= (0 == fillActionTable(itemSet->Number(), kv.first, (*iter)->Number()));
            }
        }
    }
    return parsedSucc_ ? 0 : 1;
}

std::map<std::string, std::shared_ptr<ItemSet<LR0Item>>> SLRParser::computeGOTO(
    std::shared_ptr<const ItemSet<LR0Item>> itemSet) {
    std::map<std::string, std::shared_ptr<ItemSet<LR0Item>>> targets;
    for (auto item : itemSet->Items()) {
        if (item->HasNextSymbol()) {
            if (targets[item->NextSymbol()] == nullptr) {
                targets[item->NextSymbol()] = std::make_shared<ItemSet<LR0Item>>();
            }
            targets[item->NextSymbol()]->Add(item->Shift());
        } else if (item->CanReduce()) {  // actually `item->CanReduce()` always equals to `!item->HasNextSymbol()`
            auto production = item->GetProduction();
            const auto &followSet = grammar_.FOLLOW(production->Left());
            for (const auto &symbol : followSet) {
                parsedSucc_ &= (0 == fillActionTable(itemSet->Number(), symbol, -production->Number()));
            }
        }
    }
    return targets;
}

}  // namespace mcc
