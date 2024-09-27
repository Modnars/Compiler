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

#include "parser.h"

namespace slr {
class Item;
}

bool operator<(const slr::Item &lhs, const slr::Item &rhs);

namespace slr {

static const std::string DotMark = "·";

class Item {
    friend bool ::operator<(const Item & lhs, const Item & rhs);

public:
    Item(std::shared_ptr<Production> production, std::size_t dot_pos) : production_(production), dot_pos_(dot_pos) {}

    std::string ToString() const;

public:
    bool HasNextSymbol() const;

    const std::string &NextSymbol() const;

    std::shared_ptr<Item> Shift() const { return this->shift_; }
    void SetShift(std::shared_ptr<Item> shift) { this->shift_ = shift; };

private:
    std::shared_ptr<Production> production_;
    std::size_t dot_pos_ = 0UL;
    std::shared_ptr<Item> shift_;
};

class ItemSet {
public:
    ItemSet() = default;

    bool Add(std::shared_ptr<Item> item) { return this->items_.insert(item).second; }
    bool Contains(std::shared_ptr<Item> item) const { return this->items_.find(item) != this->items_.end(); }

    const std::set<std::shared_ptr<Item>> &Items() const { return items_; }

private:
    std::set<std::shared_ptr<Item>> items_;
};

class Parser {
public:
    Parser(const Grammar &grammar) : productions_(grammar.Productions), productionIndexes_(grammar.ProductionIndexes) {
        makeItems();
    }

    Parser(const std::vector<std::shared_ptr<Production>> &productions,
           const std::map<std::string, std::vector<std::shared_ptr<Production>>> &productionIndexes)
        : productions_(productions), productionIndexes_(productionIndexes) {
        makeItems();
    }

    int CLOSURE(std::shared_ptr<ItemSet> itemSet);

    int Parse();

private:
    int makeItems();

    std::shared_ptr<Item> item(std::shared_ptr<Production> p, std::size_t pos) const {
        const auto iter = item_table_.find(std::make_pair(p, pos));
        if (iter == item_table_.end())
            return nullptr;
        return iter->second;
    }

    std::vector<std::shared_ptr<ItemSet>> shiftItemSets(std::shared_ptr<ItemSet> itemSet);

private:
    const std::vector<std::shared_ptr<Production>> &productions_;
    const std::map<std::string, std::vector<std::shared_ptr<Production>>> &productionIndexes_;
    std::map<std::uint32_t, std::map<std::uint32_t, std::string>> shift_table_; // <I(a), <I(b), mark>>
    std::map<std::pair<std::shared_ptr<Production>, std::size_t>, std::shared_ptr<Item>> item_table_;
    std::map<std::uint32_t, std::shared_ptr<ItemSet>> closures_;
    std::uint32_t closure_id_generator_ = 0U;
};

} // namespace slr
