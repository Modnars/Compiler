/*
 * @Author: modnarshen
 * @Date: 2024.09.27 16:17:40
 * @Note: Copyrights (c) 2024 modnarshen. All rights reserved.
 */
#include <algorithm>
#include <cctype>
#include <memory>
#include <queue>
#include <sstream>

#include "lalr.h"
#include "lr.h"
#include "lr1.h"
#include "util.h"

namespace mcc {

int LALRParser::Parse() {
    grammar_.ComputeAndCacheFirstSet();
    computeAndCacheLr0Items();

    parsedSucc_ = true;

    auto coreI0 = std::make_shared<ItemSet<LR0Item>>();
    coreI0->Add(lr0Item(grammar_.AllProductions()[0], 0UL));
    coreI0->SetNumber(coreItemSetNum_++);
    coreItemSets_.emplace_back(coreI0);
    std::set<std::shared_ptr<ItemSet<LR0Item>>> added;
    std::queue<std::shared_ptr<ItemSet<LR0Item>>> seq;
    seq.push(coreI0);
    while (!seq.empty()) {
        auto coreItemSet = seq.front();
        seq.pop();
        if (added.count(coreItemSet)) {
            continue;
        }
        added.insert(coreItemSet);
        for (const auto &kv : computeGOTO(CLOSURE(coreItemSet))) {
            auto iter = std::find_if(coreItemSets_.begin(), coreItemSets_.end(),
                                     [&kv](auto &&closure) { return closure->Equals(kv.second); });
            if (iter == coreItemSets_.end()) {
                kv.second->SetNumber(coreItemSetNum_++);
                coreItemSets_.emplace_back(kv.second);
                seq.push(kv.second);
                addGotoState(coreItemSet->Number(), kv.second);
                parsedSucc_ &= (0 == fillActionTable(coreItemSet->Number(), kv.first, kv.second->Number()));
            } else {
                addGotoState(coreItemSet->Number(), *iter);
                parsedSucc_ &= (0 == fillActionTable(coreItemSet->Number(), kv.first, (*iter)->Number()));
            }
        }
    }

    bool isSpreading = addLookahead(0, lr0Item(grammar_.AllProductions()[0], 0UL), {Grammar::EndMark});
    while (isSpreading) {
        isSpreading = false;
        std::queue<std::shared_ptr<const ItemSet<LR0Item>>> seq;
        std::set<std::shared_ptr<const ItemSet<LR0Item>>> added;
        seq.push(coreI0);
        while (!seq.empty()) {
            auto coreItemSet = seq.front();
            seq.pop();
            if (added.count(coreItemSet)) {
                continue;
            }
            added.insert(coreItemSet);
            isSpreading |= spreadInClosure(coreItemSet);
            if (auto iter = gotoStates_.find(coreItemSet->Number()); iter != gotoStates_.end()) {
                for (auto nextState : gotoStates_[coreItemSet->Number()]) {
                    isSpreading |= spreadBetweenItemSet(coreItemSet->Number(), nextState);
                    seq.push(nextState);
                }
            }
        }
    }

    for (std::size_t stateNo = 0UL; stateNo < itemSetLookahead_.size(); ++stateNo) {
        for (const auto &kv : itemSetLookahead_[stateNo]) {
            if (!kv.first->CanReduce()) {
                continue;
            }
            for (const auto &symbol : kv.second) {
                parsedSucc_ &= (0 == fillActionTable(stateNo, symbol, -kv.first->GetProduction()->Number()));
            }
        }
    }

    return parsedSucc_ ? 0 : 1;
}

std::shared_ptr<ItemSet<LR0Item>> LALRParser::CLOSURE(std::shared_ptr<ItemSet<LR0Item>> itemSet) {
    auto result = std::make_shared<ItemSet<LR0Item>>();
    std::queue<std::shared_ptr<const LR0Item>> seq;
    std::set<std::shared_ptr<const LR0Item>> added;
    for (auto item : itemSet->Items()) {
        seq.push(item);
    }
    while (!seq.empty()) {
        auto item = seq.front();
        seq.pop();
        if (added.count(item)) {
            continue;
        }
        added.insert(item);
        result->Add(item);
        if (!item->HasNextSymbol() || !grammar_.IsNonTerminal(item->NextSymbol())) {
            continue;
        }
        for (auto p : grammar_.GetProductions(item->NextSymbol())) {
            auto newItem = lr0Item(p, 0UL);
            if (!added.count(newItem)) {
                seq.push(newItem);
            }
        }
    }
    return result;
}

void LALRParser::ShowDetails(std::ostream &os) const {
    grammar_.ShowDetails(os);

    os << "CORE ITEMSETS:\n";
    for (const auto &itemSet : coreItemSets_) {
        os << std::endl << "I" << itemSet->Number() << ":\n";
        for (auto item : itemSet->Items()) {
            std::stringstream ss;
            os << "[" << item->ToString() << ", ";
            const auto &la = lookahead(itemSet->Number(), item);
            for (auto iter = la.begin(); iter != la.end(); ++iter) {
                if (iter != la.begin()) {
                    ss << "/";
                }
                ss << *iter;
            }
            os << ss.str() << "]\n";
        }
    }
    os << std::endl;

    os << "GOTO STATES:\n";
    for (const auto &kv : gotoStates_) {
        os << "I" << kv.first << ":\n";
        for (auto itemSet : kv.second) {
            for (auto item : itemSet->Items()) {
                os << "I" << itemSet->Number() << ": " << item->ToString() << "\n";
            }
        }
        os << "\n";
    }
    os << std::endl;

    os << "LOOKAHEAD:\n";
    for (std::size_t stateNo = 0UL; stateNo < itemSetLookahead_.size(); ++stateNo) {
        os << "I" << stateNo << ":\n";
        for (const auto &kv : itemSetLookahead_[stateNo]) {
            std::stringstream ss;
            ss << "[" << kv.first->ToString() << ", ";
            for (auto iter = kv.second.begin(); iter != kv.second.end(); ++iter) {
                if (iter != kv.second.begin()) {
                    ss << "/";
                }
                ss << *iter;
            }
            ss << "]";
            os << ss.str() << "\n";
        }
        os << std::endl;
    }

    os << "ACTION TABLE:\n";
    for (std::size_t i = 0UL; i < actionTable_.size(); ++i) {
        os << i << ":\t";
        for (const auto &kv : actionTable_[i]) {
            os << kv.first << ": " << kv.second << "\t";
        }
        os << std::endl;
    }
}

std::map<std::string, std::shared_ptr<ItemSet<LR0Item>>> LALRParser::computeGOTO(
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

void LALRParser::addGotoState(std::size_t from, std::shared_ptr<const ItemSet<LR0Item>> itemSet) {
    gotoStates_[from].emplace_back(itemSet);
}

bool LALRParser::spreadInClosure(std::shared_ptr<const ItemSet<LR0Item>> coreItemSet) {
    bool isSpreading = false;
    std::queue<std::shared_ptr<const LR0Item>> seq;
    std::set<std::shared_ptr<const LR0Item>> added;
    for (auto item : coreItemSet->Items()) {
        seq.push(item);
    }
    while (!seq.empty()) {
        auto item = seq.front();
        seq.pop();
        if (added.count(item)) {
            continue;
        }
        added.insert(item);
        if (!item->HasNextSymbol() || !grammar_.IsNonTerminal(item->NextSymbol())) {
            continue;
        }
        for (auto p : grammar_.GetProductions(item->NextSymbol())) {
            auto newItem = lr0Item(p, 0UL);
            auto newLookahead = ComputeLookahead(grammar_, item, lookahead(coreItemSet->Number(), item));
            isSpreading |= updateLookahead(coreItemSet->Number(), newItem, newLookahead);
            if (added.count(newItem)) {
                continue;
            }
            seq.push(newItem);
        }
    }
    return isSpreading;
}

bool LALRParser::spreadBetweenItemSet(std::size_t from, std::shared_ptr<const ItemSet<LR0Item>> toState) {
    bool isSpreading = false;
    for (auto coreItem : toState->Items()) {
        isSpreading |= addLookahead(toState->Number(), coreItem,
                                    lookahead(from, lr0Item(coreItem->GetProduction(), coreItem->DotPos() - 1UL)));
    }
    return isSpreading;
}

}  // namespace mcc
