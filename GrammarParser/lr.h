/*
 * @Author: modnarshen
 * @Date: 2024.09.25 11:00:40
 * @Note: Copyrights (c) 2024 modnarshen. All rights reserved.
 */
#pragma once

#include <cstddef>
#include <map>
#include <memory>
#include <set>
#include <vector>

#include "grammar.h"
#include "lr0.h"
#include "parser.h"

namespace lr {
static const std::string DotMark = "·";
class Item;
class Parser;

class Item {
    friend bool operator<(const Item &lhs, const Item &rhs);
    friend class Parser;

public:
    Item(std::shared_ptr<const lr0::Item> item, const std::set<std::string> &lookahead)
        : item_(item), lookahead_(lookahead) { }

    std::string ToString() const;

public:
    bool HasNextSymbol() const { return this->item_->HasNextSymbol(); }

    const std::string &NextSymbol() const { return this->item_->NextSymbol(); }

    bool CanReduce() const { return this->item_->CanReduce(); }

    std::size_t DotPos() const { return this->item_->DotPos(); }

    const std::vector<std::string> &Right() const { return this->item_->Right(); }

private:
    std::shared_ptr<const lr0::Item> item_ = nullptr;
    std::set<std::string> lookahead_;
};

bool operator<(const Item &lhs, const Item &rhs);

class ItemSet {
public:
    bool Add(std::shared_ptr<const Item> item) { return this->items_.insert(item).second; }
    bool Contains(std::shared_ptr<const Item> item) const { return this->items_.find(item) != this->items_.end(); }
    bool Equals(std::shared_ptr<const ItemSet> rhs) const { return this->items_ == rhs->items_; };

public:
    const std::set<std::shared_ptr<const Item>> &Items() const { return this->items_; };
    std::size_t Number() const { return this->number; }
    void SetNumber(std::size_t number) { this->number = number; }

private:
    std::set<std::shared_ptr<const Item>> items_;
    std::size_t number = 0UL;
};

class Parser {
public:
    Parser(Grammar &grammar) : grammar_(grammar) { }

public:
    int Parse();

    void ShowDetails() const;

    std::shared_ptr<const ItemSet> CLOSURE(std::shared_ptr<ItemSet> itemSet);

    std::shared_ptr<const ItemSet> GOTO(std::shared_ptr<const ItemSet> itemSet, const std::string &shiftSymbol);

private:
    void makeItems();

    std::shared_ptr<const lr0::Item> lr0Item(std::shared_ptr<Production> p, std::size_t pos) const {
        const auto iter = lr0Items_.find(std::make_pair(p, pos));
        if (iter == lr0Items_.end()) {
            return nullptr;
        }
        return iter->second;
    }

    std::shared_ptr<const Item> lr1Item(std::shared_ptr<const lr0::Item> item,
                                        const std::set<std::string> &lookahead) const {
        const auto iter = items_.find(std::make_pair(item, lookahead));
        if (iter == items_.end()) {
            return nullptr;
        }
        return iter->second;
    }

    std::shared_ptr<const Item> newLrItem(std::shared_ptr<const lr0::Item> lr0It,
                                          const std::set<std::string> &lookahead) {
        auto newItem = lr1Item(lr0It, lookahead);
        if (newItem != nullptr) {
            return newItem;
        }
        newItem = std::make_shared<const Item>(lr0It, lookahead);
        this->items_.insert({{lr0It, lookahead}, newItem});
        return newItem;
    }

    std::shared_ptr<const Item> newLrItem(std::shared_ptr<Production> p, std::size_t pos,
                                          const std::set<std::string> &lookahead) {
        return newLrItem(lr0Item(p, pos), lookahead);
    }

    std::set<std::string> computeLookahead(std::shared_ptr<const Item> item) const;

    std::map<std::string, std::shared_ptr<ItemSet>> computeGOTO(std::shared_ptr<const ItemSet> itemSet);

    void fillActionTable(std::size_t stateNum, const std::string &symbol, std::int64_t val);

    int queryActionTable(std::size_t stateNum, const std::string &symbol, std::int64_t &val) const;

private:
    Grammar &grammar_;
    std::map<std::pair<std::shared_ptr<Production>, std::size_t>, std::shared_ptr<const lr0::Item>> lr0Items_;
    std::map<std::pair<std::shared_ptr<const lr0::Item>, std::set<std::string>>, std::shared_ptr<const Item>> items_;
    std::map<std::size_t, std::shared_ptr<const ItemSet>> closures_;
    std::size_t closureNum_ = 0UL;

    std::vector<std::map<std::string, std::int64_t>> actionTable_;
};

}  // namespace lr
