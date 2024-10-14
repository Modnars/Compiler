/*
 * @Author: modnarshen
 * @Date: 2024.09.27 16:17:23
 * @Note: Copyrights (c) 2024 modnarshen. All rights reserved.
 */
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <queue>
#include <sstream>
#include <utility>

#include "grammar.h"
#include "slr.h"
#include "util.h"

namespace mcc {

std::shared_ptr<const ItemSet<LR0Item>> SLRParser::CLOSURE(std::shared_ptr<ItemSet<LR0Item>> itemSet) {
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
    return 0;
}

std::shared_ptr<const ItemSet<LR0Item>> SLRParser::GOTO(std::shared_ptr<const ItemSet<LR0Item>> itemSet,
                                                        const std::string &shiftSymbol) {
    auto result = std::make_shared<ItemSet<LR0Item>>();
    for (auto item : itemSet->Items()) {
        if (item->HasNextSymbol() && item->NextSymbol() == shiftSymbol) {
            result->Add(item->Shift());
        }
    }
    return CLOSURE(result);
}

void SLRParser::ShowDetails() const {
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

int SLRParser::Parse() {
    grammar_.ComputeAndCacheFirstSet();
    grammar_.ComputeAndCacheFollowSet();
    computeAndCacheLr0Items();

    auto I0 = std::make_shared<ItemSet<LR0Item>>();
    I0->Add(lr0Item(grammar_.AllProductions()[0], 0UL));
    CLOSURE(I0);
    I0->SetNumber(closureNum_++);
    closures_.insert({I0->Number(), I0});
    std::queue<std::shared_ptr<ItemSet<LR0Item>>> seq;
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
                fillActionTable(itemSet->Number(), kv.first, kv.second->Number());
                for (auto item : kv.second->Items()) {
                    if (!item->CanReduce()) {  // only process the item that can be reduced
                        continue;
                    }
                    auto production = item->GetProduction();
                    const auto &followSet = grammar_.FOLLOW(production->Left());
                    for (const auto &symbol : followSet) {
                        fillActionTable(kv.second->Number(), symbol, -production->Number());
                    }
                }
            } else {
                fillActionTable(itemSet->Number(), kv.first, iter->second->Number());
            }
        }
    }
    if (!parsedSucc_) {
        util::LOG_ERROR("[ERROR] the grammar is not an SLR grammar for there is a parsing conflict");
        return 1;
    }
    return 0;
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
        }
    }
    return targets;
}

}  // namespace mcc
