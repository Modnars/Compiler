/*
 * @Author: modnarshen
 * @Date: 2024.10.08 09:57:49
 * @Note: Copyrights (c) 2024 modnarshen. All rights reserved.
 */
#pragma once

#include <memory>
#include <string>

#include "grammar.h"

namespace lr0 {
class Item;
}

bool operator<(const lr0::Item &lhs, const lr0::Item &rhs);

namespace lr0 {

static const std::string DotMark = "·";

class Item {
    friend bool ::operator<(const Item & lhs, const Item & rhs);
    friend std::shared_ptr<Item> NewItem(std::shared_ptr<const Production>, std::size_t, std::shared_ptr<Item>);

public:
    Item(std::shared_ptr<const Production> production, std::size_t dotPos)
        : production_(production), dotPos_(dotPos) { }

    std::string ToString() const;

public:
    bool HasNextSymbol() const {
        return production_->Right().size() > dotPos_ && production_->Right().back() != Grammar::NilMark;
    }

    const std::string &NextSymbol() const {
        static const std::string empty = "";  // return empty string when HasNextSymbol is false
        if (!HasNextSymbol()) {
            return empty;
        }
        return production_->Right()[dotPos_];
    }

    bool CanReduce() const {
        // return !HasNextSymbol();
        return dotPos_ >= production_->Right().size() || production_->Right().back() == Grammar::NilMark;
    }

    std::size_t DotPos() const { return dotPos_; }

    const std::vector<std::string> &Right() const { return production_->Right(); }

    std::shared_ptr<const Production> GetProduction() const { return production_; }

    std::shared_ptr<Item> Shift() const { return shift_; }

private:
    std::shared_ptr<const Production> production_;
    std::size_t dotPos_ = 0UL;
    std::shared_ptr<Item> shift_ = nullptr;
};

inline std::shared_ptr<Item> NewItem(std::shared_ptr<const Production> production, std::size_t dotPos,
                                     std::shared_ptr<Item> prev = nullptr) {
    auto newItem = std::make_shared<Item>(production, dotPos);
    if (prev) {
        prev->shift_ = newItem;
    }
    return newItem;
}

}  // namespace lr0
