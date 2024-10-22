/*
 * @Author: modnarshen
 * @Date: 2024.09.25 11:00:40
 * @Note: Copyrights (c) 2024 modnarshen. All rights reserved.
 */
#pragma once

#include <cstddef>
#include <map>
#include <memory>
#include <ostream>
#include <set>
#include <stack>
#include <vector>

#include "grammar.h"
#include "parser.h"

namespace mcc {

class LR0Item {
public:
    LR0Item(std::shared_ptr<const Production> production, std::size_t dotPos)
        : production_(production), dotPos_(dotPos) { }

    static std::shared_ptr<LR0Item> New(std::shared_ptr<const Production> production, std::size_t dotPos,
                                        std::shared_ptr<LR0Item> prev = nullptr) {
        auto newItem = std::make_shared<LR0Item>(production, dotPos);
        if (prev) {
            prev->shift_ = newItem;
        }
        return newItem;
    }

public:
    bool operator<(const LR0Item &rhs) const {
        return std::tie(*production_, dotPos_) < std::tie(*rhs.production_, rhs.dotPos_);
    }

    bool operator==(const LR0Item &rhs) const {
        return std::tie(*production_, dotPos_) == std::tie(*rhs.production_, rhs.dotPos_);
    }

    std::string ToString() const;

public:
    virtual bool HasNextSymbol() const {
        return production_->Right().size() > dotPos_ && production_->Right().back() != Grammar::NilMark;
    }

    virtual const std::string &NextSymbol() const {
        static const std::string empty = "";  // return empty string when HasNextSymbol is false
        if (!HasNextSymbol()) {
            return empty;
        }
        return production_->Right()[dotPos_];
    }

    virtual bool CanReduce() const {
        // return !HasNextSymbol();
        return dotPos_ >= production_->Right().size() || production_->Right().back() == Grammar::NilMark;
    }

    virtual std::size_t DotPos() const { return dotPos_; }

    virtual const std::vector<std::string> &Right() const { return production_->Right(); }

    virtual std::shared_ptr<const Production> GetProduction() const { return production_; }

    std::shared_ptr<LR0Item> Shift() const { return shift_; }

private:
    std::shared_ptr<const Production> production_;
    std::size_t dotPos_ = 0UL;
    std::shared_ptr<LR0Item> shift_ = nullptr;
};

// inline std::shared_ptr<LR0Item> NewLR0Item(std::shared_ptr<const Production> production, std::size_t dotPos,
//                                            std::shared_ptr<LR0Item> prev = nullptr) {
//     auto newItem = std::make_shared<LR0Item>(production, dotPos);
//     if (prev) {
//         prev->shift_ = newItem;
//     }
//     return newItem;
// }

template <typename _Item>
class ItemSet {
public:
    bool Add(std::shared_ptr<const _Item> item) { return items_.insert(item).second; }
    bool Contains(std::shared_ptr<const _Item> item) const { return items_.find(item) != items_.end(); }
    bool Equals(std::shared_ptr<const ItemSet> rhs) const { return items_ == rhs->items_; };

public:
    const std::set<std::shared_ptr<const _Item>> &Items() const { return items_; };
    std::size_t Number() const { return number_; }
    void SetNumber(std::size_t number) { number_ = number; }

private:
    std::set<std::shared_ptr<const _Item>> items_;
    std::size_t number_ = 0UL;
};

class LRParser : public mcc::Parser {
public:
    LRParser(Grammar &grammar) : grammar_(grammar) { }

public:
    virtual void Analyze(std::istream &is) const override;

public:
    virtual void OutputToGraphviz(std::ostream &os) const override;

    virtual void OutputToCsv(std::ostream &os) const override;

    virtual void OutputToGo(std::ostream &os) const override;

protected:
    void computeAndCacheLr0Items();

    std::shared_ptr<const LR0Item> lr0Item(std::shared_ptr<const Production> p, std::size_t pos) const {
        const auto iter = lr0Items_.find(std::make_pair(p, pos));
        if (iter == lr0Items_.end()) {
            return nullptr;
        }
        return iter->second;
    }

    int fillActionTable(std::size_t stateNum, const std::string &symbol, std::int64_t val);

    int searchActionTable(std::size_t stateNum, const std::string &symbol, std::int64_t &val) const;

protected:
    Grammar &grammar_;
    std::map<std::pair<std::shared_ptr<const Production>, std::size_t>, std::shared_ptr<LR0Item>> lr0Items_;

    std::vector<std::map<std::string, std::int64_t>> actionTable_;
    bool parsedSucc_ = false;  // only has executed `Parse()` and result is succ, the flag can be true.
};

template <typename _Item>
std::set<std::string> ComputeLookahead(const Grammar &grammar, std::shared_ptr<const _Item> lrItem) {
    if (lrItem->CanReduce()) {
        return lrItem->lookahead_;
    }
    if (lrItem->HasNextSymbol() && !grammar.IsNonTerminal(lrItem->NextSymbol())) {
        return lrItem->lookahead_;
    }
    auto iter = lrItem->Right().begin();
    std::advance(iter, lrItem->DotPos() + 1UL);
    auto result = grammar.ComputeFirstSet(iter, lrItem->Right().end());
    if (result.count(Grammar::NilMark)) {
        result.erase(Grammar::NilMark);
        // Compute the FIRST(β lookahead). If FIRST(β) contains ε, then FIRST(β lookahead) contains FIRST(lookahead).
        // Since lookahead is a terminal symbol (`lookahead_` should also be a set of terminal symbols), we can insert
        // lookahead (`lookahead_`) directly.
        result.insert(lrItem->lookahead_.begin(), lrItem->lookahead_.end());
    }
    return result;
}

template <typename _Item>
std::set<std::string> ComputeLookahead(const Grammar &grammar, std::shared_ptr<const _Item> lrItem,
                                       const std::set<std::string> &lookahead) {
    if (lrItem->CanReduce()) {
        return lookahead;
    }
    if (lrItem->HasNextSymbol() && !grammar.IsNonTerminal(lrItem->NextSymbol())) {
        return lookahead;
    }
    auto iter = lrItem->Right().begin();
    std::advance(iter, lrItem->DotPos() + 1UL);
    auto result = grammar.ComputeFirstSet(iter, lrItem->Right().end());
    if (result.count(Grammar::NilMark)) {
        result.erase(Grammar::NilMark);
        // Compute the FIRST(β lookahead). If FIRST(β) contains ε, then FIRST(β lookahead) contains FIRST(lookahead).
        // Since lookahead is a terminal symbol (`lookahead_` should also be a set of terminal symbols), we can insert
        // lookahead (`lookahead_`) directly.
        result.insert(lookahead.begin(), lookahead.end());
    }
    return result;
}

}  // namespace mcc
