/*
 * @Author: modnarshen
 * @Date: 2024.09.27 16:17:19
 * @Note: Copyrights (c) 2024 modnarshen. All rights reserved.
 */
#pragma once

#include <map>
#include <memory>
#include <set>
#include <utility>
#include <vector>

#include "grammar.h"

namespace slr {
class Item;

inline std::shared_ptr<Item> NewItem(std::shared_ptr<const Production>, std::size_t,
                                     std::shared_ptr<Item> prev = nullptr);
}  // namespace slr

bool operator<(const slr::Item &lhs, const slr::Item &rhs);

namespace slr {
static const std::string DotMark = "·";

class Item { // TODO use `lr0::Item` directly
    friend bool ::operator<(const Item & lhs, const Item & rhs);
    friend std::shared_ptr<Item> NewItem(std::shared_ptr<const Production>, std::size_t, std::shared_ptr<Item>);

public:
    Item(std::shared_ptr<const Production> production, std::size_t dotPos)
        : production_(production), dotPos_(dotPos) { }

    std::string ToString() const;

public:
    bool HasNextSymbol() const;

    const std::string &NextSymbol() const;

    std::shared_ptr<Item> Shift() const { return this->shift_; }

private:
    std::shared_ptr<const Production> production_;
    std::size_t dotPos_ = 0UL;
    std::shared_ptr<Item> shift_ = nullptr;
};

inline std::shared_ptr<Item> NewItem(std::shared_ptr<const Production> production, std::size_t dotPos,
                                     std::shared_ptr<Item> prev) {
    auto newItem = std::make_shared<Item>(production, dotPos);
    if (prev)
        prev->shift_ = newItem;
    return newItem;
}

class ItemSet {
public:
    ItemSet() = default;

    bool Add(std::shared_ptr<Item> item) { return this->items_.insert(item).second; }
    bool Contains(std::shared_ptr<Item> item) const { return this->items_.find(item) != this->items_.end(); }
    bool Equals(std::shared_ptr<ItemSet> rhs) const { return this->items_ == rhs->items_; }

public:
    const std::set<std::shared_ptr<Item>> &Items() const { return items_; }
    std::vector<std::shared_ptr<ItemSet>> Shift() const;

private:
    std::set<std::shared_ptr<Item>> items_;
};

class Parser {
public:
    Parser(Grammar &grammar) : grammar_(grammar) { makeItems(); }

    int CLOSURE(std::shared_ptr<ItemSet> itemSet);
    std::shared_ptr<ItemSet> GOTO(std::shared_ptr<ItemSet> itemSet, std::string symbol);

    int Parse();

    int ShowDetails() const;

private:
    int makeItems();

    std::shared_ptr<Item> item(std::shared_ptr<const Production> p, std::size_t pos) const {
        const auto iter = itemTable_.find(std::make_pair(p, pos));
        if (iter == itemTable_.end())
            return nullptr;
        return iter->second;
    }

    int calcAllClosure();

private:
    Grammar &grammar_;
    std::map<std::pair<std::shared_ptr<const Production>, std::size_t>, std::shared_ptr<Item>> itemTable_;
    std::map<std::uint32_t, std::shared_ptr<ItemSet>> closures_;
    std::uint32_t closure_id_generator_ = 0U;
};
}  // namespace slr
