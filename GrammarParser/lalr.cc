/*
 * @Author: modnarshen
 * @Date: 2024.09.27 16:17:40
 * @Note: Copyrights (c) 2024 modnarshen. All rights reserved.
 */
#include <algorithm>
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
    auto I0 = std::make_shared<ItemSet<LALRItem>>();
    I0->Add(lalrItem(lr0Item(grammar_.AllProductions()[0], 0UL), {}));
    CLOSURE(I0);
    I0->SetNumber(closureNum_++);
    closures_.emplace_back(I0);
    std::queue<std::shared_ptr<ItemSet<LALRItem>>> seq;
    seq.push(I0);
    while (!seq.empty()) {
        auto itemSet = seq.front();
        seq.pop();
        for (const auto &kv : computeGOTO(itemSet)) {
            CLOSURE(kv.second);
            auto iter = std::find_if(closures_.begin(), closures_.end(),
                                     [&kv](auto &&closure) { return closure->Equals(kv.second); });
            if (iter == closures_.end()) {
                kv.second->SetNumber(closureNum_++);
                closures_.emplace_back(kv.second);
                seq.push(kv.second);
                // parsedSucc_ &= (0 == fillActionTable(itemSet->Number(), kv.first, kv.second->Number()));
            } else {
                // parsedSucc_ &= (0 == fillActionTable(itemSet->Number(), kv.first, (*iter)->Number()));
            }
        }
    }
    bool isSpreading = spreadLookahead(lalrItem(lr0Item(grammar_.AllProductions()[0], 0UL), {}), {Grammar::EndMark});
    std::cout << isSpreading << std::endl;
    return parsedSucc_ ? 0 : 1;
}

std::shared_ptr<ItemSet<LALRItem>> LALRParser::CLOSURE(std::shared_ptr<ItemSet<LALRItem>> itemSet) {
    std::queue<std::shared_ptr<const LALRItem>> seq;
    std::set<std::shared_ptr<const LALRItem>> added;
    for (auto item : itemSet->Items()) {
        added.insert(item);
        if (item->HasNextSymbol() && !grammar_.IsNonTerminal(item->NextSymbol())) {
            continue;
        }
        for (auto p : grammar_.GetProductions(item->NextSymbol())) {
            auto newItem = lalrItem(lr0Item(p, 0UL), {});
            if (!added.count(newItem)) {
                seq.push(newItem);
            }
        }
    }
    while (!seq.empty()) {
        auto item = seq.front();
        seq.pop();
        if (added.count(item)) {
            continue;
        }
        added.insert(item);
        itemSet->Add(item);
        if (item->HasNextSymbol() && !grammar_.IsNonTerminal(item->NextSymbol())) {
            continue;
        }
        for (auto p : grammar_.GetProductions(item->NextSymbol())) {
            auto newItem = lalrItem(lr0Item(p, 0UL), {});
            if (!added.count(newItem)) {
                seq.push(newItem);
            }
        }
    }
    return 0;
}

void LALRParser::ShowDetails(std::ostream &os) const {
    grammar_.ShowDetails(os);

    std::cout << std::endl << "STATE CLOSURE:\n";
    for (std::size_t i = 0UL; i < closures_.size(); ++i) {
        std::cout << std::endl << "I" << i << ":\n";
        for (auto item : closures_[i]->Items()) {
            std::cout << item->ToString() << "\n";
        }
    }
    std::cout << std::endl;
    std::cout << "ACTION TABLE:\n";
    for (std::size_t i = 0UL; i < actionTable_.size(); ++i) {
        std::cout << i << ":\t";
        for (const auto &kv : actionTable_[i]) {
            std::cout << kv.first << ": " << kv.second << "\t";
        }
        std::cout << std::endl;
    }
}

std::map<std::string, std::shared_ptr<ItemSet<LALRItem>>> LALRParser::computeGOTO(
    std::shared_ptr<const ItemSet<LALRItem>> itemSet) {
    std::map<std::string, std::shared_ptr<ItemSet<LALRItem>>> targets;
    for (auto item : itemSet->Items()) {
        if (item->HasNextSymbol()) {
            if (targets[item->NextSymbol()] == nullptr) {
                targets[item->NextSymbol()] = std::make_shared<ItemSet<LALRItem>>();
            }
            targets[item->NextSymbol()]->Add(lalrItem(item->lr0Item_->Shift(), item->lookahead_));
        } else if (item->CanReduce()) {  // actually `item->CanReduce()` always equals to `!item->HasNextSymbol()`
            auto production = item->lr0Item_->GetProduction();
            for (const auto &symbol : item->lookahead_) {
                parsedSucc_ &= (0 == fillActionTable(itemSet->Number(), symbol, -production->Number()));
            }
        }
    }
    return targets;
}

}  // namespace mcc
