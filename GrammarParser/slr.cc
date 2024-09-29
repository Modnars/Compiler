/*
 * @Author: modnarshen
 * @Date: 2024.09.27 16:17:23
 * @Note: Copyrights (c) 2024 modnarshen. All rights reserved.
 */
#include <iostream>
#include <memory>
#include <queue>
#include <sstream>
#include <utility>

#include "slr.h"

bool operator<(const slr::Item &lhs, const slr::Item &rhs) {
    return std::tie(lhs.production_, lhs.dot_pos_) < std::tie(rhs.production_, rhs.dot_pos_);
}

namespace slr {
std::string Item::ToString() const {
    std::stringstream ss;
    ss << this->production_->Left << " -> ";
    for (std::size_t i = 0UL; i <= this->production_->Right.size(); ++i) {
        if (i == this->dot_pos_) {
            ss << slr::DotMark << "";
        }
        if (i < this->production_->Right.size()) {
            ss << this->production_->Right[i] << " ";
        }
    }
    return ss.str();
}

bool Item::HasNextSymbol() const {
    return this->production_->Right.size() > this->dot_pos_;
}

const std::string &Item::NextSymbol() const {
    static const std::string empty = ""; // return empty string when HasNextSymbol is false
    if (!this->HasNextSymbol())
        return empty;
    return this->production_->Right[this->dot_pos_];
}

std::vector<std::shared_ptr<ItemSet>> ItemSet::Shift() const {
    std::map<std::string, std::shared_ptr<ItemSet>> targets;
    for (auto item : this->items_) {
        if (item->Shift()) {
            if (targets[item->NextSymbol()] == nullptr)
                targets[item->NextSymbol()] = std::make_shared<ItemSet>();
            targets[item->NextSymbol()]->Add(item->Shift());
        }
    }
    std::vector<std::shared_ptr<ItemSet>> res;
    res.reserve(targets.size());
    for (auto &&kv : targets) {
        res.emplace_back(kv.second);
    }
    return res;
}

int Parser::makeItems() {
    for (auto p : this->productions_) {
        auto head = NewItem(p, 0UL);
        auto prev = head;
        item_table_.insert({std::make_pair(p, 0UL), head});
        for (std::size_t i = 1UL; i <= p->Right.size(); ++i) {
            auto item = NewItem(p, i, prev);
            item_table_.insert({std::make_pair(p, i), item});
            prev = item;
        }
    }
    return 0;
}

int Parser::CLOSURE(std::shared_ptr<ItemSet> itemSet) {
    std::queue<std::shared_ptr<Item>> seq;
    std::set<std::shared_ptr<Item>> added;
    for (auto item : itemSet->Items()) {
        added.insert(item);
        const auto iter = this->productionIndexes_.find(item->NextSymbol());
        if (iter == this->productionIndexes_.end())
            continue;
        for (auto p : iter->second) {
            auto newItem = this->item(p, 0UL);
            if (added.find(newItem) == added.end()) {
                seq.push(newItem);
            }
        }
    }
    while (!seq.empty()) {
        auto item = seq.front();
        seq.pop();
        if (added.find(item) != added.end())
            continue;
        added.insert(item);
        itemSet->Add(item);
        const auto iter = this->productionIndexes_.find(item->NextSymbol());
        if (iter == this->productionIndexes_.end())
            continue;
        for (auto p : iter->second) {
            auto newItem = this->item(p, 0UL);
            if (added.find(newItem) == added.end()) {
                seq.push(newItem);
            }
        }
    }
    return 0;
}

int Parser::calcAllClosure() {
    auto I0 = std::make_shared<ItemSet>();
    I0->Add(this->item(this->productions_[0], 0UL));
    CLOSURE(I0);

    std::queue<std::shared_ptr<ItemSet>> seq;
    seq.push(I0);
    while (!seq.empty()) {
        auto currItemSet = seq.front();
        seq.pop();
        bool added = false;
        for (auto closure : closures_) {
            if (currItemSet->Equals(closure.second)) {
                added = true;
                break;
            }
        }
        if (added)
            continue;
        closures_.insert({closure_id_generator_++, currItemSet});
        const auto &vec = currItemSet->Shift();
        for (auto its : vec) {
            bool needAdd = true;
            CLOSURE(its);
            for (auto closure : closures_) {
                if (its->Equals(closure.second)) {
                    needAdd = false;
                    break;
                }
            }
            if (needAdd) {
                seq.push(its);
            }
        }
    }
    return 0;
}

int Parser::Parse() {
    return calcAllClosure();
}

int Parser::ShowDetails() const {
    for (const auto &kv : closures_) {
        std::cout << "I" << kv.first << ":" << std::endl;
        for (auto item : kv.second->Items()) {
            std::cout << item->ToString() << std::endl;
        }
        std::cout << std::endl;
    }
    return 0;
}
} // namespace slr
