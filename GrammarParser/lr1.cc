/*
 * @Author: modnarshen
 * @Date: 2024.10.13 15:46:30
 * @Note: Copyrights (c) 2024 modnarshen. All rights reserved.
 */
#include <algorithm>
#include <iostream>
#include <iterator>
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
    I0 = CLOSURE(I0);
    I0->SetNumber(closureNum_++);
    closures_.emplace_back(I0);
    std::queue<std::shared_ptr<ItemSet<LR1Item>>> seq;
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

void LR1Parser::ShowDetails(std::ostream &os) const {
    grammar_.ShowDetails(os);

    os << std::endl << "STATE CLOSURE:" << std::endl;
    for (const auto &closure : closures_) {
        os << "\nI" << closure->Number() << ":" << std::endl;
        for (auto item : closure->Items()) {
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
    return ComputeLookahead(grammar_, item, item->lookahead_);
}

std::shared_ptr<ItemSet<LR1Item>> LR1Parser::CLOSURE(std::shared_ptr<ItemSet<LR1Item>> itemSet) {
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

std::shared_ptr<ItemSet<LR1Item>> LR1Parser::GOTO(std::shared_ptr<const ItemSet<LR1Item>> itemSet,
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
            auto production = item->lr0Item_->GetProduction();
            for (const auto &symbol : item->lookahead_) {
                parsedSucc_ &= (0 == fillActionTable(itemSet->Number(), symbol, -production->Number()));
            }
            continue;
        }
    }
    return result;
}

}  // namespace mcc
