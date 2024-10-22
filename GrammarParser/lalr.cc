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

std::string LALRItem::ToString() const {
    std::stringstream ss;
    for (auto iter = lookahead_.begin(); iter != lookahead_.end(); ++iter) {
        if (iter != lookahead_.begin()) {
            ss << "/";
        }
        ss << *iter;
    }
    return "[" + lr0Item_->ToString() + ", " + ss.str() + "]";
}

int LALRParser::Parse() {
    grammar_.ComputeAndCacheFirstSet();
    computeAndCacheLr0Items();

    parsedSucc_ = true;

    auto coreI0 = std::make_shared<ItemSet<LR0Item>>();
    coreI0->Add(lr0Item(grammar_.AllProductions()[0], 0UL));
    coreI0->SetNumber(closureNum_++);
    coreClosures_.emplace_back(coreI0);
    std::set<std::shared_ptr<const ItemSet<LR0Item>>> added;
    std::queue<std::shared_ptr<const ItemSet<LR0Item>>> seq;
    seq.push(coreI0);
    while (!seq.empty()) {
        auto coreItemSet = seq.front();
        seq.pop();
        if (added.count(coreItemSet)) {
            continue;
        }
        added.insert(coreItemSet);
        for (const auto &kv : computeGOTO(CLOSURE(coreItemSet))) {
            auto iter = std::find_if(coreClosures_.begin(), coreClosures_.end(),
                                     [&kv](auto &&closure) { return closure->Equals(kv.second); });
            if (iter == coreClosures_.end()) {
                kv.second->SetNumber(closureNum_++);
                coreClosures_.emplace_back(kv.second);
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
    std::queue<std::shared_ptr<const ItemSet<LR0Item>>> sequence;
    std::set<std::shared_ptr<const ItemSet<LR0Item>>> haded;
    sequence.push(coreI0);
    while (!sequence.empty()) {
        auto coreItemSet = sequence.front();
        sequence.pop();
        if (haded.count(coreItemSet)) {
            continue;
        }
        haded.insert(coreItemSet);
        isSpreading |= spreadInClosure(coreItemSet);
        for (auto nextState : gotoStates_[coreItemSet->Number()]) {
            isSpreading |= spreadFromState(coreItemSet->Number(), nextState);
            sequence.push(nextState);
        }
    }

    // for (std::size_t stateNo = 0UL; stateNo < coreClosures_.size(); ++stateNo) {
    //     for (auto coreItem : coreClosures_[stateNo]->Items()) {
    //         if (!coreItem->CanReduce()) {
    //             continue;
    //         }
    //         for (const auto &symbol : lookahead(stateNo, coreItem)) {
    //             parsedSucc_ &= (0 == fillActionTable(stateNo, symbol, -coreItem->GetProduction()->Number()));
    //         }
    //     }
    // }

    return parsedSucc_ ? 0 : 1;
}

std::shared_ptr<const ItemSet<LR0Item>> LALRParser::CLOSURE(std::shared_ptr<const ItemSet<LR0Item>> itemSet) {
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
    for (std::size_t i = 0UL; i < coreClosures_.size(); ++i) {
        std::cout << std::endl << "I" << i << ":\n";
        for (auto item : coreClosures_[i]->Items()) {
            std::stringstream ss;
            os << "[" << item->ToString() << ", ";
            const auto &la = lookahead(i, item);
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

    std::cout << "GOTO STATES:\n";
    for (std::size_t stateNo = 0UL; stateNo < gotoStates_.size(); ++stateNo) {
        if (gotoStates_[stateNo].empty()) {
            continue;
        }
        std::cout << "I" << stateNo << ":\n";
        for (const auto &toState : gotoStates_[stateNo]) {
            for (auto item : toState->Items()) {
                std::cout << "I" << toState->Number() << ": " << item->ToString() << "\n";
            }
        }
        std::cout << std::endl;
    }

    std::cout << "LOOKAHEAD:\n";
    for (std::size_t stateNo = 0UL; stateNo < itemSetLookahead_.size(); ++stateNo) {
        std::cout << "I" << stateNo << ":\n";
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
            std::cout << ss.str() << "\n";
        }
        std::cout << std::endl;
    }

    std::cout << "ACTION TABLE:\n";
    for (std::size_t i = 0UL; i < actionTable_.size(); ++i) {
        std::cout << i << ":\t";
        for (const auto &kv : actionTable_[i]) {
            std::cout << kv.first << ": " << kv.second << "\t";
        }
        std::cout << std::endl;
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
    if (gotoStates_.size() < from + 1UL) {
        gotoStates_.reserve(from + 1UL);
        gotoStates_.insert(gotoStates_.end(), from + 1UL - gotoStates_.size(),
                           std::vector<std::shared_ptr<const ItemSet<LR0Item>>>());
    }
    gotoStates_[from].emplace_back(itemSet);
}

bool LALRParser::spreadInClosure(std::shared_ptr<const ItemSet<LR0Item>> coreItemSet) {
    std::cout << "spread " << coreItemSet->Number() << std::endl;
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

bool LALRParser::spreadFromState(std::size_t from, std::shared_ptr<const ItemSet<LR0Item>> toState) {
    bool isSpreading = false;
    for (auto coreItem : toState->Items()) {
        isSpreading |= addLookahead(toState->Number(), coreItem,
                                    lookahead(from, lr0Item(coreItem->GetProduction(), coreItem->DotPos() - 1UL)));
    }
    return isSpreading;
}

}  // namespace mcc
